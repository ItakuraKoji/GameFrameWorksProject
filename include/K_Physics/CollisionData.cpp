#include"BulletPhysics.h"

namespace K_Physics {
	////////
	//public
	////
	//コリジョンを扱いやすくするもの
	CollisionData::CollisionData(btCollisionObject* obj, int myselfMask, int giveMask, CollisionTag tag) :
		collision(obj), myselfMask(myselfMask), giveMask(giveMask), tag(tag), active(true)
	{
	}

	void CollisionData::SetCollisionPosition(const K_Math::Vector3& position) {
		btTransform& trans = this->collision->getWorldTransform();
		trans.setOrigin(btVector3(position.x, position.y, position.z));
	}

	void CollisionData::SetCollisionRotation(const K_Math::Vector3& rotation) {
		btTransform& trans = this->collision->getWorldTransform();
		trans.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
	}

	void CollisionData::SetActive(bool active) {
		if (active) {
			//activeフラグが立った時は元に戻す
			this->collision->getBroadphaseHandle()->m_collisionFilterGroup = this->myselfMask;
			this->collision->getBroadphaseHandle()->m_collisionFilterMask = this->giveMask;
		}
		else {
			//activeフラグが下りたときはフィルターをいじって衝突を禁止する
			this->collision->getBroadphaseHandle()->m_collisionFilterGroup = 0;
			this->collision->getBroadphaseHandle()->m_collisionFilterMask = 0;
		}
		this->active = active;
	}

	void CollisionData::SetCollisionTag(const CollisionTag& tag) {
		this->tag = tag;
	}


	K_Math::Vector3 CollisionData::GetCollisionPosition(){
		btVector3& pos = this->collision->getWorldTransform().getOrigin();
		return K_Math::Vector3(pos.x(), pos.y(), pos.z());
	}

	K_Math::Vector3 CollisionData::GetCollisionRotation() {
		btQuaternion rot = this->collision->getWorldTransform().getRotation();
		btScalar x, y, z;
		rot.getEulerZYX(y, x, z);
		return K_Math::Vector3(x, y, z);
	}

	bool CollisionData::IsActive() {
		return this->active;
	}

	btCollisionObject* CollisionData::GetCollision() {
		return this->collision;
	}

	void CollisionData::SetMyselfMask(int mask) {
		this->myselfMask = mask;
		this->collision->getBroadphaseHandle()->m_collisionFilterGroup = this->myselfMask;
	}
	void CollisionData::SetGiveMask(int mask) {
		this->giveMask = mask;
		this->collision->getBroadphaseHandle()->m_collisionFilterMask = this->giveMask;
	}

	int CollisionData::GetMyselfMask() const {
		return this->myselfMask;
	}
	int CollisionData::GetGiveMask() const {
		return this->giveMask;
	}
	CollisionTag* CollisionData::GetCollisionTag() {
		return &this->tag;
	}

	////////
	//public
	////
	RigidBodyData::RigidBodyData(btRigidBody* obj, int myselfMask, int giveMask, CollisionTag tag) :
		CollisionData(obj, myselfMask, giveMask, tag)
	{
	}

	void RigidBodyData::AddForce(const K_Math::Vector3& vector) {
		btRigidBody* rigid = btRigidBody::upcast(this->collision);
		rigid->activate(true);
		rigid->applyForce(btVector3(vector.x, vector.y, vector.z), this->collision->getWorldTransform().getOrigin());
	}

	void RigidBodyData::RigidActivate(bool frag) {
		btRigidBody* rigid = btRigidBody::upcast(this->collision);
		rigid->activate(frag);
	}
}
