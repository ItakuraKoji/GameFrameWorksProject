#pragma once
#include<string>
#include"ModelData.h"
#include"TextureList.h"
#include"FbxModelLoader.h"

namespace K_Graphics {

	//!@brief ���f���N���X�̏������ɕK�v�ȃp�����[�^�[�̐����S������N���X
	class ModelDataFactory {
	public:
		//!@brief FBX�`����3D���f����ǂݍ���
		//!@param[in] fileName FBX���f���̃t�@�C���p�X
		//!@param[in] textureList �e�N�X�`�����X�g�ւ̃|�C���^�iFBX�Ŏg���Ă���e�N�X�`����ǂݍ��ނ̂Ɏg���j
		//!@return ���f���̃f�[�^�N���X�ւ̃|�C���^
		ModelDatas* LoadFBXModel(const std::string& fileName, TextureList* textureList);
		//!���f���f�[�^�̃��\�[�X�쐬�i�Ԃ��|�C���^�̊J���ӔC������j
		ModelResource* CreateModelResourceFromFBX(const std::string& fileName, TextureList* textureList);
		//!���\�[�X����f�[�^���؂�ă��f�����쐬�i�Ԃ��|�C���^�̊J���ӔC������j
		ModelDatasCopy* CreateModelDataCopy(ModelResource* resource);
		//!@brief �l�p�`�̔|���S�����쐬
		//!@param[in] width �|���S���̕�
		//!@param[in] height �|���S���̍���
		//!@param[in] texture �g�p����e�N�X�`���inullptr�j
		//!@param[in] isCenter �R���g���[���|�C���g�𒆉��ɂ��邩�i�ȗ���true�j\n�X�v���C�g�ł̓R���g���[���|�C���g������ɂ���
		ModelDatas* CreateSquareModel(float width, float height, Texture* texture, bool isCenter = true);
	};

}
