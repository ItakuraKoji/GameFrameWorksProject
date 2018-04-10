#pragma once
#include<GLEW\glew.h>
#include<string>
#include"ImageLoader.h"

namespace K_Graphics {
	///@brief �e�N�X�`���������l�̌^
	enum TextureType {
		Unsigned_Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
	};
	///@brief �e�N�X�`���ɓn�������̉���
	enum TextureColorType {
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA32F = GL_RGBA32F,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RED = GL_RED,
	};

	///@brief �O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ�e�N�X�`����ێ�����\n���̃N���X�́A�ێ����Ă���e�N�X�`���̉���ӔC������
	class Texture {
	public:
		///@brief Initialize()���Ă�
		Texture();
		///@brief �e�N�X�`�����J������
		~Texture();
		///@brief ��̃f�[�^�Ńe�N�X�`����������
		bool Initialize();
		///@brief �摜��ǂݍ���Ńe�N�X�`���ɓn��
		///@param fileName �摜�̃t�@�C���p�X
		///@return ���������true
		bool LoadImage(const std::string& fileName);
		///@brief �z��f�[�^���摜�Ƃ��ēn��
		///@param[in] data �f�[�^�̓������z��
		///@param[in] width �摜��
		///@param[in] height �摜����
		///@param[in] bType �e�N�X�`���̐��l�^�̐ݒ�
		///@param[in] textureColor �e�N�X�`���̐F�̌`��
		///@param[in] dataColor �e�N�X�`�������p����摜�̐F�̌`��
		void SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor);
		///@brief �摜�̊g�k���Ƀt�B���^�[�������邩�ݒ�
		///@param[in] isFiltering �t�B���^�����O�����邩�̃t���O
		void SetFilter(bool isFiltering);
		///@return �V�F�[�_�[�ɓn���ۂ̃e�N�X�`���C���f�b�N�X
		GLuint GetTextureID();
		///@return �e�N�X�`���摜��
		unsigned int GetWidth();
		///@return �e�N�X�`���摜����
		unsigned int GetHeight();

	private:
		GLuint textureID;
		unsigned int width;
		unsigned int height;
	};
}
