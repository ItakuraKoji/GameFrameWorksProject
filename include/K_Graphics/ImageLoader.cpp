#include"ImageLoader.h"

namespace K_Loader {

	////////
	//public
	////

	//TGAファイルを読み込み
	bool ImageLoader::LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight) {
		std::ifstream file;

		TGAHeader header;
		unsigned int numColor;
		GLenum glColorFormat, tgaColorFormat;


		file.open(fileName, std::ios::binary | std::ios::in);
		if (file.fail()) {
			return false;
		}
		file.read((char*)&header, sizeof(TGAHeader));


		int idSize = (int)header.idSize;
		int type = (int)header.type;
		int width = (int)header.width;
		int height = (int)header.height;
		int bpp = (int)header.bpp;
		int descriptor = (int)header.descriptor;

		//いまのところはRGB形式のTGAイメージだけを扱う
		if (type != 2 && type != 10) {
			file.close();
			return false;
		}

		//深度情報から色のフォーマットを決定
		if (bpp == 32) {
			numColor = 4;
			glColorFormat = GL_RGBA;
			tgaColorFormat = GL_BGRA;
		}
		else if (bpp == 24) {
			numColor = 3;
			glColorFormat = GL_RGBA;
			tgaColorFormat = GL_BGR;
		}
		else {
			file.close();
			return false;
		}

		//配列サイズと配列のデータを設定
		int imagesize = width * height * numColor;
		char* tgaPreImage = new char[imagesize];
		char* tgaImage = new char[imagesize];

		file.seekg(idSize, std::ios::cur);
		file.read(tgaPreImage, imagesize);

		//RLE圧縮RGBデータ
		if (type == 10) {
			//読み込んだデータをもとにデコードのデータでtgaPreImageのバッファを置き換える（元のデータはdelete）
			char* temp = tgaPreImage;
			tgaPreImage = new char[imagesize];
			DecodeRLEImage(tgaPreImage, temp, width, height, numColor);
			delete[] temp;
		}

		SetTgaData(tgaImage, tgaPreImage, width, height, numColor, descriptor & 0x10, descriptor & 0x20);

		file.close();

		returnWidth = width;
		returnHeight = height;

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, glColorFormat, width, height, 0, tgaColorFormat, GL_UNSIGNED_BYTE, tgaImage);

		//範囲外の表示
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//拡大縮小時の補完
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//ミップマップを作成
		glGenerateMipmap(GL_TEXTURE_2D);

		delete[] tgaPreImage;
		delete[] tgaImage;

		return true;
	}


	bool ImageLoader::LoadPNGImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight) {
		// png画像ファイルのロード
		FILE* fp;
		fopen_s(&fp, fileName.data(), "rb");
		if (!fp) {
			return false;
		}

		png_structp sp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop   ip = png_create_info_struct(sp);

		png_init_io(sp, fp);
		png_read_info(sp, ip);

		unsigned int width, height;
		int depth, colortype, interlacetype, numColor;
		png_get_IHDR(sp, ip, (png_uint_32*)&width, (png_uint_32*)&height, &depth, &colortype, &interlacetype, NULL, NULL);

		//画像フォーマットが対応してなかったら帰る
		if (colortype == PNG_COLOR_TYPE_RGBA) {
			numColor = 4;
		}
		else if (colortype == PNG_COLOR_TYPE_RGB) {
			numColor = 3;
		}
		else {
			png_destroy_read_struct(&sp, &ip, NULL);
			fclose(fp);
			return false;
		}


		// メモリ領域確保
		int rb = png_get_rowbytes(sp, ip);
		//配列サイズと配列のデータを設定
		png_bytep pngImage = new png_byte[height * rb];
		png_bytep pngPreImage = new png_byte[height * rb];
		png_bytepp recv = new png_bytep[height];

		for (int i = 0; i < height; i++) {
			//一列ずつアドレスを入れて、のちにpng_readimage()にて読み込んでもらう
			recv[i] = &pngPreImage[i * rb];
		}
		png_read_image(sp, recv);
		png_read_end(sp, ip);

		//OpenGLだと画像の向きが違うので組み換え
		SetTgaData((char*)pngImage, (char*)pngPreImage, width, height, numColor, true, true);

		png_destroy_read_struct(&sp, &ip, NULL);
		fclose(fp);

		// テクスチャへの登録
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pngImage);
		//範囲外の表示
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//拡大縮小時の補完
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//ミップマップを作成
		glGenerateMipmap(GL_TEXTURE_2D);
		
		delete[] recv;
		delete[] pngImage;
		delete[] pngPreImage;

		returnWidth = width;
		returnHeight = height;
		return true;
	}



	////////
	//private
	////


	//TGA関連
	void ImageLoader::SetTgaData(char* data, char* src, int width, int height, int numColor, bool xReverse, bool yReverse) {
		int count = 0;
		//X:右から左　Y:下から上　を正の方向とする
		//XYに反転なし
		if (!(xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//Xのみ反転
		if ((xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = width - 1; x >= 0; --x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//Yのみ反転
		if (!(xReverse) && (yReverse)) {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
		//両方反転
		if ((xReverse) && (yReverse)) {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = width - 1; x >= 0; --x) {
					for (int i = 0; i < numColor; ++i) {
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
	}

	//RLE圧縮を解凍する
	void ImageLoader::DecodeRLEImage(char* data, char* src, int width, int height, int numColor) {
		int imageSize = width * height * numColor;
		char* colorData = new char[numColor];
		int count = 0;
		int srcOffset = 0;

		while (count < imageSize) {
			//生データかRLE圧縮データかを判別
			int packet = src[srcOffset];
			++srcOffset;

			if (packet & 0x80) {
				//RLE
				for (int i = 0; i < numColor; ++i) {
					colorData[i] = src[srcOffset];
					++srcOffset;
				}

				int loopCount = (packet & 0x7F) + 1;
				for (int i = 0; i < loopCount; ++i) {
					for (int j = 0; j < numColor; ++j) {
						data[count] = colorData[j];
						++count;
					}
				}
			}
			else {
				//生
				int loopCount = (packet + 1) * numColor;
				for (int i = 0; i < loopCount; ++i) {
					data[count] = src[srcOffset];
					++count;
					++srcOffset;
				}
			}
		}
		delete[] colorData;
	}
	//TGA関連/


	//PNG関連
}
