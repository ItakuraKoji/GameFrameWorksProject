#pragma once
#include<GLEW\glew.h>
#include<fstream>
#include<string>

namespace K_Loader {
	//画像読み込みクラス。3Dモデルデータと異なりデータが少ないのでポインタの譲渡とかはしない
	class ImageLoader {
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
		bool LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight);

	private:
		void SetTgaData(char* data, char* src, int width, int height, int pixelDepth, bool xReverse, bool yReverse);
		void DecodeRLEImage(char* data, char* src, int width, int height, int pixelDepth);
	};
}
