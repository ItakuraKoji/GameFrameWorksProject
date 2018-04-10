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
		++this->interporationCount;
		if (this->interporationCount >= this->interporationMaxCount) {
			this->interporationMaxCount = 0.0f;
		}

		int numBone = (int)this->boneData[arrayIndex].size();
		float mat[16 * 120] = {};

		for (int i = 0; i < numBone; ++i) {
			int matIndex = i * 16;
			//行列補間
			if (this->interporationMaxCount) {
				BoneInterporation(arrayIndex, i, this->interporationCount / this->interporationMaxCount);
			}

			K_Math::Matrix4x4 boneMat = CalculateBoneMatrix(arrayIndex, i);
			for (int k = 0; k < 16; ++k) {
				mat[matIndex + k] = boneMat(k);
			}
		}

		GLuint tex = texture->GetTextureID();
		texture->SetImageData(mat, numBone * 4, 1, TextureType::Float, TextureColorType::RGBA32F, TextureColorType::RGBA);
	}

	void BoneData::SetClurrentBoneData(int arrayIndex, int time) {
		FbxTime fbxTime;
		//60フレームのアニメーションを120として半分の速度での再生を可能にした
		fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);


		int numBone = (int)this->boneData[arrayIndex].size();
		for (int k = 0; k < numBone; ++k) {
			FbxAMatrix mat;
			K_Math::Matrix4x4 currentBone;
			mat = this->boneData[arrayIndex][k].cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					currentBone(x, y) = (float)mat.Get(y, x);
				}
			}
			this->boneData[arrayIndex][k].currentMat = currentBone;
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

	int BoneData::GetNumBone(int arrayIndex) {
		return (int)this->boneData[arrayIndex].size();
	}



	////////
	//private
	////
	void BoneData::BoneInterporation(int arrayIndex, int boneIndex, float ratio) {
		//平行移動取り出し
		K_Math::Vector3 translationA = this->boneData[arrayIndex][boneIndex].interPolationMat.block(0, 3, 3, 1);
		K_Math::Vector3 translationB = this->boneData[arrayIndex][boneIndex].currentMat.block(0, 3, 3, 1);

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

		K_Math::Vector3 resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
		K_Math::Vector3 resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
		K_Math::Quaternion resultRot = rotationA.slerp(ratio, rotationB);

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

	K_Math::Matrix4x4 BoneData::CalculateBoneMatrix(int arrayIndex, int boneIndex) {
		K_Math::Matrix4x4 bind = this->boneData[arrayIndex][boneIndex].bindMat.inverse();
		K_Math::Matrix4x4 current = this->boneData[arrayIndex][boneIndex].currentMat;
		//blenderの出力ではボーンは軸情報に従わないので補正(180度Y軸回転　→　90度X軸回転)
		K_Math::Matrix3x3 rot = (K_Math::AngleAxis(K_Math::DegToRad(90), K_Math::Vector3(1.0f, 0.0f, 0.0f)) * K_Math::AngleAxis(K_Math::DegToRad(180), K_Math::Vector3(0.0f, 1.0f, 0.0f))).matrix();
		K_Math::Matrix4x4 mat = K_Math::Matrix4x4::Identity();
		mat.block(0, 0, 3, 3) = rot;

		return current * bind * mat;
	}

}