#pragma once
#include<string>
#include"ModelData.h"
#include"TextureList.h"
#include"FbxModelLoader.h"

namespace K_Graphics {

	//���f���N���X�̏������ɕK�v�ȃp�����[�^�[�̐����S������N���X
	//�E�O���t�@�C���̓ǂݍ���
	//�E��{�}�`(�|���S���Ȃ�)
	class ModelDataFactory {
	public:
		ModelDatas* LoadFBXModel(const std::string& fileName, TextureList* textureList);
		ModelDatas* CreateSquareModel(float width, float height, Texture* texture, bool isCenter = true);
	};

}
