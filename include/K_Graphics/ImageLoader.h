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

	//�e��摜�t�H�[�}�b�g��ǂݍ��ފ��N���X
	class ImageDataLoader {
	public:
		virtual ~ImageDataLoader() = default;
		virtual void LoadFromFile(const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false) = 0;
		virtual void LoadFromBinary(char* binaryData, ImageData* result, bool xReverse = false, bool yReverse = false) = 0;
	};

	//TGA
	class TGALoader : public ImageDataLoader {
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
		virtual ~TGALoader() = default;
		virtual void LoadFromFile  (const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false) override;
		virtual void LoadFromBinary(char* binaryData           , ImageData* result, bool xReverse = false, bool yReverse = false) override;

	private:
		void DecodeRLEImage    (unsigned char* data, unsigned char* src, size_t* binaryPosition, int imageSize, int numColor);
	};

	//PNG
	class PNGLoader : public ImageDataLoader {
	public:
		struct PNGFileData {
			char* pngData;//PNG�̃o�C�i���f�[�^
			size_t dataSize;//�S�̂̃f�[�^�T�C�Y
			size_t binaryPosition;//�o�C�i�����ǂ��܂œǂ񂾂��Ƃ����f�[�^
		};

	public:
		virtual ~PNGLoader() = default;
		virtual void LoadFromFile  (const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false) override;
		virtual void LoadFromBinary(char* binaryData           , ImageData* result, bool xReverse = false, bool yReverse = false) override;

	private:
		PNGFileData data;
	};


	//�摜�ǂݍ��݃N���X�B3D���f���f�[�^�ƈقȂ�f�[�^�����Ȃ��̂Ń|�C���^�̏��n�Ƃ��͂��Ȃ�
	class ImageLoader {
	public:
		bool LoadTGAImage(const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false);
		bool LoadPNGImage(const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false);
	};
}
