#include"BulletPhysics.h"

namespace K_Physics {
	////////
	//public
	////
	//ƒRƒŠƒWƒ‡ƒ“‚ðˆµ‚¢‚â‚·‚­‚·‚é‚à‚Ì
	CollisionData::CollisionData(btCollisionObject* obj, CollisionTag tag) : collision(obj), tag(tag) {
	}
	void CollisionData::SetCollisionPosition(const K_Math::Vector3& position) {
		btTransform trans = this->collision->getWorldTransform();
		trans.setOrigin(btVector3(position.x(), position.y(), position.z()));
		this->collision->setWorldTransform(trans);
	}

	K_Math::Vector3 CollisionData::GetCollisionPosition(){
		btVector3 pos = this->collision->getWorldTransform().getOrigin();
		return K_Math::Vector3(pos.x(), pos.y(), pos.z());
	}
}
