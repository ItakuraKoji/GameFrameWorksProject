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
		if (this->currentAnimTime > this->maxAnimTime) {
			if (this->isLoop) {
				this->currentAnimTime = 0.0f;
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
	void AnimationData::SetAnimation(const std::string& animName, FbxScene *fbxScene, bool playOnce, bool isInterpolation, bool loop) {
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
		FbxAnimStack *pStack = fbxScene->GetSrcObject<FbxAnimStack>(this->currentAnimID);
		fbxScene->SetCurrentAnimationStack(pStack);

		this->isLoop = loop;
		this->isInterpolation = isInterpolation;
	}

	float AnimationData::GetCurrentAnimTime() {
		return this->currentAnimTime;
	}

	bool AnimationData::IsStartInterpolation() {
		return this->isInterpolation;
	}

}