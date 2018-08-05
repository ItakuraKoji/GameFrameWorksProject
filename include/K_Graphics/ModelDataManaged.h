#pragma once
#include"K_Math\MyMathFanctions.h"

#include<string>
#include<vector>
#include<unordered_map>
#include<fbxsdk\fbxsdk.h>
#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	//�ω��̂���i���b�V�����f���̃C���X�^���X���ƂɈႤ�j�A�j���[�V�����f�[�^
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
		//�؂蕨�̃{�[���f�[�^
		//�K�w�ƃ{�[�����ɂ��z��
		std::vector<std::vector<BoneAnimation>> boneData;
		float interporationCount;
		float interporationMaxCount;
		//���݂̃A�j���[�V�������
		int   currentAnimID;
		float currentAnimTime;
		int   maxAnimTime;
		bool  isLoop;
		bool  isInterpolation;
	};

}