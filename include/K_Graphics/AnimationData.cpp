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
		//�z��m��
		this->currentBone.resize(numHierarchy);
		for (int i = 0; i < numHierarchy; ++i) {
			this->currentBone[i].resize(this->bone->GetNumBone(i));
		}
		//�f�t�H���g�̃A�j���[�V�����ɐݒ�
		SetAnimation(0, false, true, 0);


		//blender�ł̎�(X��X Y��Z Z��Y)�ɒ���
		glm::quat rot;
		rot *= glm::angleAxis(K_Math::DegToRad(180.0f), K_Math::Vector3(0.0f, 1.0f, 0.0f));
		rot *= glm::angleAxis(K_Math::DegToRad(-90.0f), K_Math::Vector3(1.0f, 0.0f, 0.0f));
		//���]
		const K_Math::Matrix4x4& scale = glm::scale(K_Math::Matrix4x4(), K_Math::Vector3(-1.0f, 1.0f, 1.0f));

		this->fixMat = scale * glm::toMat4(rot);
	}
	AnimationData::~AnimationData() {

	}

	void AnimationData::UpdateAnimation(float timeSpeed) {
		//�A�j�����ԍX�V
		this->currentAnimTime += this->speed * 2.0f * timeSpeed;
		if (this->currentAnimTime > this->maxAnimTime - 1) {
			if (this->isLoop) {
				this->currentAnimTime = 0.0f;
			}
			else {
				this->currentAnimTime = (float)this->maxAnimTime - 1.0f;
			}
		}
		//��Ԏ��ԍX�V
		this->interporationCount += timeSpeed;
		if (this->interporationCount > this->interporationMaxCount) {
			this->interporationMaxCount = 0.0f;
			this->interporationCount = 0.0f;
		}

		//�s��X�V
		int numArray = (int)this->currentBone.size();
		for (int i = 0; i < numArray; ++i) {
			int numBone = (int)this->currentBone[i].size();
			for (int k = 0; k < numBone; ++k) {
				this->currentBone[i][k].currentMat = this->bone->GetCurrentBoneMatrix(i, k, this->currentAnimID, (int)this->currentAnimTime);
				this->currentBone[i][k].currentMat = this->currentBone[i][k].currentMat;
				//��Ԓ��̂ݕ��
				if (this->interporationMaxCount > 0.0f) {
					BoneInterporation(i, k, this->interporationCount / this->interporationMaxCount);
				}
			}
		}
	}



	void AnimationData::SetSpeed(float speed) {
		this->speed = speed;
	}

	//�A�j���[�V�������Z�b�g
	void AnimationData::SetAnimation(const std::string& animName, bool playOnce, bool loop, int interpolationCount) {
		AnimType anim = this->bone->GetAnimData(animName);
		SetCurrentAnimation(anim, playOnce, loop, interpolationCount);
	}
	void AnimationData::SetAnimation(int animIndex, bool playOnce, bool loop, int interpolationCount) {
		AnimType anim = this->bone->GetAnimData(animIndex);
		SetCurrentAnimation(anim, playOnce, loop, interpolationCount);
	}

	void AnimationData::SetMatrixTextureData(int arrayIndex, Texture *texture) {
		//�{�[�����v�Z���A�e�N�X�`���ɍs�����[�߂�
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
		//�A���Đ��i�����A�j���[�V�������Đ��j�������Ă��Ȃ��ꍇ
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
			//��ԊJ�n����ꍇ�͕�Ԃ��n�߂�
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


		//���s�ړ����o��
		const K_Math::Vector3& translationA = bone.interPolationMat[3];
		const K_Math::Vector3& translationB = bone.currentMat[3];

		//��]�s�񁕊g�k�s��
		K_Math::Matrix3x3& rotScaleA = (K_Math::Matrix3x3)bone.interPolationMat;
		K_Math::Matrix3x3& rotScaleB = (K_Math::Matrix3x3)bone.currentMat;

		//�g�k�̓x�N�g���̒����ŕ\�����
		K_Math::Vector3 scaleA;
		scaleA.x = glm::length(K_Math::Vector3(rotScaleA[0][0], rotScaleA[1][0], rotScaleA[2][0]));
		scaleA.y = glm::length(K_Math::Vector3(rotScaleA[0][1], rotScaleA[1][1], rotScaleA[2][1]));
		scaleA.z = glm::length(K_Math::Vector3(rotScaleA[0][2], rotScaleA[1][2], rotScaleA[2][2]));
		K_Math::Vector3 scaleB;
		scaleB.x = glm::length(K_Math::Vector3(rotScaleB[0][0], rotScaleB[1][0], rotScaleB[2][0]));
		scaleB.y = glm::length(K_Math::Vector3(rotScaleB[0][1], rotScaleB[1][1], rotScaleB[2][1]));
		scaleB.z = glm::length(K_Math::Vector3(rotScaleB[0][2], rotScaleB[1][2], rotScaleB[2][2]));

		//�X�P�[���ŉ�]�s�������Ɖ�]�s��݂̂ɂȂ�
		for (int y = 0; y < 3; ++y) {
			for (int x = 0; x < 3; ++x) {
				rotScaleA[x][y] /= scaleA[y];
				rotScaleB[x][y] /= scaleB[y];
			}
		}

		//������N�H�[�^�j�I���֕ϊ�
		K_Math::Quaternion rotationA(rotScaleA);
		K_Math::Quaternion rotationB(rotScaleB);

		//�����ĕ��
		const K_Math::Vector3& resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
		const K_Math::Vector3& resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
		const K_Math::Quaternion& resultRot = glm::slerp(rotationA, rotationB, ratio);

		//SRT�s��쐬
		K_Math::Matrix4x4 resultMat;
		resultMat[0][0] = resultScale.x;
		resultMat[1][1] = resultScale.y;
		resultMat[2][2] = resultScale.z;

		resultMat *= glm::toMat4(resultRot);
		resultMat[3] = K_Math::Vector4(resultTrans, 1.0f);

		//�i�[
		bone.currentMat = resultMat;
	}


	void AnimationData::CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int arrayIndex, int boneIndex) {
		const K_Math::Matrix4x4& bind = this->bone->GetBindBoneMatrix(arrayIndex, boneIndex);
		const K_Math::Matrix4x4& current = this->currentBone[arrayIndex][boneIndex].currentMat;

		resultMat = this->fixMat * current * bind;
	}
}