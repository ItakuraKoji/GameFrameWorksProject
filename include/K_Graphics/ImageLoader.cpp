#include"ImageLoader.h"

namespace K_Loader {

	////////
	//public
	////
	//TGA�t�@�C����ǂݍ���
	bool ImageLoader::LoadTGAImage(const std::string& fileName, ImageData* result, bool xReverse, bool yReverse) {
		std::ifstream file;

		TGAHeader header;
		unsigned int numColor;
		ImageData::ImageType tgaColorFormat;


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

		//���܂̂Ƃ����RGB�`����TGA�C���[�W����������
		if (type != 2 && type != 10) {
			file.close();
			return false;
		}

		//�[�x��񂩂�F�̃t�H�[�}�b�g������
		if (bpp == 32) {
			numColor = 4;
			tgaColorFormat = ImageData::ImageType::BGRA;
		}
		else if (bpp == 24) {
			numColor = 3;
			tgaColorFormat = ImageData::ImageType::BGR;
		}
		else {
			file.close();
			return false;
		}

		//�z��T�C�Y�Ɣz��̃f�[�^��ݒ�
		int imagesize = width * height * numColor;
		unsigned char* tgaPreImage = new unsigned char[imagesize];
		unsigned char* tgaImage = new unsigned char[imagesize];

		file.seekg(idSize, std::ios::cur);
		file.read((char*)tgaPreImage, imagesize);

		//RLE���kRGB�f�[�^
		if (type == 10) {
			//�ǂݍ��񂾃f�[�^�����ƂɃf�R�[�h�̃f�[�^��tgaPreImage�̃o�b�t�@��u��������i���̃f�[�^��delete�j
			unsigned char* temp = tgaPreImage;
			tgaPreImage = new unsigned char[imagesize];
			DecodeRLEImage(tgaPreImage, temp, width, height, numColor);
			delete[] temp;
		}

		CreateReverseImage(tgaImage, tgaPreImage, width, height, numColor, ((descriptor & 0x10) != 0) != xReverse, ((descriptor & 0x20) != 0) != yReverse);
		file.close();

		result->SetData(tgaImage, width, height, tgaColorFormat, numColor);
		delete[] tgaPreImage;
		return true;
	}


	bool ImageLoader::LoadPNGImage(const std::string& fileName, ImageData* result, bool xReverse, bool yReverse) {
		// png�摜�t�@�C���̃��[�h
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

		//�摜�t�H�[�}�b�g���Ή����ĂȂ�������A��
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
			fclose(fp);
			return false;
		}


		// �������̈�m��
		int rb = (int)png_get_rowbytes(sp, ip);
		//�z��T�C�Y�Ɣz��̃f�[�^��ݒ�
		png_bytep pngImage = new png_byte[height * rb];
		png_bytep pngPreImage = new png_byte[height * rb];
		png_bytepp recv = new png_bytep[height];

		for (unsigned int i = 0; i < height; i++) {
			//��񂸂A�h���X�����āA�̂���png_readimage()�ɂēǂݍ���ł��炤
			recv[i] = &pngPreImage[i * rb];
		}
		png_read_image(sp, recv);
		png_read_end(sp, ip);

		//OpenGL���Ɖ摜�̌������Ⴄ�̂őg�݊���
		CreateReverseImage((unsigned char*)pngImage, (unsigned char*)pngPreImage, width, height, numColor, xReverse, !yReverse);

		png_destroy_read_struct(&sp, &ip, NULL);
		fclose(fp);

		result->SetData(pngImage, width, height, colorFormat, numColor);

		delete[] recv;
		delete[] pngPreImage;
		return true;
	}



	////////
	//private
	////


	//TGA�֘A
	void ImageLoader::CreateReverseImage(unsigned char* data, unsigned char* src, int width, int height, int numColor, bool xReverse, bool yReverse) {
		int count = 0;
		//X:�E���獶�@Y:�������@�𐳂̕����Ƃ���
		//XY�ɔ��]�Ȃ�
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
		//X�̂ݔ��]
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
		//Y�̂ݔ��]
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
		//�������]
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

	//RLE���k���𓀂���
	void ImageLoader::DecodeRLEImage(unsigned char* data, unsigned char* src, int width, int height, int numColor) {
		int imageSize = width * height * numColor;
		char* colorData = new char[numColor];
		int count = 0;
		int srcOffset = 0;

		while (count < imageSize && srcOffset < imageSize) {
			//���f�[�^��RLE���k�f�[�^���𔻕�
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
				//��
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
	//TGA�֘A/


	//PNG�֘A
}
