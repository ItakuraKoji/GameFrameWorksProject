#include"BulletPhysics.h"

namespace K_Physics {
	////////
	//public
	////

	BulletPhysics::BulletPhysics() {
		this->bulletWorld = nullptr;
		this->solver = nullptr;
		this->broadphase = nullptr;
		this->dispatcher = nullptr;
		this->config = nullptr;

		if (!Initialize()) {
			throw std::runtime_error("Physics Initialize Failed");
		}
	}
	BulletPhysics::~BulletPhysics() {
		Finalize();
	}

	//bulletWorld�̏�����
	bool BulletPhysics::Initialize() {
		try {
			Finalize();

			this->config = new btDefaultCollisionConfiguration;
			this->dispatcher = new btCollisionDispatcher(this->config);
			this->broadphase = new btDbvtBroadphase;
			this->solver = new btSequentialImpulseConstraintSolver;
			this->bulletWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->config);

			this->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints | btIDebugDraw::DBG_DrawAabb);
			this->bulletWorld->setDebugDrawer(&this->debugDrawer);

			this->bulletWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
			this->toSkyVector = btVector3(0.0f, 1.0f, 0.0f);
		}
		catch (...) {
			Finalize();
			return false;
		}
		return true;
	}
	//�J��
	void BulletPhysics::Finalize() {
		for (int i = 0; i < this->shapeArray.size(); ++i) {
			delete this->shapeArray[i];
			this->shapeArray[i] = nullptr;
		}

		if (this->bulletWorld != nullptr) {
			for (int i = this->bulletWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
				btCollisionObject* obj = this->bulletWorld->getCollisionObjectArray()[i];
				RemoveCollisionObject(obj);
			}
			delete this->bulletWorld;
			this->bulletWorld = nullptr;
		}
		if (this->solver != nullptr) {
			delete this->solver;
			this->solver = nullptr;
		}
		if (this->broadphase != nullptr) {
			delete this->broadphase;
			this->broadphase = nullptr;
		}
		if (this->dispatcher != nullptr) {
			delete this->dispatcher;
			this->dispatcher = nullptr;
		}
		if (this->config != nullptr) {
			delete this->config;
			this->config = nullptr;
		}
	}

	//�V�~�����[�V������i�߂�
	void BulletPhysics::Run() {
		this->bulletWorld->stepSimulation(1 / 60.0f, 4, 1 / 60.0f);
	}
	//�f�o�b�O�`��
	void BulletPhysics::DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans) {
		DebugDraw(shader, camera, 0.1f, 200.0f, trans);
	}

	//�f�o�b�O�`��
	void BulletPhysics::DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, float nearClip, float farClip, const K_Math::Matrix4x4& trans) {
		glDisable(GL_DEPTH_TEST);

		shader->UseShader();
		shader->SetVertexShaderSubroutine("NotSkinning");
		shader->SetFragmentShaderSubroutine("None");

		K_Math::Matrix4x4 projection;
		K_Math::MatrixPerspectiveLH(projection, (float)camera->GetScreenWidth(), (float)camera->GetScreenHeight(), nearClip, farClip, camera->GetFieldOfView());

		K_Math::Matrix4x4 mat = projection * camera->GetViewMatrix() * trans;
		shader->SetMatrix(mat);

		this->debugDrawer.StartDraw(camera);
		this->bulletWorld->debugDrawWorld();
		this->debugDrawer.DrawAllLine();
		glEnable(GL_DEPTH_TEST);
	}


	//�I�u�W�F�N�g����
	btTriangleMesh* BulletPhysics::CreateTriangleMesh(K_Math::Vector3* vectice, int numFace) {
		btTriangleMesh *mesh = new btTriangleMesh;

		for (int i = 0; i < numFace; ++i) {
			mesh->addTriangle(btVector3(vectice[i * 3].x, vectice[i * 3].y, vectice[i * 3].z),
				btVector3(vectice[i * 3 + 1].x, vectice[i * 3 + 1].y, vectice[i * 3 + 1].z),
				btVector3(vectice[i * 3 + 2].x, vectice[i * 3 + 2].y, vectice[i * 3 + 2].z), false);
		}
		return mesh;
	}
	//�����̎O�p�`����Ȃ郁�b�V���R���W����
	CollisionShape* BulletPhysics::CreateTriangleMeshShape(btTriangleMesh* mesh) {
		btBvhTriangleMeshShape* bvhMesh = new btBvhTriangleMeshShape(mesh, true);
		this->shapeArray.push_back(bvhMesh);
		return bvhMesh;
	}
	//�O�p�`
	CollisionShape* BulletPhysics::CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3) {
		btScalar points[9] = { point1.x,point1.y,point1.z,
							   point2.x,point2.y,point2.z,
							   point3.x,point3.y,point3.z };
		btCollisionShape* shape = new btConvexHullShape(points, 3, 3 * sizeof(btScalar));
		this->shapeArray.push_back(shape);
		return shape;
	}
	//��
	CollisionShape* BulletPhysics::CreateSphereShape(float radius) {
		btCollisionShape* shape = new btSphereShape(radius);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//�J�v�Z��
	CollisionShape* BulletPhysics::CreateCapsuleShape(float radius, float height) {
		btCollisionShape* shape = new btCapsuleShape(radius, height);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//�����`
	CollisionShape* BulletPhysics::CreateBoxShape(float halfWidth, float halfHeight, float halfDepth) {
		btCollisionShape* shape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));
		this->shapeArray.push_back(shape);
		return shape;
	}

	//���̍쐬
	RigidBodyData* BulletPhysics::CreateRigidBody(btCollisionShape* shape, btScalar mass, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
		btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z) * btQuaternion(btVector3(1, 0, 0), rot.x) * btQuaternion(btVector3(0, 1, 0), rot.y);
		trans.setRotation(q);

		btVector3 inertia(0.0f, 0.0f, 0.0f);
		if (mass > 0.0f) {
			shape->calculateLocalInertia(mass, inertia);
		}
		btDefaultMotionState *state = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo info(mass, state, shape, inertia);
		btRigidBody *rigid = new btRigidBody(info);
		if (ghost) {
			rigid->setCollisionFlags(rigid->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		else {
			rigid->setCollisionFlags(rigid->getCollisionFlags() & !btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		this->bulletWorld->addRigidBody(rigid, myselfMask, giveMask);
		CollisionTag tag = { "", 0, nullptr };
		RigidBodyData* colData = new RigidBodyData(rigid, myselfMask, giveMask, tag);
		rigid->setUserPointer(colData);
		return colData;
	}

	//�R���W�����쐬
	CollisionData* BulletPhysics::CreateCollisionObject(btCollisionShape* shape, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
		try {
			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
			btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z) * btQuaternion(btVector3(1, 0, 0), rot.x) * btQuaternion(btVector3(0, 1, 0), rot.y);
			trans.setRotation(q);

			btCollisionObject *collision = new btCollisionObject;

			collision->setCollisionShape(shape);
			if (ghost) {
				collision->setCollisionFlags(collision->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			}
			else {
				collision->setCollisionFlags(collision->getCollisionFlags() & !btCollisionObject::CF_NO_CONTACT_RESPONSE);
			}

			collision->setWorldTransform(trans);
			this->bulletWorld->addCollisionObject(collision, myselfMask, giveMask);

			CollisionTag tag = { "", 0, nullptr };
			CollisionData* colData = new CollisionData(collision, myselfMask, giveMask, tag);
			collision->setUserPointer(colData);
			return colData;
		}
		catch (std::exception& e) {
			throw e;
		}
	}

	//�J��

	void BulletPhysics::RemoveCollision(CollisionData** collision) {
		RemoveCollisionObject((*collision)->GetCollision());
		collision = nullptr;
	}
	void BulletPhysics::RemoveCollision(RigidBodyData** collision) {
		RemoveCollisionObject((*collision)->GetCollision());
		collision = nullptr;
	}

	void BulletPhysics::RemoveCollisionShape(CollisionShape** shape) {
		this->shapeArray.remove(*shape);
		delete *shape;
		shape = nullptr;
	}

	//�Փ˂����o���A���ʂ��|�C���^�ŕԂ�
	std::vector<CollisionTag*>& BulletPhysics::FindConfrictionObjects(CollisionData* myself) {
		if (myself == nullptr) {
			this->confrictResult.clear();
			return this->confrictResult;
		}
		CollectCollisionCallBack callback(myself->GetCollision(), this->confrictResult);
		this->bulletWorld->contactTest(myself->GetCollision(), callback);
		return this->confrictResult;
	}

	//�V�^�Փ˃V�~�����[�V����
	void BulletPhysics::MoveCharacter(CollisionData* obj, const K_Math::Vector3& move, float vLimitAngle, float hLimitAngle, float rayLength) {
		//���`��Ƃ͔���ł��Ȃ�
		if (!obj->GetCollision()->getCollisionShape()->isConvex()) {
			return;
		}
		const btVector3& moveVec = btVector3(move.x, move.y, move.z);
		btVector3 vMove;
		btVector3 hMove;
		btVector3 yAxis = this->toSkyVector;
		btVector3 xAxis = yAxis.cross(moveVec);

		//�c�����Ɖ������Ƀx�N�g���𕪉�
		if (xAxis.norm() > 0.001f) {
			xAxis.normalize();
			btVector3 zAxis = xAxis.cross(yAxis).normalized();
			vMove = yAxis * yAxis.dot(moveVec);
			hMove = moveVec - vMove;
		}
		else {
			vMove = btVector3(moveVec);
			hMove = btVector3(0.0f, 0.0f, 0.0f);
		}


		//�c�Ɉړ�
		btVector3 floorNormal = MoveSmooth(obj, vMove, vLimitAngle, -vMove);

		//���o���p�x�����m�F
		float angle_cos = (float)floorNormal.dot(this->toSkyVector);
		float limit_cos = (float)btCos(btRadians(hLimitAngle));
		if (abs(angle_cos - limit_cos) < 0.0001f) {
			angle_cos = limit_cos;
		}

		//�c�ړ����ɓo��鏰���������ꍇ�́A���̖@�����g���ď��ɉ����ĕ����悤�ɂ���
		if (floorNormal.norm() > 0.001f && angle_cos >= limit_cos) {
			btVector3 floorXAxis = floorNormal.cross(hMove);
			if (floorXAxis.norm() > 0.001f) {
				hMove = floorXAxis.cross(floorNormal);
			}
		}

		//���Ɉړ�
		MoveSmooth(obj, hMove, hLimitAngle, this->toSkyVector);
	}

	//�������@�y���ق�
	void BulletPhysics::MoveCharacterDiscrete(CollisionData* obj, const K_Math::Vector3& move, float moveDitail, float vLimitAngle, float hLimitAngle) {
		const btVector3& moveVec = btVector3(move.x, move.y, move.z);
		btVector3 vMove;
		btVector3 hMove;
		btVector3 yAxis = this->toSkyVector;
		btVector3 xAxis = yAxis.cross(moveVec);

		if (xAxis.norm() > 0.001f) {
			//�c�����Ɖ������Ƀx�N�g���𕪉�
			xAxis.normalize();
			btVector3 zAxis = xAxis.cross(yAxis).normalized();
			vMove = yAxis * yAxis.dot(moveVec);
			hMove = moveVec - vMove;
		}
		else {
			//�ړ��������d�͕����x�N�g���ƕ��s�Ȃ�AhMove�͍��Ȃ�
			vMove = btVector3(moveVec);
			hMove = btVector3(0.0f, 0.0f, 0.0f);
		}

		//���`��Ƃ͔���ł��Ȃ�
		if (!obj->GetCollision()->getCollisionShape()->isConvex()) {
			return;
		}
		//�c�ړ�
		btVector3 floorNormal;
		if (vMove.norm() >= 0.001f) {

			bool wallFollowResult;
			floorNormal = MoveByDiscrete(obj, vMove, vLimitAngle, 0.001f, 0.05f, wallFollowResult);

			//�c�ړ����ɓo��鏰���������ꍇ�́A���̖@�����g���ď��ɉ����ĕ����悤�ɂ���
			//(�c�ړ����ɕǂ��肪�Ȃ������ꍇ)
			if (!wallFollowResult && floorNormal.norm() >= 0.001f) {
				btVector3 floorXAxis = floorNormal.cross(hMove);
				if (floorXAxis.norm() > 0.001f) {
					//���̖@����������𒲐�
					hMove = floorXAxis.cross(floorNormal);
				}
			}
			else if(floorNormal.norm() >= 0.001f) {
			}
		}
		{
			//�߂荞�݂�����
			FixContactCallBack fix_cb(obj->GetCollision(), btVector3(0.0f, 1.0f, 0.0f));
			this->bulletWorld->contactTest(obj->GetCollision(), fix_cb);
		}


		//���ړ�
		if (hMove.norm() >= 0.001f) {
			bool wallFollowResult;
			MoveByDiscrete(obj, hMove, hLimitAngle, 0.001f, 0.05f, wallFollowResult);
		}
		if (hMove.norm() >= 0.001f) {
			//�߂荞�݂�����
			FixContactCallBack fix_cb(obj->GetCollision(), -hMove.normalized());
			this->bulletWorld->contactTest(obj->GetCollision(), fix_cb);
		}
	}

	float BulletPhysics::Raycast(const K_Math::Vector3& from, const K_Math::Vector3& to, int myselfMask) {
		const btVector3& fromPos = btVector3(from.x, from.y, from.z);
		const btVector3& toPos = btVector3(to.x, to.y, to.z);
		MyRaycastCallBack ray_cb(fromPos, toPos, myselfMask);
		this->bulletWorld->rayTest(fromPos, toPos, ray_cb);
		return ray_cb.m_closestHitFraction;
	}

	void BulletPhysics::SetSkyVector(const K_Math::Vector3& vector) {
		this->toSkyVector = btVector3(vector.x, vector.y, vector.z).normalized();
	}


	////////
	//private
	////
	void BulletPhysics::RemoveCollisionObject(btCollisionObject* obj) {
		if (obj == nullptr) {
			return;
		}
		btRigidBody* rigid = btRigidBody::upcast(obj);
		if (rigid != nullptr) {
			if (rigid->getMotionState()) {
				delete rigid->getMotionState();
			}
		}
		if (obj->getUserPointer()) {
			delete (CollisionData*)obj->getUserPointer();
		}
		this->bulletWorld->removeCollisionObject(obj);
		delete obj;
	}

	void BulletPhysics::MoveCollisionObject(CollisionData* obj, const btVector3& moveVector) {
		btTransform& trans = obj->GetCollision()->getWorldTransform();
		trans.setOrigin(trans.getOrigin() + moveVector);
	}



	btVector3 BulletPhysics::MoveSmooth(CollisionData *obj, const btVector3 &moveVector, float limitAngle, const btVector3& limitDirection) {
		FixContactCallBack fix_cb(obj->GetCollision(), btVector3(0.0f, 1.0f, 0.0f));
		this->bulletWorld->contactTest(obj->GetCollision(), fix_cb);

		if (moveVector.norm() < 0.001f) {
			return btVector3(0.0f, 0.0f, 0.0f);
		}

		//�ړ�����
		btVector3 prevPos = obj->GetCollision()->getWorldTransform().getOrigin();
		btVector3 normal = MoveBySweep(obj, moveVector, limitDirection, limitAngle, 0.01f);

		//�i�s������Z���Ƃ����O�����`
		btVector3 zAxis = moveVector.normalized();
		btVector3 xAxis = this->toSkyVector.cross(zAxis);
		btVector3 yAxis;
		if (xAxis.norm() > 0.001f) {
			yAxis = xAxis.cross(zAxis);
		}
		else {
			yAxis = this->toSkyVector;
		}

		//���o���p�x�����m�F
		float angle_cos = (float)normal.dot(this->toSkyVector);
		float limit_cos = (float)btCos(btRadians(limitAngle));
		if (abs(angle_cos - limit_cos) < 0.0001f) {
			angle_cos = limit_cos;
		}
		if (normal.norm() < 0.001f) {
			return btVector3(0.0f, 0.0f, 0.0f);
		}

		btVector3 fixedNormal;
		//�^��ւ̈ړ���^���ւ̈ړ���xAxis��0�ƂȂ�̂ŁA���̔���
		if (xAxis.norm() > 0.001f) {
			//���o��Ȃ��ꍇ�͕ǈ����Ƃ��ĉ������̕ǂ���x�N�g�����쐬
			if (angle_cos < limit_cos) {
				const btVector3& goVecZ = zAxis * zAxis.dot(normal);
				const btVector3& goVecX = xAxis * xAxis.dot(normal);
				fixedNormal = (goVecX + goVecZ).normalized();
			}
			else {
				//��Ƃ��Ĉ����ꍇ�́A�i�s�����Ƌ�x�N�g���̍�镽�ʏ�ɖ@��������悤�ɂ���
				const btVector3& goVecZ = zAxis * zAxis.dot(normal);
				const btVector3& goVecY = yAxis * yAxis.dot(normal);
				fixedNormal = (goVecY + goVecZ).normalized();
			}
		}
		else {
			//�d�͗����̏ꍇ�́A�ǂ���ɕ����������v��Ȃ�
			fixedNormal = normal;
		}

		//�ǂ�������
		float hitFraction = (obj->GetCollision()->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();
		btVector3 goVec = moveVector * (1.0f - hitFraction);
		goVec += fixedNormal * (-goVec.dot(fixedNormal));

		if (goVec.norm() < 0.001f) {
			return normal;
		}

		//�ړ�����
		//MoveBySweep(obj, goVec, limitDirection, limitAngle, 0.01f);
		//MoveCharacterDiscrete(obj, K_Math::Vector3(goVec.x(), goVec.y(), goVec.z()), 0.005f, true, true);
		MoveDiscrete(obj, goVec, btVector3(0.0f, 0.0f, 0.0f), 0.01f);
		return normal;
	}

	btVector3 BulletPhysics::MoveBySweep(CollisionData *obj, const btVector3 &moveVector, const btVector3& limitDirection, float limitAngle, float allowDistance) {
		btConvexShape* shape = (btConvexShape*)obj->GetCollision()->getCollisionShape();

		float allowDis = allowDistance;
		SweepTestCallBack convex_cb(obj->GetCollision());

		btTransform from = obj->GetCollision()->getWorldTransform();
		btTransform to = from;
		//�]���Ɉړ�
		to.setOrigin(to.getOrigin() + moveVector + moveVector.normalized() * allowDis);

		//sweepTest
		this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDis);

		//sweep���ʔ��f
		if (convex_cb.hasHit()) {
			btVector3 objPos;
			objPos.setZero();
			objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
			to.setOrigin(objPos);
		}
		obj->GetCollision()->setWorldTransform(to);

		//�@���́A��Ԃ߂荞��ł���K�p
		DetectMaxDistance callback(obj->GetCollision());
		this->bulletWorld->contactTest(obj->GetCollision(), callback);
		btVector3 normal;
		normal = callback.fixVec;

		//�I�u�W�F�N�g�̈ʒu�ɔ��f
		btVector3 objPos = obj->GetCollision()->getWorldTransform().getOrigin();
		objPos -= moveVector.normalized() * allowDis;
		to.setOrigin(objPos);
		obj->GetCollision()->setWorldTransform(to);

		return normal;
	}

	void BulletPhysics::MoveRotation(CollisionData* obj, const K_Math::Quaternion& from, const K_Math::Quaternion& to, float speed) {
		float t = 0.0f;
		
		do {
			//�����Â�]
			t += speed;
			if (t > 1.0f) {
				t = 1.0f;
			}

			K_Math::Quaternion rotation = K_Math::Slerp(from, to, t);

			//��]
			K_Math::Quaternion prevRot = obj->GetCollisionRotation();
			obj->SetCollisionRotation(rotation);
			//����
			FixContactCallBack callback(obj->GetCollision(), btVector3(0.0f, 0.0f, 0.0f));
			this->bulletWorld->contactTest(obj->GetCollision(), callback);
		} while (t < 1.0f);

	}


	btVector3 BulletPhysics::MoveDiscrete(CollisionData* obj, const btVector3& moveVector, const btVector3& limitDirection, float moveDitail) {
		int numMove = 1 + (int)(moveVector.norm() / moveDitail);
		int numFix = 35;

		const btVector3& goVec = moveVector / (float)numMove;
		btVector3 normal(0.0f, 0.0f, 0.0f);
		//�ړ�
		DetectMaxDistance contact_cb(obj->GetCollision());
		bool isHit = false;
		//������Ƃ����肵�Ȃ���O�i
		for (int i = 0; i < numMove; ++i) {
			MoveCollisionObject(obj, goVec);
			//��Ԑ[���߂荞�񂾂��̂�������
			this->bulletWorld->contactTest(obj->GetCollision(), contact_cb);
			if (!contact_cb.maxDistance) {
				continue;
			}
			normal = contact_cb.fixVec;

			//�����������Ȃ��ꍇ�͖@�������ɉ����o��
			if (limitDirection.norm() <= 0.0f) {
				MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
				//�����o��2���
				for (int i = 0; i < 10; ++i) {
					contact_cb = DetectMaxDistance(obj->GetCollision());
					this->bulletWorld->contactTest(obj->GetCollision(), contact_cb);
					if (contact_cb.maxDistance) {
						if (contact_cb.fixVec.normalized().angle(goVec.normalized()) < btRadians(90.0f)) {
							continue;
						}

						MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
					}
				}
				isHit = true;
				continue;
			}

			//�����������������Ă���ꍇ�͂��̕����ɉ����o��
			auto contactObj = contact_cb.maxObject;
			for (int j = 0; j < numFix; ++j) {
				MoveCollisionObject(obj, limitDirection * 0.01f);
				DetectMaxDistance callback(obj->GetCollision());
				this->bulletWorld->contactPairTest(obj->GetCollision(), contactObj, callback);
				if (!callback.maxDistance) {
					break;
				}
				isHit = true;
			}
			if (isHit) {
				break;
			}
		}
		return normal;
	}

	//�ړ����L�����Z�����鎮�̂߂荞�܂Ȃ��ړ�
	btVector3 BulletPhysics::MoveDiscrete2(CollisionData* obj, const btVector3& moveVector, float backDistance, float moveDitailRatio){

		btVector3 normal(0.0f, 0.0f, 0.0f);
		//�ړ�
		DetectMaxDistance contact_cb(obj->GetCollision());
		bool isHit = false;


		//�R�s�[
		btVector3 moveVec = moveVector;

		//�ׂ����i��łԂ����Ă͖߂�A�X�ɍׂ����i��łԂ����Ă͖߂�A�œK�Ȉʒu���ߎ�
		for (int i = 0; i < 5; ++i) {
			//������Ƃ����肵�Ȃ���O�i
			int numMove = 1 + (int)(moveVec.norm() / moveDitailRatio);
			const btVector3& goVec = moveVec / (float)numMove;

			for (int i = 0; i < numMove; ++i) {
				MoveCollisionObject(obj, goVec);
				//��Ԑ[���߂荞�񂾂��̂�������
				this->bulletWorld->contactTest(obj->GetCollision(), contact_cb);
				if (!contact_cb.maxDistance) {
					//�Ԃ����ĂȂ��Ƃ��͌p��
					moveVec -= goVec;
					continue;
				}
				isHit = true;
				//�@�����L�^
				normal = contact_cb.fixVec;

				//�Ԃ��������͋t�߂�
				MoveCollisionObject(obj, -goVec);
				break;
			}
		}
		//�߂荞�݂��Â����邽�߂ɒ�ʂ̉����߂����s��(�ǂ��肪���������邽��)
		MoveCollisionObject(obj, -moveVector.normalized() * backDistance);

		return normal;
	}

	//�ǂ�����܂ޗ��U�ړ�
	btVector3 BulletPhysics::MoveByDiscrete(CollisionData* obj, const btVector3& moveVector, float limitAngle, float allowDistance, float moveDitailRatio, bool& wallFollowResult) {
		wallFollowResult = false;

		//�c�ړ�
		btVector3 floorNormal = btVector3(0.0f, 0.0f, 0.0f);
		if (moveVector.norm() >= 0.001f) {
			btVector3 prevPos = obj->GetCollision()->getWorldTransform().getOrigin();
			floorNormal = MoveDiscrete2(obj, moveVector, 0.0f, moveDitailRatio);

			if (floorNormal.norm() < 0.001f) {
				return floorNormal;
			}

			//���o���p�x�����m�F
			float angle_cos = (float)floorNormal.dot(this->toSkyVector);
			float limit_cos = (float)btCos(btRadians(limitAngle));
			if (abs(angle_cos - limit_cos) < 0.0001f) {
				angle_cos = limit_cos;
			}

			//�w��̊p�x������@���p�x����ꂽ�Ƃ��A�ǂ���x�N�g�����쐬
			if (angle_cos < limit_cos) {

				//�ǂ�������
				float hitFraction = (obj->GetCollision()->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();
				//�c���Ă���ړ��x�N�g�����Z�o
				btVector3 goVec = moveVector * (1.0f - hitFraction);
				goVec += floorNormal * (-goVec.dot(floorNormal));

				if (goVec.norm() < 0.001f) {
					return floorNormal;
				}
				//�ǂɉ����Ĉړ�
				MoveDiscrete2(obj, goVec, allowDistance, moveDitailRatio);

				//�ǂ��肵���A�Ƃ������ʂ��L�^
				wallFollowResult = true;
			}
		}
		return floorNormal;
	}



}
