#pragma once
#include"K_Math\MyMathFanctions.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<fbxsdk\fbxsdk.h>
#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	//変化のある（メッシュモデルのインスタンスごとに違う）アニメーションデータ
	struct BoneAnimation {
		K_Math::Matrix4x4 currentMat;
		K_Math::Matrix4x4 interPolationMat;
	};

	class AnimationStatus {
	public:
		AnimationStatus();
		~AnimationStatus();
		void SetClurrentBoneData(int hierarchyIndex);
		void StartInterporation(int frameCount);
		int GetCurrentMat(int hierarchyIndex);
		int GetInterPolationMat(int hierarchyIndex);
		int GetNumBone(int hierarchyIndex);

	private:
		void UpdateInterporation();
		void CalculateBoneMatrix(K_Math::Matrix4x4& resultMat, int hierarchyIndex, int boneIndex);
		void BoneInterporation(int hierarchyIndex, int boneIndex, float ratio);

	private:
		//借り物のボーンデータ
		//階層とボーン数による配列
		std::vector<std::vector<BoneAnimation>> boneData;
		float interporationCount;
		float interporationMaxCount;
		//現在のアニメーション情報
		int   currentAnimID;
		float currentAnimTime;
		int   maxAnimTime;
		bool  isLoop;
		bool  isInterpolation;
	};

}