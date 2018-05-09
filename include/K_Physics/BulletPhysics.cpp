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
			throw("Physics Initialize Failed");
		}
	}
	BulletPhysics::~BulletPhysics() {
		Finalize();
	}

	//bulletWorldの初期化
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
			return false;
		}
		return true;
	}
	//開放
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

	//シミュレーションを進める
	void BulletPhysics::Run() {
		this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
	}
	//デバッグ描画
	void BulletPhysics::DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans) {
		glDisable(GL_DEPTH_TEST);

		shader->UseShader();
		shader->SetVertexShaderSubroutine("NotSkinning");
		shader->SetFragmentShaderSubroutine("None");

		K_Math::Matrix4x4 projection;
		K_Math::MatrixPerspectiveLH(projection, (float)camera->GetScreenWidth(), (float)camera->GetScreenHeight(), 0.1f, 200.0f, camera->GetFieldOfView());

		K_Math::Matrix4x4 mat = projection * camera->GetViewMatrix() * trans;
		shader->SetMatrix(mat);
		this->bulletWorld->debugDrawWorld();
		glEnable(GL_DEPTH_TEST);
	}

	//オブジェクト生成
	btTriangleMesh* BulletPhysics::CreateTriangleMesh(K_Math::Vector3* vectice, int numFace) {
		btTriangleMesh *mesh = new btTriangleMesh;

		for (int i = 0; i < numFace; ++i) {
			mesh->addTriangle(btVector3(vectice[i * 3].x(), vectice[i * 3].y(), vectice[i * 3].z()),
				btVector3(vectice[i * 3 + 1].x(), vectice[i * 3 + 1].y(), vectice[i * 3 + 1].z()),
				btVector3(vectice[i * 3 + 2].x(), vectice[i * 3 + 2].y(), vectice[i * 3 + 2].z()), false);
		}
		return mesh;
	}
	//複数の三角形からなるメッシュコリジョン
	CollisionShape* BulletPhysics::CreateTriangleMeshShape(btTriangleMesh* mesh) {
		btBvhTriangleMeshShape* bvhMesh = new btBvhTriangleMeshShape(mesh, true);
		this->shapeArray.push_back(bvhMesh);
		return bvhMesh;
	}
	//三角形
	CollisionShape* BulletPhysics::CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3) {
		btScalar points[9] = { point1.x(),point1.y(),point1.z(),
							   point2.x(),point2.y(),point2.z(),
							   point3.x(),point3.y(),point3.z() };
		btCollisionShape* shape = new btConvexHullShape(points, 3, 3 * sizeof(btScalar));
		this->shapeArray.push_back(shape);
		return shape;
	}
	//球
	CollisionShape* BulletPhysics::CreateSphereShape(float radius) {
		btCollisionShape* shape = new btSphereShape(radius);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//カプセル
	CollisionShape* BulletPhysics::CreateCapsuleShape(float radius, float height) {
		btCollisionShape* shape = new btCapsuleShape(radius, height);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//長方形
	CollisionShape* BulletPhysics::CreateBoxShape(float halfWidth, float halfHeight, float halfDepth) {
		btCollisionShape* shape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));
		this->shapeArray.push_back(shape);
		return shape;
	}

	//剛体作成
	RigidBodyData* BulletPhysics::CreateRigidBody(btCollisionShape* shape, btScalar mass, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
		btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z()) * btQuaternion(btVector3(1, 0, 0), rot.x()) * btQuaternion(btVector3(0, 1, 0), rot.y());
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

		this->bulletWorld->addRigidBody(rigid, 1, myselfMask | giveMask);
		CollisionTag tag = { "default", 0, nullptr };
		RigidBodyData* colData = new RigidBodyData(rigid, myselfMask, giveMask, tag);
		rigid->setUserPointer(colData);
		return colData;
	}

	//コリジョン作成
	CollisionData* BulletPhysics::CreateCollisionObject(btCollisionShape* shape, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
		btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z()) * btQuaternion(btVector3(1, 0, 0), rot.x()) * btQuaternion(btVector3(0, 1, 0), rot.y());
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
		this->bulletWorld->addCollisionObject(collision, 1, myselfMask | giveMask);

		CollisionTag tag = { "default", 0, nullptr };
		CollisionData* colData = new CollisionData(collision, myselfMask, giveMask, tag);
		collision->setUserPointer(colData);
		return colData;
	}

	//開放

	void BulletPhysics::RemoveCollision(CollisionData** collision) {
		RemoveCollisionObject((*collision)->GetCollision());
		collision = nullptr;
	}

	void BulletPhysics::RemoveCollisionShape(CollisionShape** shape) {
		this->shapeArray.remove(*shape);
		delete *shape;
		shape = nullptr;
	}

	//衝突を検出し、結果をポインタで返す
	std::vector<CollisionTag*>& BulletPhysics::FindConfrictionObjects(CollisionData* myself) {
		CollectCollisionCallBack callback(this->confrictResult);
		this->bulletWorld->contactTest(myself->GetCollision(), callback);
		return this->confrictResult;
	}

	//新型衝突シミュレーション
	void BulletPhysics::MoveCharacter(CollisionData *obj, const K_Math::Vector3& move, float vLimitAngle, float hLimitAngle) {
		//凹形状とは判定できない
		if (!obj->GetCollision()->getCollisionShape()->isConvex()) {
			return;
		}
		btVector3 moveVec = btVector3(move.x(), move.y(), move.z());
		btVector3 vMove;
		btVector3 hMove;
		btVector3 yAxis = this->toSkyVector;
		btVector3 xAxis = yAxis.cross(moveVec);

		//縦成分と横成分にベクトルを分解
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


		//縦に移動
		btVector3 virtical = vMove;
		MoveSmooth(obj->GetCollision(), virtical, vLimitAngle, true);

		//横
		//空方向へ雑に移動
		MoveDiscrete(obj->GetCollision(), this->toSkyVector * 0.15f, true);
		
		//横に移動
		btVector3 horizontal = hMove;
		MoveSmooth(obj->GetCollision(), hMove, hLimitAngle, false);

		//そして重力方向へ雑に戻す
		MoveDiscrete(obj->GetCollision(), -this->toSkyVector * 0.15f, true);
	}

	//処理が　軽いほう
	void BulletPhysics::MoveCharacterDiscrete(CollisionData *obj, const K_Math::Vector3& move, bool vLimitDirection, bool hLimitDirection) {
		btVector3 moveVec = btVector3(move.x(), move.y(), move.z());
		btVector3 vMove;
		btVector3 hMove;
		btVector3 yAxis = this->toSkyVector;
		btVector3 xAxis = yAxis.cross(moveVec);

		if (xAxis.norm() > 0.001f) {
			//縦成分と横成分にベクトルを分解
			xAxis.normalize();
			btVector3 zAxis = xAxis.cross(yAxis).normalized();
			vMove = yAxis * yAxis.dot(moveVec);
			hMove = moveVec - vMove;
		}
		else {
			//移動方向が重力方向ベクトルと平行なら、hMoveは作らない
			vMove = btVector3(moveVec);
			hMove = btVector3(0.0f, 0.0f, 0.0f);
		}

		//凹形状とは判定できない
		if (!obj->GetCollision()->getCollisionShape()->isConvex()) {
			return;
		}

		//横移動
		if (hMove.norm() >= 0.001f) {
			MoveDiscrete(obj->GetCollision(), hMove, hLimitDirection);
		}
		//縦移動
		if (vMove.norm() >= 0.001f) {
			MoveDiscrete(obj->GetCollision(), vMove, vLimitDirection);
		}
	}

	void BulletPhysics::SetSkyVector(const K_Math::Vector3& vector) {
		this->toSkyVector = btVector3(vector.x(), vector.y(), vector.z());
	}


	////////
	//private
	////
	void BulletPhysics::RemoveCollisionObject(btCollisionObject* obj) {
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

	void BulletPhysics::MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection) {
		int numFix = 10;
		btVector3 goVec = moveVector / (float)numFix;
		//移動
		for (int i = 0; i < numFix; ++i) {
			MoveCollisionObject(obj, goVec);
			//一番深くめり込んだものの法線方向へ押し出し
			FixContactCallBack contact_cb(obj);
			do {
				this->bulletWorld->contactTest(obj, contact_cb);
			} while (contact_cb.isLoop);
			//押し出し
			if (!contact_cb.maxDistance) {
				continue;
			}

			//方向制限がかかっている場合は進んだ方向から戻るようにしか押し出せない
			if (limitDirection) {
				MoveCollisionObject(obj, moveVector * contact_cb.maxDistance);
			}
			else {
				MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
			}
		}

	}

	void BulletPhysics::MoveSmooth(btCollisionObject *obj, const btVector3 &moveVector, float limitAngle, bool limitDirection) {
		if (moveVector.norm() < 0.001f) {
			return;
		}

		//移動一回目
		btVector3& prevPos = obj->getWorldTransform().getOrigin();
		btVector3 normal = MoveBySweep(obj, moveVector, limitDirection);

		//壁ずりを作る角度かを確認
		float angle_cos = (float)-normal.dot(moveVector.normalized());
		float limit_cos = (float)btCos(btRadians(limitAngle));
		if (abs(angle_cos - limit_cos) < 0.0001f) {
			angle_cos = limit_cos;
		}
		//壁ずりを作らない場合は帰る
		if (angle_cos > limit_cos || normal.norm() < 0.001f) {
			return;
		}

		//壁ずりを作る
		float hitFraction = (obj->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();
		btVector3 goVec = moveVector * (1.0f - hitFraction);
		goVec += normal * (-goVec.dot(normal));

		if (goVec.norm() < 0.001f) {
			return;
		}

		//移動二回目
		MoveBySweep(obj, goVec, limitDirection);
	}

	btVector3 BulletPhysics::MoveBySweep(btCollisionObject *obj, const btVector3 &moveVector, bool limitDirection, float allowDistance) {
		btConvexShape* shape = (btConvexShape*)obj->getCollisionShape();


		btTransform& from = obj->getWorldTransform();
		btTransform to = from;
		to.setOrigin(to.getOrigin() + moveVector);

		//sweepTest
		SweepTestCallBack convex_cb(obj);
		this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDistance);

		//sweep結果反映
		if (convex_cb.hasHit()) {
			btVector3 objPos;
			objPos.setZero();
			objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
			objPos -= moveVector.normalized() * allowDistance;
			to.setOrigin(objPos);
		}
		obj->setWorldTransform(to);

		//一番深くめり込んだものの法線方向へ押し出し
		btVector3& prevPos = obj->getWorldTransform().getOrigin();
		btVector3 resultPos = prevPos;

		//指定回数押し出す(今は8回)
		for (int i = 0; i < 8; ++i) {
			FixContactCallBack contact_cb(obj);
			do {
				this->bulletWorld->contactTest(obj, contact_cb);
			} while (contact_cb.isLoop);

			//押し出し
			if (!contact_cb.maxDistance) {
				continue;
			}
			if (limitDirection) {
				MoveCollisionObject(obj, moveVector * contact_cb.maxDistance);
			}
			else {
				MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
			}
			resultPos += contact_cb.fixVec * -contact_cb.maxDistance;
		}

		//小数点第四位以下を切り捨て
		btVector3 normal = resultPos - prevPos;
		normal.setX((int)(normal.x() * 1000.0f) / 1000.0f);
		normal.setY((int)(normal.y() * 1000.0f) / 1000.0f);
		normal.setZ((int)(normal.z() * 1000.0f) / 1000.0f);

		if (normal.norm() < 0.001f) {
			normal = btVector3(0, 0, 0);
		}
		else {
			normal.normalize();
		}

		//押し出した方向を法線とする
		return normal;
	}

}
