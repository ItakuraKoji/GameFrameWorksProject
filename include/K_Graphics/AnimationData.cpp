#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	AnimationData::AnimationData() {
		this->speed = 1;
		this->currentAnimID = 0;
		this->currentAnimTime = 0.0f;
		this->isLoop = false;
		this->isInterpolation = false;
	}
	AnimationData::~AnimationData() {

	}

	void AnimationData::UpdateAnimation() {
		this->currentAnimTime += this->speed * 2.0f;
		if (this->currentAnimTime > this->maxAnimTime - 1) {
			if (this->isLoop) {
				this->currentAnimTime = 0.0f;
			}
			else {
				this->currentAnimTime = (float)this->maxAnimTime - 1.0f;
			}
		}
		this->isInterpolation = false;
	}

	void AnimationData::Add(AnimType &animData) {

		this->animList[animData.animName] = animData;
	}

	void AnimationData::SetSpeed(float speed) {
		this->speed = speed;
	}

	//アニメーションをセット
	void AnimationData::SetAnimation(const std::string& animName, bool playOnce, bool isInterpolation, bool loop) {
		//キーが存在しないなら帰る
		if (this->animList.find(animName) == this->animList.end()) {
			return;
		}

		AnimType anim = this->animList[animName];
		//連続再生（同じアニメーションを再生）を許可していない場合
		if (this->currentAnimID == anim.animID && playOnce) {
			return;
		}
		this->currentAnimTime = 0;
		this->currentAnimID = anim.animID;
		this->maxAnimTime = anim.endTime - anim.startTime;

		this->isLoop = loop;
		this->isInterpolation = isInterpolation;

	}

	float AnimationData::GetCurrentAnimTime() {
		return this->currentAnimTime;
	}

	int AnimationData::GetAnimationID() {
		return this->currentAnimID;
	}

	bool AnimationData::IsStartInterpolation() {
		return this->isInterpolation;
	}

}