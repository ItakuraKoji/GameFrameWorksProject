#pragma once
#include<png.h>
#include<GLEW\glew.h>
#include<fstream>
#include<string>

namespace K_Loader {
	//�摜�f�[�^�����i�[����\����
	class ImageData {
	public:
		enum ImageType {
			RGBA = GL_RGBA,
			BGRA = GL_BGRA,
			RGB = GL_RGB,
			BGR = GL_BGR
		};

	public:
		ImageData();
		~ImageData();
		void SetData(unsigned char* data, int width, int height, ImageType type, int pixelDepth);
		unsigned char* GetData();
		int GetWidth();
		int GetHeight();
		ImageType GetType();
		int GetDepth();
	private:
		//�f�[�^���̂���
		unsigned char* data;
		//�摜�T�C�Y
		int width;
		int height;
		//�摜�̃^�C�v
		ImageType type;
		//1�s�N�Z���̃r�b�g�T�C�Y
		int pixelDepth;
	};

	//�摜�ǂݍ��݃N���X�B3D���f���f�[�^�ƈقȂ�f�[�^�����Ȃ��̂Ń|�C���^�̏��n�Ƃ��͂��Ȃ�
	class ImageLoader {


	private:
		struct TGAHeader {
			char idSize;//�摜�T�C�Y�O��ID�̃T�C�Y
			char data1[1];//�p�f�B���O
			char type;//�摜�^�C�v
			char data2[9];//�p�f�B���O
			short width;//�摜��
			short height;//�摜����
			char bpp;//�摜�[�x
			char descriptor;//�摜�f�X�N���v�^
		};
	public:
		bool LoadTGAImage(const std::string& fileName, ImageData* result);
		bool LoadPNGImage(const std::string& fileName, ImageData* result);

	private:
		void SetTgaData(unsigned char* data, unsigned char* src, int width, int height, int pixelDepth, bool xReverse, bool yReverse);
		void DecodeRLEImage(unsigned char* data, unsigned char* src, int width, int height, int pixelDepth);
	};
}
