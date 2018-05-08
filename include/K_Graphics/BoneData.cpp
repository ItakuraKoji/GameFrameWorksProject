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
		SetClurrentBoneData((int)this->boneData.size() - 1, 0);
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

		texture->SetImageData(mat->data(), numBone * 4, 1, TextureType::Float, TextureColorType::RGBA32F, TextureColorType::RGBA);
	}

	void BoneData::SetClurrentBoneData(int arrayIndex, int time) {
		//60フレームのアニメーションを120として半分の速度での再生を可能にする
		FbxTime fbxTime;
		fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);


		int numBone = (int)this->boneData[arrayIndex].size();
		for (int k = 0; k < numBone; ++k) {
			this->boneData[arrayIndex][k].cluster;
			FbxAMatrix& mat = this->boneData[arrayIndex][k].cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					this->boneData[arrayIndex][k].currentMat(x, y) = (float)mat.Get(y, x);
				}
			}
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
		const K_Math::Vector3& translationA = this->boneData[arrayIndex][boneIndex].interPolationMat.block(0, 3, 3, 1);
		const K_Math::Vector3& translationB = this->boneData[arrayIndex][boneIndex].currentMat.block(0, 3, 3, 1);

		//回転行列＆拡縮行列
		K_Math::Matrix3x3 rotScaleA = this->boneData[arrayIndex][boneIndex].interPolationMat.block(0, 0, 3, 3);
		K_Math::Matrix3x3 rotScaleB = this->boneData[arrayIndex][boneIndex].currentMat.block(0, 0, 3, 3);

		//拡縮はベクトルの長さで表される
		K_Math::Vector3 scaleA;
		scaleA.x() = rotScaleA.block(0, 0, 1, 3).norm();
		scaleA.y() = rotScaleA.block(1, 0, 1, 3).norm();
		scaleA.z() = rotScaleA.block(2, 0, 1, 3).norm();
		K_Math::Vector3 scaleB;
		scaleB.x() = rotScaleB.block(0, 0, 1, 3).norm();
		scaleB.y() = rotScaleB.block(1, 0, 1, 3).norm();
		scaleB.z() = rotScaleB.block(2, 0, 1, 3).norm();

		//スケールで回転行列を割ると回転行列のみになる
		rotScaleA.block(0, 0, 1, 3) = rotScaleA.block(0, 0, 1, 3) / scaleA.x();
		rotScaleA.block(1, 0, 1, 3) = rotScaleA.block(1, 0, 1, 3) / scaleA.y();
		rotScaleA.block(2, 0, 1, 3) = rotScaleA.block(2, 0, 1, 3) / scaleA.z();
		rotScaleB.block(0, 0, 1, 3) = rotScaleB.block(0, 0, 1, 3) / scaleB.x();
		rotScaleB.block(1, 0, 1, 3) = rotScaleB.block(1, 0, 1, 3) / scaleB.y();
		rotScaleB.block(2, 0, 1, 3) = rotScaleB.block(2, 0, 1, 3) / scaleB.z();

		//それをクォータニオンへ変換
		K_Math::Quaternion rotationA(rotScaleA);
		K_Math::Quaternion rotationB(rotScaleB);

		const K_Math::Vector3& resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
		const K_Math::Vector3& resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
		const K_Math::Quaternion& resultRot = rotationA.slerp(ratio, rotationB);

		//SRT行列作成
		K_Math::Matrix4x4 resultMat = K_Math::Matrix4x4::Identity();
		resultMat(0, 0) = resultScale.x();
		resultMat(1, 1) = resultScale.y();
		resultMat(2, 2) = resultScale.z();
		K_Math::Matrix4x4 rotMat = K_Math::Matrix4x4::Identity();
		rotMat.block(0, 0, 3, 3) = resultRot.matrix();

		resultMat = resultMat * rotMat;
		resultMat.block(0, 3, 3, 1) = resultTrans;

		//格納
		this->boneData[arrayIndex][boneIndex].currentMat = resultMat;
	}

	void BoneData::CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int arrayIndex, int boneIndex) {
		const K_Math::Matrix4x4& bind = this->boneData[arrayIndex][boneIndex].bindMat.inverse();
		const K_Math::Matrix4x4& current = this->boneData[arrayIndex][boneIndex].currentMat;
		resultMat = current * bind;
	}

}