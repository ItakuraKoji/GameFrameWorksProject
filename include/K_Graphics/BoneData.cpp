#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	BoneData::BoneData() {
		this->interporationCount = 0.0f;
		this->interporationMaxCount = 0.0f;
	}
	BoneData::~BoneData() {

	}

	void BoneData::Add(std::vector<Bone> &boneData) {
		this->boneData.push_back(boneData);
		//SetClurrentBoneData((int)this->boneData.size() - 1, 0);
	}

	void BoneData::SetMatrixTextureData(int arrayIndex, Texture *texture) {
		int numBone = (int)this->boneData[arrayIndex].size();
		K_Math::Matrix4x4 mat[120];

		for (int i = 0; i < numBone; ++i) {
			int matIndex = i * 16;
			//行列補間
			if (this->interporationMaxCount) {
				BoneInterporation(arrayIndex, i, this->interporationCount / this->interporationMaxCount);
			}
			CalculateBoneMatrix(mat[i], arrayIndex, i);
		}

		texture->SetImageData(&mat[0][0], numBone * 4, 1, TextureType::Float, TextureColorType::RGBA32F, TextureColorType::RGBA);
	}

	void BoneData::SetClurrentBoneData(int arrayIndex, int animID, int time) {
		//60フレームのアニメーションを120として半分の速度での再生を可能にする
		FbxTime fbxTime;
		fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);

		int numBone = (int)this->boneData[arrayIndex].size();
		for (int k = 0; k < numBone; ++k) {
			this->boneData[arrayIndex][k].currentMat = this->boneData[arrayIndex][k].mat[animID][time];

		}
	}

	void BoneData::StartInterporation(int frameCount) {
		for (int i = 0; i < this->boneData.size(); ++i) {
			for (int j = 0; j < this->boneData[i].size(); ++j) {
				this->boneData[i][j].interPolationMat = this->boneData[i][j].currentMat;
			}
		}
		this->interporationCount = 0.0f;
		this->interporationMaxCount = (float)frameCount;
	}

	void BoneData::UpdateInterporation() {
		++this->interporationCount;
		if (this->interporationCount >= this->interporationMaxCount) {
			this->interporationMaxCount = 0.0f;
		}
	}

	int BoneData::GetNumBone(int arrayIndex) {
		return (int)this->boneData[arrayIndex].size();
	}



	////////
	//private
	////
	void BoneData::BoneInterporation(int arrayIndex, int boneIndex, float ratio) {
		//平行移動取り出し
		const K_Math::Vector3& translationA = this->boneData[arrayIndex][boneIndex].interPolationMat[3];
		const K_Math::Vector3& translationB = this->boneData[arrayIndex][boneIndex].currentMat[3];

		//回転行列＆拡縮行列
		K_Math::Matrix3x3 rotScaleA;
		rotScaleA[0] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].interPolationMat[0];
		rotScaleA[1] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].interPolationMat[1];
		rotScaleA[2] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].interPolationMat[2];
		K_Math::Matrix3x3 rotScaleB;
		rotScaleB[0] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].currentMat[0];
		rotScaleB[1] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].currentMat[1];
		rotScaleB[2] = (K_Math::Vector3)this->boneData[arrayIndex][boneIndex].currentMat[2];

		//拡縮はベクトルの長さで表される
		K_Math::Vector3 scaleA;
		scaleA.x = glm::length(rotScaleA[0]);
		scaleA.y = glm::length(rotScaleA[1]);
		scaleA.z = glm::length(rotScaleA[2]);
		K_Math::Vector3 scaleB;
		scaleB.x = glm::length(rotScaleB[0]);
		scaleB.y = glm::length(rotScaleB[1]);
		scaleB.z = glm::length(rotScaleB[2]);

		//スケールで回転行列を割ると回転行列のみになる
		rotScaleA[0] = rotScaleA[0] / scaleA.x;
		rotScaleA[1] = rotScaleA[1] / scaleA.y;
		rotScaleA[2] = rotScaleA[2] / scaleA.z;
		rotScaleB[0] = rotScaleB[0] / scaleB.x;
		rotScaleB[1] = rotScaleB[1] / scaleB.y;
		rotScaleB[2] = rotScaleB[2] / scaleB.z;

		//それをクォータニオンへ変換
		K_Math::Quaternion rotationA(rotScaleA);
		K_Math::Quaternion rotationB(rotScaleB);

		const K_Math::Vector3& resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
		const K_Math::Vector3& resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
		const K_Math::Quaternion& resultRot = glm::slerp(rotationA, rotationB, ratio);

		//SRT行列作成
		K_Math::Matrix4x4 resultMat;
		resultMat[0][0] = resultScale.x;
		resultMat[1][1] = resultScale.y;
		resultMat[2][2] = resultScale.z;
		K_Math::Matrix4x4 rotMat;
		rotMat = glm::toMat4(resultRot);

		resultMat = resultMat * rotMat;
		resultMat[3] = K_Math::Vector4(resultTrans, 1.0f);

		//格納
		this->boneData[arrayIndex][boneIndex].currentMat = resultMat;
	}

	void BoneData::CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int arrayIndex, int boneIndex) {
		const K_Math::Matrix4x4& bind = glm::inverse(this->boneData[arrayIndex][boneIndex].bindMat);
		const K_Math::Matrix4x4& current = this->boneData[arrayIndex][boneIndex].currentMat;
		resultMat = current * bind;
	}

}