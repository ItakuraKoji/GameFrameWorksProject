#include"ImageLoader.h"

namespace K_Loader {



	//画像データ並べ替え
	void CreateReverseImage(unsigned char* data, unsigned char* src, int width, int height, int numColor, bool xReverse, bool yReverse) {

		//X:右から左　Y:下から上　を正の方向とする
		int count = 0;

		//XYに反転なし
		if (!(xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						//横：横位置　縦：横幅×縦位置
						//ピクセル位置：（横＋縦）×色数 ＋色位置
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
						//横：横位置　縦：横幅×縦位置
						//ピクセル位置：（横＋縦）×色数 ＋色位置
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
						//横：横位置　縦：横幅×縦位置
						//ピクセル位置：（横＋縦）×色数 ＋色位置
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
						//横：横位置　縦：横幅×縦位置
						//ピクセル位置：（横＋縦）×色数 ＋色位置
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
	}
	void ReadBinary(char* dest, char* src, size_t* position, size_t size) {
		memcpy(dest, src + (*position), size);
		//読みこんだ分だけ位置を進める
		(*position) += size;
	}



	////////
	//public
	////
	//TGAファイルを読み込み
	bool ImageLoader::LoadTGAImage(const std::string& fileName, ImageData* result, bool xReverse, bool yReverse) {
		TGALoader loader;
		loader.LoadFromFile(fileName, result, xReverse, yReverse);
		return true;
	}


	bool ImageLoader::LoadPNGImage(const std::string& fileName, ImageData* result, bool xReverse, bool yReverse) {
		PNGLoader loader;
		loader.LoadFromFile(fileName, result, xReverse, yReverse);
		return true;
	}



	////////
	//private
	////




	void TGALoader::LoadFromFile(const std::string& fileName, ImageData * result, bool xReverse, bool yReverse){
		char* binaryData = nullptr;
		try {
			std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
			if (!ifs) {
				throw std::runtime_error("file open failed");
			}
			//ファイルサイズ取得
			ifs.seekg(0, std::ios::end);
			size_t binarySize = ifs.tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);

			//バイナリを全部読みこみ
			binaryData = new char[binarySize];
			ifs.read(binaryData, binarySize);
			ifs.close();

			//バイナリから読み込み
			LoadFromBinary(binaryData, result, xReverse, yReverse);

			delete[] binaryData;
		}
		catch (std::exception& e) {
			if (binaryData != nullptr) {
				delete binaryData;
			}
			throw std::runtime_error("image Load Failed : " + std::string(e.what()) + " : "+ fileName);
		}
	}
	void TGALoader::LoadFromBinary(char* binaryData, ImageData * result, bool xReverse, bool yReverse){
		//バイナリをどこまで読んだか、というカウンタ
		size_t binaryPosition = 0;

		//ヘッダ読み込み
		TGAHeader header;
		//file.read((char*)&header, sizeof(TGAHeader));
		ReadBinary((char*)&header, binaryData, &binaryPosition, sizeof(TGAHeader));
		int idSize = (int)header.idSize;
		int type = (int)header.type;
		int width = (int)header.width;
		int height = (int)header.height;
		int bpp = (int)header.bpp;
		int descriptor = (int)header.descriptor;

		//いまのところはRGB形式のTGAイメージだけを扱う
		if (type != 2 && type != 10) {
			throw std::runtime_error("unsupport tga format");
		}

		//深度情報から色のフォーマットを決定
		unsigned int numColor;
		ImageData::ImageType tgaColorFormat;
		if (bpp == 32) {
			numColor = 4;
			tgaColorFormat = ImageData::ImageType::BGRA;
		}
		else if (bpp == 24) {
			numColor = 3;
			tgaColorFormat = ImageData::ImageType::BGR;
		}

		//配列サイズと配列のデータを設定
		int imagesize = width * height * numColor;
		unsigned char* tgaPreImage = new unsigned char[imagesize];
		unsigned char* tgaImage = new unsigned char[imagesize];

		//圧縮無しRGBの場合の読み込み
		if (type == 2) {
			binaryPosition += idSize;
			ReadBinary((char*)tgaPreImage, binaryData, &binaryPosition, sizeof(char) * imagesize);
		}

		//RLE圧縮RGBデータの場合の読み込み
		if (type == 10) {
			//読み込んだデータをもとにデコードのデータでtgaPreImageのバッファを置き換える（元のデータはdelete）
			DecodeRLEImage(tgaPreImage, (unsigned char*)binaryData, &binaryPosition, imagesize, numColor);
		}

		//画像の原点に合わせて画像全体を反転
		CreateReverseImage(tgaImage, tgaPreImage, width, height, numColor, ((descriptor & 0x10) != 0) != xReverse, ((descriptor & 0x20) != 0) != yReverse);

		//構造体に読み込み結果を格納
		result->SetData(tgaImage, width, height, tgaColorFormat, numColor);
		delete[] tgaPreImage;
	}

	//RLE圧縮を解凍する
	void TGALoader::DecodeRLEImage(unsigned char* data, unsigned char* src, size_t* binaryPosition, int imageSize, int numColor) {
		char* colorData = new char[numColor];
		int count = 0;

		while (count < imageSize && (*binaryPosition) < imageSize) {
			//生データかRLE圧縮データかを判別
			char controlPacket;
			ReadBinary((char*)&controlPacket, (char*)src, binaryPosition, sizeof(char));
			int packet = controlPacket & 0xFF;

			if ((packet & 0x80) != 0) {
				//RLE
				//色データ読み込み
				ReadBinary(colorData, (char*)src, binaryPosition, sizeof(char) * numColor);

				//繰り返しの分だけ色を代入
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
				//色データをそのままコピー
				int loopCount = (packet + 1) * numColor;
				ReadBinary((char*)&data[count], (char*)src, binaryPosition, sizeof(char) * loopCount);
				count += loopCount;
			}
		}
		delete[] colorData;
	}

	//png_ptr  : png構造体、これを使ってpng_set_read_fn()で事前に渡しておいたvoid*へとアクセス可能
	//buf      : データを書き込む領域
	//readSize : 読み込むサイズ
	void PNGBinaryReadCallBack(png_structp png_ptr, png_bytep buf, png_size_t readSize) {
		//事前に渡した独自データ取得
		PNGLoader::PNGFileData* data = (PNGLoader::PNGFileData*)png_get_io_ptr(png_ptr);
		//サイズチェック
		if (readSize + data->binaryPosition <= data->dataSize) {
			//読み込み
			memcpy(buf, data->pngData + data->binaryPosition, readSize);
			data->binaryPosition += readSize;
		}
		else {
			//サイズが異常
			png_error(png_ptr, "png_mem_read_func failed");
		}

	}

	void PNGLoader::LoadFromFile(const std::string & fileName, ImageData * result, bool xReverse, bool yReverse){
		// png画像ファイルのロード
		char* binaryData = nullptr;
		try {
			std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
			if (!ifs) {
				throw std::runtime_error("file open failed");
			}
			//ファイルサイズ取得
			ifs.seekg(0, std::ios::end);
			size_t binarySize = ifs.tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);

			//バイナリを全部読みこみ
			binaryData = new char[binarySize];
			ifs.read(binaryData, binarySize);
			ifs.close();

			//必要なデータを設定
			this->data.binaryPosition = 0;
			this->data.pngData = binaryData;
			this->data.dataSize = binarySize;

			//バイナリから読み込み
			LoadFromBinary(binaryData, result, xReverse, yReverse);

			delete[] binaryData;
		}
		catch (std::exception& e) {
			if (binaryData != nullptr) {
				delete binaryData;
			}
			throw std::runtime_error("image Load Failed : " + std::string(e.what()) + " : " + fileName);
		}



	}

	void PNGLoader::LoadFromBinary(char* binaryData, ImageData * result, bool xReverse, bool yReverse){
		png_structp sp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop   ip = png_create_info_struct(sp);

		png_set_read_fn(sp, &this->data, PNGBinaryReadCallBack);
		png_read_info(sp, ip);

		unsigned int width, height;
		int depth, colortype, interlacetype, numColor;
		png_get_IHDR(sp, ip, (png_uint_32*)&width, (png_uint_32*)&height, &depth, &colortype, &interlacetype, NULL, NULL);

		//画像フォーマットが対応してなかったら帰る
		ImageData::ImageType colorFormat;
		if (colortype == PNG_COLOR_TYPE_RGBA) {
			colorFormat = ImageData::ImageType::RGBA;
			numColor = 4;
		}
		else if (colortype == PNG_COLOR_TYPE_RGB) {
			colorFormat = ImageData::ImageType::RGB;
			numColor = 3;
		}
		else {
			png_destroy_read_struct(&sp, &ip, NULL);
			throw std::runtime_error("unsupport file format");
		}

		// メモリ領域確保
		int rb = (int)png_get_rowbytes(sp, ip);
		//配列サイズと配列のデータを設定
		png_bytep pngImage = new png_byte[height * rb];
		png_bytep pngPreImage = new png_byte[height * rb];
		png_bytepp recv = new png_bytep[height];

		for (unsigned int i = 0; i < height; i++) {
			//一列ずつアドレスを入れて、のちにpng_readimage()にて読み込んでもらう
			recv[i] = &pngPreImage[i * rb];
		}
		png_read_image(sp, recv);
		png_read_end(sp, ip);

		//OpenGLだと画像の向きが違うので組み換え
		CreateReverseImage((unsigned char*)pngImage, (unsigned char*)pngPreImage, width, height, numColor, xReverse, !yReverse);

		png_destroy_read_struct(&sp, &ip, NULL);

		result->SetData(pngImage, width, height, colorFormat, numColor);

		delete[] recv;
		delete[] pngPreImage;
		return;
	}

}
