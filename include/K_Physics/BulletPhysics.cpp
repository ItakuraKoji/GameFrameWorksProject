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
	btCollisionShape* BulletPhysics::CreateTriangleMeshShape(btTriangleMesh* mesh) {
		btBvhTriangleMeshShape* bvhMesh = new btBvhTriangleMeshShape(mesh, true);
		this->shapeArray.push_back(bvhMesh);
		return bvhMesh;
	}
	//三角形
	btCollisionShape* BulletPhysics::CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3) {
		btScalar points[9] = { point1.x(),point1.y(),point1.z(),
							   point2.x(),point2.y(),point2.z(),
							   point3.x(),point3.y(),point3.z() };
		btCollisionShape* shape = new btConvexHullShape(points, 3, 3 * sizeof(btScalar));
		this->shapeArray.push_back(shape);
		return shape;
	}
	//球
	btCollisionShape* BulletPhysics::CreateSphereShape(float radius) {
		btCollisionShape* shape = new btSphereShape(radius);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//カプセル
	btCollisionShape* BulletPhysics::CreateCapsuleShape(float radius, float height) {
		btCollisionShape* shape = new btCapsuleShape(radius, height);
		this->shapeArray.push_back(shape);
		return shape;
	}
	//長方形
	btCollisionShape* BulletPhysics::CreateBoxShape(float halfWidth, float halfHeight, float halfDepth) {
		btCollisionShape* shape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));
		this->shapeArray.push_back(shape);
		return shape;
	}

	//剛体
	CollisionData* BulletPhysics::CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
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
		rigid->setCollisionFlags(rigid->getCollisionFlags() & !btCollisionObject::CF_NO_CONTACT_RESPONSE);

		this->bulletWorld->addRigidBody(rigid, 1, mask);

		CollisionTag tag = { "default", 0, nullptr };
		CollisionData* colData = new CollisionData(rigid, tag);
		rigid->setUserPointer(colData);
		return colData;

	}

	CollisionData* BulletPhysics::CreateCollisionObject(btCollisionShape* shape, bool ghost, int mask, const K_Math::Vector3& pos, const K_Math::Vector3& rot) {
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
		this->bulletWorld->addCollisionObject(collision, 1, mask);

		CollisionTag tag = { "default", 0, nullptr };
		CollisionData* colData = new CollisionData(collision, tag);
		collision->setUserPointer(colData);
		return colData;
	}

	//開放
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
	void BulletPhysics::RemoveCollisionShape(btCollisionShape* shape) {
		this->shapeArray.remove(shape);
		delete shape;
		shape = 0;
	}

	//衝突を検出し、結果をポインタで返す
	std::vector<CollisionTag>& BulletPhysics::FindConfrictionObjects(btCollisionObject* myself) {
		CollectCollisionCallBack callback(this->confrictResult);
		this->bulletWorld->contactTest(myself, callback);
		return this->confrictResult;
	}

	//新型衝突シミュレーション
	void BulletPhysics::MoveCharacter(btCollisionObject *obj, const K_Math::Vector3& move) {
		//凹形状とは判定できない
		if (!obj->getCollisionShape()->isConvex()) {
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


		//縦
		btVector3 virtical = vMove;
		MoveSmooth(obj, virtical, 40.0f, true);

		//横
		//上に雑に移動
		MoveDiscrete(obj, this->toSkyVector * 0.15f, true);
		
		//横に移動
		btVector3 horizontal = hMove;
		MoveSmooth(obj, hMove, 0.0f, false);

		//そして下に雑に戻す
		MoveDiscrete(obj, -this->toSkyVector * 0.15f, true);
	}

	//処理が　軽いほう
	void BulletPhysics::MoveCharacterDiscrete(btCollisionObject *obj, const K_Math::Vector3& hMove, const K_Math::Vector3& vMove) {
		int fix = 10;

		//凹形状とは判定できない
		if (!obj->getCollisionShape()->isConvex()) {
			return;
		}
		btVector3 horizontal = btVector3(hMove.x(), hMove.y(), hMove.z()) / (float)fix;
		if (horizontal.norm() >= 0.001f) {
			//移動
			for (int i = 0; i < fix; ++i) {
				btTransform to = obj->getWorldTransform();
				to.setOrigin(to.getOrigin() + horizontal);
				obj->setWorldTransform(to);
				//一番深くめり込んだものの法線方向へ押し出し
				FixContactCallBack contact_cb(obj);
				do {
					this->bulletWorld->contactTest(obj, contact_cb);
				} while (contact_cb.isLoop);
				//押し出し
				MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
			}
		}
		//移動
		btVector3 vertical = btVector3(vMove.x(), vMove.y(), vMove.z()) / (float)fix;
		if (vertical.norm() >= 0.001f) {
			for (int i = 0; i < fix; ++i) {
				btTransform to = obj->getWorldTransform();
				to.setOrigin(to.getOrigin() + vertical);
				obj->setWorldTransform(to);
				//一番深くめり込んだものの法線方向へ押し出し
				FixContactCallBack contact_cb(obj);
				do {
					this->bulletWorld->contactTest(obj, contact_cb);
				} while (contact_cb.isLoop);
				//押し出し
				//垂直成分はそのベクトルにしか押し出さない（坂を滑るので）
				//MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
				MoveCollisionObject(obj, vertical.normalized() * contact_cb.maxDistance);
			}
		}
	}

	void BulletPhysics::SetSkyVector(const K_Math::Vector3& vector) {
		this->toSkyVector = btVector3(vector.x(), vector.y(), vector.z());
	}


	////////
	//private
	////

	void BulletPhysics::MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector) {
		btVector3 objPos = obj->getWorldTransform().getOrigin();
		objPos = objPos + moveVector;
		btTransform trans = obj->getWorldTransform();
		trans.setOrigin(objPos);
		obj->setWorldTransform(trans);
	}

	void BulletPhysics::MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection) {
		int numFix = 10;
		btVector3 goVec = moveVector / (float)numFix;
		//移動
		for (int i = 0; i < numFix; ++i) {
			btTransform to = obj->getWorldTransform();
			to.setOrigin(to.getOrigin() + goVec);
			obj->setWorldTransform(to);
			//一番深くめり込んだものの法線方向へ押し出し
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
		}

	}

	btVector3 BulletPhysics::MoveSmooth(btCollisionObject *obj, const btVector3 &moveVector, float limitAngle, bool limitDirection) {
		if (moveVector.norm() < 0.001f) {
			return btVector3(0, 0, 0);
		}

		float hitFraction = 0.0f;
		btVector3 normal;


		//移動一回目
		btVector3 prevPos = obj->getWorldTransform().getOrigin();
		normal = MoveBySweep(obj, moveVector, limitDirection);
		hitFraction = (obj->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();

		//壁ずりを作る角度かを確認
		float angle_cos = (float)-normal.dot(moveVector.normalized());
		float limit_cos = (float)btCos(btRadians(limitAngle));
		if (abs(angle_cos - limit_cos) < 0.0001f) {
			angle_cos = limit_cos;
		}
		//壁ずりを作らない場合は法線を返す
		if (angle_cos > limit_cos || normal.norm() < 0.001f) {
			return normal;
		}

		//壁ずりを作る
		btVector3 goVec;
		goVec.setZero();
		goVec = moveVector * (1.0f - hitFraction);
		goVec += normal * (-goVec.dot(normal));

		if (goVec.norm() < 0.001f) {
			return btVector3(0, 0, 0);
		}

		//移動二回目
		MoveBySweep(obj, goVec, limitDirection);


		//壁ずり時は衝突法線を返さない
		return btVector3(0, 0, 0);
	}

	btVector3 BulletPhysics::MoveBySweep(btCollisionObject *obj, const btVector3 &moveVector, bool limitDirection, float allowDistance) {
		btConvexShape* shape = (btConvexShape*)obj->getCollisionShape();


		btTransform from = obj->getWorldTransform();
		btTransform to = from;
		to.setOrigin(to.getOrigin() + moveVector);

		//sweepTest
		SweepTestCallBack convex_cb(obj);
		this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDistance);

		//sweep結果反映
		if (convex_cb.hasHit()) {
			btVector3 objPos;
			objPos.setZero();
			obj->setWorldTransform(to);
			objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
			objPos -= moveVector.normalized() * allowDistance;
			to.setOrigin(objPos);
		}
		obj->setWorldTransform(to);

		//指定回数押し出す(今は十回)
		//一番深くめり込んだものの法線方向へ押し出し
		btVector3 prevPos = obj->getWorldTransform().getOrigin();
		btVector3 resultPos = prevPos;

		for (int i = 0; i < 15; ++i) {
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
		btVector3 normal = btVector3(0.0f, 0.0f, 0.0f);
		//normal = obj->getWorldTransform().getOrigin() - prevPos;
		normal = resultPos - prevPos;
		normal.setX((int)(normal.x() * 1000.0f) / 1000.0f);
		normal.setY((int)(normal.y() * 1000.0f) / 1000.0f);
		normal.setZ((int)(normal.z() * 1000.0f) / 1000.0f);
		//normal = convex_cb.m_hitNormalWorld;

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
