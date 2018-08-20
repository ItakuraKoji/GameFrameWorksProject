#include"ImageLoader.h"

namespace K_Loader {



	//�摜�f�[�^���בւ�
	void CreateReverseImage(unsigned char* data, unsigned char* src, int width, int height, int numColor, bool xReverse, bool yReverse) {

		//X:�E���獶�@Y:�������@�𐳂̕����Ƃ���
		int count = 0;

		//XY�ɔ��]�Ȃ�
		if (!(xReverse) && !(yReverse)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					for (int i = 0; i < numColor; ++i) {
						//���F���ʒu�@�c�F�����~�c�ʒu
						//�s�N�Z���ʒu�F�i���{�c�j�~�F�� �{�F�ʒu
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
						//���F���ʒu�@�c�F�����~�c�ʒu
						//�s�N�Z���ʒu�F�i���{�c�j�~�F�� �{�F�ʒu
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
						//���F���ʒu�@�c�F�����~�c�ʒu
						//�s�N�Z���ʒu�F�i���{�c�j�~�F�� �{�F�ʒu
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
						//���F���ʒu�@�c�F�����~�c�ʒu
						//�s�N�Z���ʒu�F�i���{�c�j�~�F�� �{�F�ʒu
						data[count] = src[(y * width + x) * numColor + i];
						++count;
					}
				}
			}
		}
	}
	void ReadBinary(char* dest, char* src, size_t* position, size_t size) {
		memcpy(dest, src + (*position), size);
		//�ǂ݂��񂾕������ʒu��i�߂�
		(*position) += size;
	}



	////////
	//public
	////
	//TGA�t�@�C����ǂݍ���
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
			//�t�@�C���T�C�Y�擾
			ifs.seekg(0, std::ios::end);
			size_t binarySize = ifs.tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);

			//�o�C�i����S���ǂ݂���
			binaryData = new char[binarySize];
			ifs.read(binaryData, binarySize);
			ifs.close();

			//�o�C�i������ǂݍ���
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
		//�o�C�i�����ǂ��܂œǂ񂾂��A�Ƃ����J�E���^
		size_t binaryPosition = 0;

		//�w�b�_�ǂݍ���
		TGAHeader header;
		//file.read((char*)&header, sizeof(TGAHeader));
		ReadBinary((char*)&header, binaryData, &binaryPosition, sizeof(TGAHeader));
		int idSize = (int)header.idSize;
		int type = (int)header.type;
		int width = (int)header.width;
		int height = (int)header.height;
		int bpp = (int)header.bpp;
		int descriptor = (int)header.descriptor;

		//���܂̂Ƃ����RGB�`����TGA�C���[�W����������
		if (type != 2 && type != 10) {
			throw std::runtime_error("unsupport tga format");
		}

		//�[�x��񂩂�F�̃t�H�[�}�b�g������
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

		//�z��T�C�Y�Ɣz��̃f�[�^��ݒ�
		int imagesize = width * height * numColor;
		unsigned char* tgaPreImage = new unsigned char[imagesize];
		unsigned char* tgaImage = new unsigned char[imagesize];

		//���k����RGB�̏ꍇ�̓ǂݍ���
		if (type == 2) {
			binaryPosition += idSize;
			ReadBinary((char*)tgaPreImage, binaryData, &binaryPosition, sizeof(char) * imagesize);
		}

		//RLE���kRGB�f�[�^�̏ꍇ�̓ǂݍ���
		if (type == 10) {
			//�ǂݍ��񂾃f�[�^�����ƂɃf�R�[�h�̃f�[�^��tgaPreImage�̃o�b�t�@��u��������i���̃f�[�^��delete�j
			DecodeRLEImage(tgaPreImage, (unsigned char*)binaryData, &binaryPosition, imagesize, numColor);
		}

		//�摜�̌��_�ɍ��킹�ĉ摜�S�̂𔽓]
		CreateReverseImage(tgaImage, tgaPreImage, width, height, numColor, ((descriptor & 0x10) != 0) != xReverse, ((descriptor & 0x20) != 0) != yReverse);

		//�\���̂ɓǂݍ��݌��ʂ��i�[
		result->SetData(tgaImage, width, height, tgaColorFormat, numColor);
		delete[] tgaPreImage;
	}

	//RLE���k���𓀂���
	void TGALoader::DecodeRLEImage(unsigned char* data, unsigned char* src, size_t* binaryPosition, int imageSize, int numColor) {
		char* colorData = new char[numColor];
		int count = 0;

		while (count < imageSize && (*binaryPosition) < imageSize) {
			//���f�[�^��RLE���k�f�[�^���𔻕�
			char controlPacket;
			ReadBinary((char*)&controlPacket, (char*)src, binaryPosition, sizeof(char));
			int packet = controlPacket & 0xFF;

			if ((packet & 0x80) != 0) {
				//RLE
				//�F�f�[�^�ǂݍ���
				ReadBinary(colorData, (char*)src, binaryPosition, sizeof(char) * numColor);

				//�J��Ԃ��̕������F����
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
				//�F�f�[�^�����̂܂܃R�s�[
				int loopCount = (packet + 1) * numColor;
				ReadBinary((char*)&data[count], (char*)src, binaryPosition, sizeof(char) * loopCount);
				count += loopCount;
			}
		}
		delete[] colorData;
	}

	//png_ptr  : png�\���́A������g����png_set_read_fn()�Ŏ��O�ɓn���Ă�����void*�ւƃA�N�Z�X�\
	//buf      : �f�[�^���������ޗ̈�
	//readSize : �ǂݍ��ރT�C�Y
	void PNGBinaryReadCallBack(png_structp png_ptr, png_bytep buf, png_size_t readSize) {
		//���O�ɓn�����Ǝ��f�[�^�擾
		PNGLoader::PNGFileData* data = (PNGLoader::PNGFileData*)png_get_io_ptr(png_ptr);
		//�T�C�Y�`�F�b�N
		if (readSize + data->binaryPosition <= data->dataSize) {
			//�ǂݍ���
			memcpy(buf, data->pngData + data->binaryPosition, readSize);
			data->binaryPosition += readSize;
		}
		else {
			//�T�C�Y���ُ�
			png_error(png_ptr, "png_mem_read_func failed");
		}

	}

	void PNGLoader::LoadFromFile(const std::string & fileName, ImageData * result, bool xReverse, bool yReverse){
		// png�摜�t�@�C���̃��[�h
		char* binaryData = nullptr;
		try {
			std::ifstream ifs(fileName, std::ios::binary | std::ios::in);
			if (!ifs) {
				throw std::runtime_error("file open failed");
			}
			//�t�@�C���T�C�Y�擾
			ifs.seekg(0, std::ios::end);
			size_t binarySize = ifs.tellg();
			ifs.clear();
			ifs.seekg(0, std::ios::beg);

			//�o�C�i����S���ǂ݂���
			binaryData = new char[binarySize];
			ifs.read(binaryData, binarySize);
			ifs.close();

			//�K�v�ȃf�[�^��ݒ�
			this->data.binaryPosition = 0;
			this->data.pngData = binaryData;
			this->data.dataSize = binarySize;

			//�o�C�i������ǂݍ���
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
			throw std::runtime_error("unsupport file format");
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

		result->SetData(pngImage, width, height, colorFormat, numColor);

		delete[] recv;
		delete[] pngPreImage;
		return;
	}

}
