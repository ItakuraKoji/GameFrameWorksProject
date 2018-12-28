#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	AnimationData::AnimationData(BoneData* bone, int numHierarchy) {
		this->speed = 1;
		this->currentAnimID = 0;
		this->currentAnimTime = 0.0f;
		this->interporationCount = 0.0f;
		this->isLoop = false;
		this->bone = bone;
		//配列確保
		this->currentBone.resize(numHierarchy);
		for (int i = 0; i < numHierarchy; ++i) {
			this->currentBone[i].resize(this->bone->GetNumBone(i));
		}
		//デフォルトのアニメーションに設定
		SetAnimation(0, false, true, 0);


		//blenderでの軸(X→X Y→Z Z→Y)に直す
		glm::quat rot;
		rot *= glm::angleAxis(K_Math::DegToRad(180.0f), K_Math::Vector3(0.0f, 1.0f, 0.0f));
		rot *= glm::angleAxis(K_Math::DegToRad(-90.0f), K_Math::Vector3(1.0f, 0.0f, 0.0f));
		//反転
		const K_Math::Matrix4x4& scale = glm::scale(K_Math::Matrix4x4(), K_Math::Vector3(-1.0f, 1.0f, 1.0f));

		this->fixMat = scale * glm::toMat4(rot);
	}
	AnimationData::~AnimationData() {

	}

	void AnimationData::UpdateAnimation(float timeSpeed) {
		//アニメ時間更新
		this->currentAnimTime += this->speed * 2.0f * timeSpeed;
		if (this->currentAnimTime > this->maxAnimTime - 1) {
			if (this->isLoop) {
				this->currentAnimTime = 0.0f;
			}
			else {
				this->currentAnimTime = (float)this->maxAnimTime - 1.0f;
			}
		}
		//補間時間更新
		this->interporationCount += timeSpeed;
		if (this->interporationCount > this->interporationMaxCount) {
			this->interporationMaxCount = 0.0f;
			this->interporationCount = 0.0f;
		}

		//行列更新
		int numArray = (int)this->currentBone.size();
		for (int i = 0; i < numArray; ++i) {
			int numBone = (int)this->currentBone[i].size();
			for (int k = 0; k < numBone; ++k) {
				this->currentBone[i][k].currentMat = this->bone->GetCurrentBoneMatrix(i, k, this->currentAnimID, (int)this->currentAnimTime);
				this->currentBone[i][k].currentMat = this->currentBone[i][k].currentMat;
				//補間中のみ補間
				if (this->interporationMaxCount > 0.0f) {
					BoneInterporation(i, k, this->interporationCount / this->interporationMaxCount);
				}
			}
		}
	}



	void AnimationData::SetSpeed(float speed) {
		this->speed = speed;
	}

	//アニメーションをセット
	void AnimationData::SetAnimation(const std::string& animName, bool playOnce, bool loop, int interpolationCount) {
		AnimType anim = this->bone->GetAnimData(animName);
		SetCurrentAnimation(anim, playOnce, loop, interpolationCount);
	}
	void AnimationData::SetAnimation(int animIndex, bool playOnce, bool loop, int interpolationCount) {
		AnimType anim = this->bone->GetAnimData(animIndex);
		SetCurrentAnimation(anim, playOnce, loop, interpolationCount);
	}

	void AnimationData::SetMatrixTextureData(int arrayIndex, Texture *texture) {
		//ボーンを計算し、テクスチャに行列情報を納める
		int numBone = (int)this->currentBone[arrayIndex].size();
		K_Math::Matrix4x4 mat[120];
		for (int i = 0; i < numBone; ++i) {
			CalculateBoneMatrix(mat[i], arrayIndex, i);
		}
		texture->SetImageData(&mat[0][0], numBone * 4, 1, TextureType::Float, TextureColorType::RGBA32F, TextureColorType::RGBA);
	}

	float AnimationData::GetCurrentAnimTime() {
		return this->currentAnimTime;
	}

	int AnimationData::GetAnimationID() {
		return this->currentAnimID;
	}
	int AnimationData::GetNumBone(int hirarchyIndex) const {
		return (int)this->currentBone[hirarchyIndex].size();
	}


	////////
	//private
	////
	void AnimationData::SetCurrentAnimation(const AnimType& anim, bool playOnce, bool loop, int interpolationCount) {
		//連続再生（同じアニメーションを再生）を許可していない場合
		if (this->currentAnimID == anim.animID && playOnce) {
			return;
		}
		this->currentAnimTime = 0;
		this->currentAnimID = anim.animID;
		this->maxAnimTime = anim.endTime - anim.startTime;

		this->isLoop = loop;

		this->interporationCount = 0.0f;
		this->interporationMaxCount = (float)interpolationCount;
		if (interpolationCount > 0) {
			//補間開始する場合は補間を始める
			for (int i = 0; i < this->currentBone.size(); ++i) {
				for (int j = 0; j < this->currentBone[i].size(); ++j) {
					this->currentBone[i][j].interPolationMat = this->currentBone[i][j].currentMat;
				}
			}
		}

		UpdateAnimation(0.0f);

	}

	void AnimationData::BoneInterporation(int arrayIndex, int boneIndex, float ratio) {
		BoneAnimation& bone = this->currentBone[arrayIndex][boneIndex];


		//平行移動取り出し
		const K_Math::Vector3& translationA = bone.interPolationMat[3];
		const K_Math::Vector3& translationB = bone.currentMat[3];

		//回転行列＆拡縮行列
		K_Math::Matrix3x3& rotScaleA = (K_Math::Matrix3x3)bone.interPolationMat;
		K_Math::Matrix3x3& rotScaleB = (K_Math::Matrix3x3)bone.currentMat;

		//拡縮はベクトルの長さで表される
		K_Math::Vector3 scaleA;
		scaleA.x = glm::length(K_Math::Vector3(rotScaleA[0][0], rotScaleA[1][0], rotScaleA[2][0]));
		scaleA.y = glm::length(K_Math::Vector3(rotScaleA[0][1], rotScaleA[1][1], rotScaleA[2][1]));
		scaleA.z = glm::length(K_Math::Vector3(rotScaleA[0][2], rotScaleA[1][2], rotScaleA[2][2]));
		K_Math::Vector3 scaleB;
		scaleB.x = glm::length(K_Math::Vector3(rotScaleB[0][0], rotScaleB[1][0], rotScaleB[2][0]));
		scaleB.y = glm::length(K_Math::Vector3(rotScaleB[0][1], rotScaleB[1][1], rotScaleB[2][1]));
		scaleB.z = glm::length(K_Math::Vector3(rotScaleB[0][2], rotScaleB[1][2], rotScaleB[2][2]));

		//スケールで回転行列を割ると回転行列のみになる
		for (int y = 0; y < 3; ++y) {
			for (int x = 0; x < 3; ++x) {
				rotScaleA[x][y] /= scaleA[y];
				rotScaleB[x][y] /= scaleB[y];
			}
		}

		//それをクォータニオンへ変換
		K_Math::Quaternion rotationA(rotScaleA);
		K_Math::Quaternion rotationB(rotScaleB);

		//そして補間
		const K_Math::Vector3& resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
		const K_Math::Vector3& resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
		const K_Math::Quaternion& resultRot = glm::slerp(rotationA, rotationB, ratio);

		//SRT行列作成
		K_Math::Matrix4x4 resultMat;
		resultMat[0][0] = resultScale.x;
		resultMat[1][1] = resultScale.y;
		resultMat[2][2] = resultScale.z;

		resultMat *= glm::toMat4(resultRot);
		resultMat[3] = K_Math::Vector4(resultTrans, 1.0f);

		//格納
		bone.currentMat = resultMat;
	}


	void AnimationData::CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int arrayIndex, int boneIndex) {
		const K_Math::Matrix4x4& bind = this->bone->GetBindBoneMatrix(arrayIndex, boneIndex);
		const K_Math::Matrix4x4& current = this->currentBone[arrayIndex][boneIndex].currentMat;

		resultMat = this->fixMat * current * bind;
	}
}