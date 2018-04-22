#include"BulletPhysics.h"

namespace K_Physics {

	////////
	//public
	////

	SweepTestCallBack::SweepTestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
	}

	//自身と衝突しないsweepTestのコールバック
	btScalar SweepTestCallBack::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		//自分が衝突しないオブジェクトの場合
		if (this->myself->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//相手が衝突しないオブジェクトの場合
		if (convexResult.m_hitCollisionObject->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//相手とマスクが合わない場合
		CollisionData* data1 = (CollisionData*)this->myself->getUserPointer();
		CollisionData* data2 = (CollisionData*)convexResult.m_hitCollisionObject->getUserPointer();
		if (!(data1->mask & data2->mask)) {
			return btScalar(0.0f);
		}
		//衝突相手が自分自身の場合
		if (convexResult.m_hitCollisionObject == this->myself) {
			return btScalar(0.0f);
		}
		ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		return convexResult.m_hitFraction;
	}


	FixContactCallBack::FixContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isLoop(false), ContactResultCallback() {
		this->maxDistance = 0.0f;
		this->fixVec = btVector3(0.0f, 0.0f, 0.0f);
		this->obj = obj;
	}

	//めり込み最大の法線ベクトルを見つけるコールバック
	btScalar FixContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//「最近衝突物との距離が０（誤差含む）」「ループが一定回数に達した」ときループを抜ける
		this->isLoop = false;
		//自分が衝突しないオブジェクトの場合
		if (this->obj->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//相手が衝突しないオブジェクトの場合
		if (colObj1Wrap->getCollisionObject()->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//相手とマスクが合わない場合
		CollisionData* data1 = (CollisionData*)this->obj->getUserPointer();
		CollisionData* data2 = (CollisionData*)colObj1Wrap->getCollisionObject()->getUserPointer();
		if (!(data1->mask & data2->mask)) {
			return btScalar(0.0f);
		}
		//衝突相手が自分自身の場合
		if (this->obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}

		//近すぎる衝突は衝突してないってことにする
		if (cp.getDistance() >= -0.01f) {
			return btScalar(0.0f);
		}
		if (this->count > 5) {
			return btScalar(0.0f);
		}

		//最大値更新
		if (this->maxDistance > cp.getDistance()) {
			this->maxDistance = cp.getDistance();
			this->fixVec = cp.m_normalWorldOnB;
		}
		++this->count;
		this->isLoop = true;
		return btScalar(0.0f);
	}

	CollectCollisionCallBack::CollectCollisionCallBack(std::vector<CollisionTag>& tagList) : result(tagList), isHit(false) {
		tagList.clear();
	}
	btScalar CollectCollisionCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//衝突した相手を記録
		CollisionData* data1 = (CollisionData*)colObj0Wrap->m_collisionObject->getUserPointer();
		CollisionData* data2 = (CollisionData*)colObj1Wrap->m_collisionObject->getUserPointer();
		//相手とマスクが合わない場合
		if (!(data1->mask & data2->mask)) {
			return btScalar(0.0f);
		}
		if (data2) {
			this->result.push_back(data2->tag);
			this->isHit = true;
		}
		return btScalar(0.0f);
	}
}
