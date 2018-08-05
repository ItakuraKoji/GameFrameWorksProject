#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	BoneData::BoneData() {
		this->animNameList.reserve(16);
	}
	BoneData::~BoneData() {

	}

	void BoneData::AddBoneData(const std::vector<Bone> &boneData) {
		this->boneData.push_back(boneData);
	}
	void BoneData::AddAnimData(const AnimType &animData) {
		//文字キーとデータが対応、文字キーも配列に記録
		this->animList[animData.animName] = animData;
		this->animNameList.push_back(animData.animName);
	}

	const K_Math::Matrix4x4& BoneData::GetBindBoneMatrix(int hierarchyIndex, int boneIndex) const {
		return this->boneData[hierarchyIndex][boneIndex].bindMat;
	}
	const K_Math::Matrix4x4& BoneData::GetCurrentBoneMatrix(int hierarchyIndex, int boneIndex, int animID, int time) const {
		return this->boneData[hierarchyIndex][boneIndex].mat[animID][time];
	}

	int BoneData::GetNumBone(int hierarchyIndex) const {
		return (int)this->boneData[hierarchyIndex].size();
	}

	const AnimType& BoneData::GetAnimData(const std::string& animName) const {
		//キーが存在しないなら例外
		if (this->animList.find(animName) == this->animList.end()) {
			throw std::runtime_error("not found animation name : " + animName);
		}
		return this->animList.at(animName);
	}
	const AnimType& BoneData::GetAnimData(int animIndex) const {
		const std::string& animName = this->animNameList.at(animIndex);
		//キーが存在しないなら例外
		if (this->animList.find(animName) == this->animList.end()) {
			throw std::runtime_error("not found animation name : " + animName + "(Index:" + std::to_string(animIndex) + ")");
		}
		return this->animList.at(animName);
	}

}