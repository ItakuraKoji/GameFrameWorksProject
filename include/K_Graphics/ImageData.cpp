#include"ImageLoader.h"

namespace K_Loader {

	ImageData::ImageData() {
		this->data = nullptr;
	}
	ImageData::~ImageData() {
		if (this->data != nullptr) {
			delete this->data;
		}
	}
	void ImageData::SetData(unsigned char* data, int width, int height, ImageType type, int pixelDepth) {
		if (this->data != nullptr) {
			delete this->data;
		}
		this->data = data;
		this->width = width;
		this->height = height;
		this->type = type;
		this->pixelDepth = pixelDepth;
	}
	unsigned char* ImageData::GetData() {
		return this->data;
	}
	int ImageData::GetWidth() {
		return this->width;
	}
	int ImageData::GetHeight() {
		return this->height;
	}
	ImageData::ImageType ImageData::GetType() {
		return this->type;
	}
	int ImageData::GetDepth() {
		return this->pixelDepth;
	}

}