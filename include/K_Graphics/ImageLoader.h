#pragma once
#include<png.h>
#include<GLEW\glew.h>
#include<fstream>
#include<string>

namespace K_Loader {
	//画像データ情報を格納する構造体
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
		//データそのもの
		unsigned char* data;
		//画像サイズ
		int width;
		int height;
		//画像のタイプ
		ImageType type;
		//1ピクセルのビットサイズ
		int pixelDepth;
	};

	//各種画像フォーマットを読み込む基底クラス
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
			char idSize;//画像サイズ前のIDのサイズ
			char data1[1];//パディング
			char type;//画像タイプ
			char data2[9];//パディング
			short width;//画像幅
			short height;//画像高さ
			char bpp;//画像深度
			char descriptor;//画像デスクリプタ
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
			char* pngData;//PNGのバイナリデータ
			size_t dataSize;//全体のデータサイズ
			size_t binaryPosition;//バイナリをどこまで読んだかというデータ
		};

	public:
		virtual ~PNGLoader() = default;
		virtual void LoadFromFile  (const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false) override;
		virtual void LoadFromBinary(char* binaryData           , ImageData* result, bool xReverse = false, bool yReverse = false) override;

	private:
		PNGFileData data;
	};


	//画像読み込みクラス。3Dモデルデータと異なりデータが少ないのでポインタの譲渡とかはしない
	class ImageLoader {
	public:
		bool LoadTGAImage(const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false);
		bool LoadPNGImage(const std::string& fileName, ImageData* result, bool xReverse = false, bool yReverse = false);
	};
}
