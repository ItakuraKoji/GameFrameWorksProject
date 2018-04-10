#include"BulletPhysics.h"

namespace K_Physics {
	////////
	//public
	////
	//ƒRƒŠƒWƒ‡ƒ“‚ðˆµ‚¢‚â‚·‚­‚·‚é‚à‚Ì
	CollisionData::CollisionData(btCollisionObject* obj, CollisionTag tag) : collision(obj), tag(tag) {
	}
	void CollisionData::SetCollisionPosition(const btVector3& position) {
		btTransform trans = this->collision->getWorldTransform();
		trans.setOrigin(position);
		this->collision->setWorldTransform(trans);
	}

	btVector3 CollisionData::GetCollisionPosition() {
		return this->collision->getWorldTransform().getOrigin();
	}
}
