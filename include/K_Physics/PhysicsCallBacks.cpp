#include"BulletPhysics.h"

namespace K_Physics {

	////////
	//public
	////

	SweepTestCallBack::SweepTestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
		CollisionData* data = (CollisionData*)this->myself->getUserPointer();
		this->m_collisionFilterGroup = data->GetMyselfMask();
		this->m_collisionFilterMask = data->GetGiveMask();
	}


	//���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
	btScalar SweepTestCallBack::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		//�������Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (this->myself->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//���肪�Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (convexResult.m_hitCollisionObject->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//����ƃ}�X�N������Ȃ��ꍇ
		CollisionData* data1 = (CollisionData*)this->myself->getUserPointer();
		CollisionData* data2 = (CollisionData*)convexResult.m_hitCollisionObject->getUserPointer();
		if (!(data1->GetMyselfMask() & data2->GetGiveMask())) {
			return btScalar(0.0f);
		}
		//�Փˑ��肪�������g�̏ꍇ
		if (convexResult.m_hitCollisionObject == this->myself) {
			return btScalar(0.0f);
		}
		ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		return convexResult.m_hitFraction;
	}

	bool SweepTestCallBack::needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool result = this->m_collisionFilterGroup & proxy0->m_collisionFilterMask;
		return result;
	}


	DetectMaxDistance::DetectMaxDistance(btCollisionObject* obj) : obj(obj), count(0), isLoop(false), ContactResultCallback() {
		this->maxDistance = 0.0f;
		this->fixVec = btVector3(0.0f, 0.0f, 0.0f);
		this->obj = obj;

		CollisionData* data = (CollisionData*)this->obj->getUserPointer();
		this->m_collisionFilterGroup = data->GetMyselfMask();
		this->m_collisionFilterMask = data->GetGiveMask();
	}

	//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
	btScalar DetectMaxDistance::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�u�ŋߏՓ˕��Ƃ̋������O�i�덷�܂ށj�v�u���[�v�����񐔂ɒB�����v�Ƃ����[�v�𔲂���
		this->isLoop = false;
		//�������Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (this->obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//���肪�Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (colObj1Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//����ƃ}�X�N������Ȃ��ꍇ
		CollisionData* data1 = (CollisionData*)this->obj->getUserPointer();
		CollisionData* data2 = (CollisionData*)colObj1Wrap->getCollisionObject()->getUserPointer();
		if (!(data1->GetMyselfMask() & data2->GetGiveMask())) {
			return btScalar(0.0f);
		}
		//�Փˑ��肪�������g�̏ꍇ
		if (this->obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}

		//�߂�����Փ˂͏Փ˂��ĂȂ����Ă��Ƃɂ���
		if (cp.getDistance() >= -0.001f) {
			return btScalar(0.0f);
		}
		if (this->count > 5) {
			return btScalar(0.0f);
		}
		
		//�ő�l�X�V
		if (this->maxDistance > cp.getDistance()) {
			this->maxObject = (btCollisionObject*)colObj1Wrap->getCollisionObject();
			this->maxDistance = cp.getDistance();
			this->fixVec = cp.m_normalWorldOnB;
		}
		++this->count;
		this->isLoop = true;
		return btScalar(0.0f);
	}

	bool DetectMaxDistance::needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool result = this->m_collisionFilterGroup & proxy0->m_collisionFilterMask;
		return result;
	}

	FixContactCallBack::FixContactCallBack(btCollisionObject* obj, const btVector3& limitDirection) : ContactResultCallback() {
		this->obj = obj;
		this->limitDirection = limitDirection;
		this->isHit = false;
		CollisionData* data = (CollisionData*)this->obj->getUserPointer();
		this->m_collisionFilterGroup = data->GetMyselfMask();
		this->m_collisionFilterMask = data->GetGiveMask();
	}

	//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
	btScalar FixContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�������Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (this->obj->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//���肪�Փ˂��Ȃ��I�u�W�F�N�g�̏ꍇ
		if (colObj1Wrap->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		//����ƃ}�X�N������Ȃ��ꍇ
		CollisionData* data1 = (CollisionData*)this->obj->getUserPointer();
		CollisionData* data2 = (CollisionData*)colObj1Wrap->getCollisionObject()->getUserPointer();
		if (!(data1->GetMyselfMask() & data2->GetGiveMask())) {
			return btScalar(0.0f);
		}
		//�Փˑ��肪�������g�̏ꍇ
		if (this->obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}

		//�߂荞�݂��������ꍇ�͏Փ˂��ĂȂ��Ƃ���
		if (cp.getDistance() >= -0.001f) {
			return btScalar(0.0f);
		}
		this->isHit = true;

		//�����������������Ă���ꍇ�͐i�񂾕�������߂�悤�ɂ��������o���Ȃ�
		if (limitDirection.norm() > 0.0f) {
			btVector3& a = obj->getWorldTransform().getOrigin();
			a = a + limitDirection * -cp.getDistance();
		}
		else {
			btVector3& a = obj->getWorldTransform().getOrigin();
			a = a + cp.m_normalWorldOnB * -cp.getDistance();
		}
		
		return btScalar(0.0f);
	}

	bool FixContactCallBack::needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool result = this->m_collisionFilterGroup & proxy0->m_collisionFilterMask;
		return result;
	}

	CollectCollisionCallBack::CollectCollisionCallBack(btCollisionObject* obj, std::vector<CollisionTag*>& tagList) : result(tagList), isHit(false) {
		CollisionData* data = (CollisionData*)obj->getUserPointer();
		this->m_collisionFilterGroup = data->GetMyselfMask();
		this->m_collisionFilterMask = data->GetGiveMask();
		tagList.clear();
	}

	btScalar CollectCollisionCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�Փ˂���������L�^
		CollisionData* data1 = (CollisionData*)colObj0Wrap->m_collisionObject->getUserPointer();
		CollisionData* data2 = (CollisionData*)colObj1Wrap->m_collisionObject->getUserPointer();
		//����ƃ}�X�N������Ȃ��ꍇ
		if (!(data1->GetMyselfMask() & data2->GetGiveMask())) {
			return btScalar(0.0f);
		}
		if (data2) {
			this->result.push_back(&data2->tag);
			this->isHit = true;
		}
		
		return btScalar(0.0f);
	}

	bool CollectCollisionCallBack::needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool result = this->m_collisionFilterGroup & proxy0->m_collisionFilterMask;
		return result;
	}

	MyRaycastCallBack::MyRaycastCallBack(const btVector3& from, const btVector3& to, int myselfMask) :
		ClosestRayResultCallback(from, to)
	{
		this->m_closestHitFraction = 1.0f;
		this->m_collisionFilterGroup = myselfMask;
		this->m_collisionFilterMask = myselfMask;
		this->m_hitNormalWorld = btVector3(0.0f, 0.0f, 0.0f);
	}
	btScalar MyRaycastCallBack::addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) {
		if (this->m_closestHitFraction > rayResult.m_hitFraction) {
			this->m_closestHitFraction = rayResult.m_hitFraction;
		}
		this->m_hitNormalWorld = rayResult.m_hitNormalLocal;
		return rayResult.m_hitFraction;
	}

	bool MyRaycastCallBack::needsCollision(btBroadphaseProxy* proxy0) const
	{
		bool result = this->m_collisionFilterGroup & proxy0->m_collisionFilterMask;
		return result;
	}
}
