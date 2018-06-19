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

			this->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
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
		this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
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
		this->bulletWorld->debugDrawWorld();
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
	void BulletPhysics::MoveCharacter(CollisionData* obj, const K_Math::Vector3& move, float vLimitAngle, float hLimitAngle) {
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


		//���Ɉړ�
		MoveSmooth(obj->GetCollision(), hMove, hLimitAngle, this->toSkyVector);

		//�c�Ɉړ�
		MoveSmooth(obj->GetCollision(), vMove, vLimitAngle, -vMove);
	}

	//�������@�y���ق�
	void BulletPhysics::MoveCharacterDiscrete(CollisionData* obj, const K_Math::Vector3& move, bool vLimitDirection, bool hLimitDirection) {
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


		//���ړ�
		if (hMove.norm() >= 0.001f) {
			//������֎G�Ɉړ�
			MoveCollisionObject(obj->GetCollision(), this->toSkyVector * 0.2f);
			btVector3 fixVector(0.0f, 0.0f, 0.0f);
			if (hLimitDirection) {
				fixVector = -hMove;
			}
			MoveDiscrete(obj->GetCollision(), hMove, fixVector);
			//�����ďd�͕����֎G�ɖ߂�
			MoveDiscrete(obj->GetCollision(), -this->toSkyVector * 0.2f, this->toSkyVector);
		}

		//�c�ړ�
		if (vMove.norm() >= 0.001f) {
			btVector3 fixVector(0.0f, 0.0f, 0.0f);
			if (vLimitDirection) {
				fixVector = -vMove;
			}
			MoveDiscrete(obj->GetCollision(), vMove, fixVector);
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

	void BulletPhysics::MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector) {
		btTransform& trans = obj->getWorldTransform();
		trans.setOrigin(trans.getOrigin() + moveVector);
	}

	void BulletPhysics::MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, const btVector3& limitDirection) {
		int numMove = 1 + moveVector.norm() * 2.0f;
		int numFix = 1 + moveVector.norm() * 0.6f;

		const btVector3& goVec = moveVector / (float)numMove;
		//�ړ�
		DetectMaxDistance contact_cb(obj);
		bool isHit = false;
		for (int i = 0; i < numMove; ++i) {
			MoveCollisionObject(obj, goVec);
			//FixContactCallBack cb(obj, limitDirection);
			//this->bulletWorld->contactTest(obj, cb);
			//if (cb.isHit) {
			//	break;
			//}
			//��Ԑ[���߂荞�񂾂��̖̂@�������։����o��
			do {
				this->bulletWorld->contactTest(obj, contact_cb);
			} while (contact_cb.isLoop);
			//�����o��
			if (!contact_cb.maxDistance) {
				continue;
			}
			//�����������������Ă���ꍇ�͐i�񂾕�������߂�悤�ɂ��������o���Ȃ�
			//if (limitDirection.norm() > 0.0f && false) {

			//	std::cout << contact_cb.m_closestDistanceThreshold << std::endl;
			//	MoveCollisionObject(obj, -goVec * 0.5f);
			//	isHit = true;
			//}
			//else {
			//	MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
			//	isHit = true;
			//	break;
			//}
			if (limitDirection.norm() <= 0.0f) {
				MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
				isHit = true;
				continue;
			}

			auto contactObj = contact_cb.maxObject;
			for (int i = 0; i < 50; ++i) {
				DetectMaxDistance callback(obj);
				this->bulletWorld->contactPairTest(obj, contactObj, callback);
				if (!callback.maxDistance) {
					break;
				}
				//�����������������Ă���ꍇ�͐i�񂾕�������߂�悤�ɂ��������o���Ȃ�
				MoveCollisionObject(obj, -goVec * 0.1f);
				isHit = true;
			}
			if (isHit) {
				break;
			}
		}

	}

	void BulletPhysics::MoveSmooth(btCollisionObject *obj, const btVector3 &moveVector, float limitAngle, const btVector3& limitDirection) {
		if (moveVector.norm() < 0.001f) {
			return;
		}

		//�ړ�����
		btVector3 prevPos = obj->getWorldTransform().getOrigin();
		btVector3 normal = MoveBySweep(obj, moveVector, limitDirection, limitAngle, 0.01f);

		//�i�s������Z���Ƃ����O�����`
		btVector3 yAxis = this->toSkyVector;
		btVector3 xAxis = yAxis.cross(moveVector);
		btVector3 zAxis;
		if (xAxis.norm() > 0.001f) {
			//�c�����Ɖ������Ƀx�N�g���𕪉�
			xAxis.normalize();
			zAxis = xAxis.cross(yAxis).normalized();
		}
		else {
			zAxis = moveVector.normalized();
		}

		//���o���p�x�����m�F
		float angle_cos = (float)normal.dot(this->toSkyVector);
		float limit_cos = (float)btCos(btRadians(limitAngle));
		if (abs(angle_cos - limit_cos) < 0.0001f) {
			angle_cos = limit_cos;
		}

		if (normal.norm() < 0.001f) {
			return;
		}

		//���o��Ȃ��ꍇ�͕ǈ����Ƃ��ĉ������̕ǂ���x�N�g�����쐬
		if (angle_cos < limit_cos) {
			const btVector3& goVecZ = zAxis * zAxis.dot(normal);
			const btVector3& goVecX = xAxis * xAxis.dot(normal);
			normal = (goVecX + goVecZ).normalized();
		}
		else {
			//��Ƃ��Ĉ����ꍇ�́A�i�s�����Ƌ�x�N�g���̍�镽�ʏ�ɖ@��������悤�ɂ���
			const btVector3& goVecZ = zAxis * zAxis.dot(normal);
			const btVector3& goVecY = yAxis * yAxis.dot(normal);
			normal = (goVecY + goVecZ).normalized();
		}

		//�ǂ�������
		float hitFraction = (obj->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();
		btVector3 goVec = moveVector * (1.0f - hitFraction);
		goVec += normal * (-goVec.dot(normal));

		if (goVec.norm() < 0.001f) {
			return;
		}

		//�ړ�����
		MoveDiscrete(obj, goVec, btVector3(0.0f, 0.0f, 0.0f));
	}

	btVector3 BulletPhysics::MoveBySweep(btCollisionObject *obj, const btVector3 &moveVector, const btVector3& limitDirection, float limitAngle, float allowDistance) {
		btConvexShape* shape = (btConvexShape*)obj->getCollisionShape();

		btTransform from = obj->getWorldTransform();
		btTransform to = from;
		to.setOrigin(to.getOrigin() + moveVector);

		btVector3 normal;
		btVector3 fixVector = limitDirection;
		{
			//�@���擾
			SweepTestCallBack convex_cb(obj);
			this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, 0.0f);

			normal = convex_cb.m_hitNormalWorld;
			if (normal.norm() < 0.001f || !convex_cb.hasHit()) {
				normal = btVector3(0, 0, 0);
			}
			else {
				normal.normalize();
			}

			//�ǂ�������p�x�����m�F
			float angle_cos = (float)-normal.dot(moveVector.normalized());
			float limit_cos = (float)btCos(btRadians(limitAngle));
			if (abs(angle_cos - limit_cos) < 0.0001f) {
				angle_cos = limit_cos;
			}
			//�ǂ�������Ȃ��ꍇ�́A�����o��������i�s�����̒�����Ɍ���
			if (angle_cos > limit_cos) {
				fixVector = -moveVector.normalized();
			}
		}

		//sweepTest
		SweepTestCallBack convex_cb(obj);
		this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDistance);

		//sweep���ʔ��f
		if (convex_cb.hasHit()) {
			btVector3 objPos;
			objPos.setZero();
			objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
			objPos -= moveVector.normalized() * allowDistance;
			to.setOrigin(objPos);
		}
		obj->setWorldTransform(to);

		//�킸���Ȃ߂荞�݂������o��
		for (int i = 0; i < 0; ++i) {
			FixContactCallBack contact_cb(obj, fixVector);
			this->bulletWorld->contactTest(obj, contact_cb);
		}


		return normal;
	}

}
