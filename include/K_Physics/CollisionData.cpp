#include"BulletPhysics.h"

namespace K_Physics {
	////////
	//public
	////
	//ƒRƒŠƒWƒ‡ƒ“‚ðˆµ‚¢‚â‚·‚­‚·‚é‚à‚Ì
	CollisionData::CollisionData(btCollisionObject* obj, int myselfMask, int giveMask, CollisionTag tag) : collision(obj), myselfMask(myselfMask), giveMask(giveMask), tag(tag) {
	}

	void CollisionData::SetCollisionPosition(const K_Math::Vector3& position) {
		btTransform& trans = this->collision->getWorldTransform();
		trans.setOrigin(btVector3(position.x(), position.y(), position.z()));
	}

	K_Math::Vector3 CollisionData::GetCollisionPosition(){
		btVector3& pos = this->collision->getWorldTransform().getOrigin();
		return K_Math::Vector3(pos.x(), pos.y(), pos.z());
	}

	btCollisionObject* CollisionData::GetCollision() {
		return this->collision;
	}

	void CollisionData::SetMyselfMask(int mask) {
		this->myselfMask = mask;
	}
	void CollisionData::SetGiveMask(int mask) {
		this->giveMask = mask;
	}

	int CollisionData::GetMyselfMask() {
		return this->myselfMask;
	}
	int CollisionData::GetGiveMask() {
		return this->giveMask;
	}

	RigidBodyData::RigidBodyData(btRigidBody* obj, int myselfMask, int giveMask, CollisionTag tag) : CollisionData(obj, myselfMask, giveMask, tag) {
	}

	void RigidBodyData::AddForce(const K_Math::Vector3& vector) {
		btRigidBody* rigid = btRigidBody::upcast(this->collision);
		rigid->activate(true);
		rigid->applyForce(btVector3(vector.x(), vector.y(), vector.z()), this->collision->getWorldTransform().getOrigin());
	}

	void RigidBodyData::Activate(bool frag) {
		btRigidBody* rigid = btRigidBody::upcast(this->collision);
		rigid->activate(frag);
	}
}
