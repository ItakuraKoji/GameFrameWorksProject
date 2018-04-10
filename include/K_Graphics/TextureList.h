#pragma once

#include<unordered_map>
#include"Texture.h"

namespace K_Graphics {
	///@brief �e�N�X�`���N���X�����X�g�Ƃ��Ď����A����ӔC�����N���X
	class TextureList {
	public:
		///@brief Initialize()���Ă�
		TextureList();
		///@brief Initialize()���Ă�
		~TextureList();

		///@brief ���X�g�̃e�N�X�`����S�ĊJ������
		void Initialize();
		///@brief �e�N�X�`�����擾
		///@param[in] textureName �e�N�X�`���̖��O
		///@return ���̖��O�̃e�N�X�`���ւ̃|�C���^�A����������nullptr
		Texture* GetTexture(const std::string& textureName);
		///@brief �e�N�X�`���ǂݍ���
		///@param[in] textureName �쐬����e�N�X�`���̃��[�U�[��`��
		///@param[in] filePass �摜�̃t�@�C���p�X
		///@return ���������true
		bool LoadTexture(const std::string& textureName, const std::string& filePass);
		///@brief ��̃e�N�X�`�����쐬
		///@param[in] textureName �쐬����e�N�X�`���̃��[�U�[��`��
		///@param[in] textureWidth �e�N�X�`���̉摜��
		///@param[in] textureHeight �e�N�X�`���̉摜����
		///@return ���������true
		bool AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight);
		///@brief �e�N�X�`�����폜
		///@param[in] textureName �폜����e�N�X�`���̖��O
		void DeleteTexture(const std::string& textureName);

	private:
		std::unordered_map<std::string, Texture*> textureList;

	};
}
