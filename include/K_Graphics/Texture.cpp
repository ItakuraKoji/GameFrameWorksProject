#include"Texture.h"

namespace K_Graphics {
	////////
	//public
	////
	Texture::Texture() {
		Initialize();
	}
	Texture::~Texture() {
		glDeleteTextures(1, &this->textureID);
	}

	bool Texture::Initialize() {
		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		//‰æ‘œ‚ðƒAƒ^ƒbƒ`(‹ó)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		//‰æ‘œ‚ÌŠO‘¤‚Ì•`‰æ‚É‚Â‚¢‚ÄÝ’è
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//Šg‘åk¬Žž‚Ì•âŠ®‚É‚Â‚¢‚ÄÝ’è
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		return true;
	}

	bool Texture::LoadImage(const std::string& fileName) {
		K_Loader::ImageLoader loader;
		int pos = (int)fileName.rfind(".");
		if (pos == (int)fileName.npos) {
			return false;
		}

		K_Loader::ImageData image;
		//Šg’£Žq‚©‚ç“Ç‚Ýž‚Ý‚ð”»’f
		std::string ext = fileName.substr(pos + 1, 3);
		bool result;
		if (ext == "png" || ext == "PNG") {
			result = loader.LoadPNGImage(fileName, &image);
		}
		else if (ext == "tga" || ext == "TGA") {
			result = loader.LoadTGAImage(fileName, &image);
		}
		else {
			result = false;
		}

		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.GetWidth(), image.GetHeight(), 0, image.GetType(), GL_UNSIGNED_BYTE, image.GetData());
		//Šg‘åk¬Žž‚Ì•âŠ®
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//‰æ‘œ‚ÌŠO‘¤‚Ì•`‰æ‚É‚Â‚¢‚ÄÝ’è
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//ƒ~ƒbƒvƒ}ƒbƒv‚ðì¬
		glGenerateMipmap(GL_TEXTURE_2D);

		this->width = image.GetWidth();
		this->height = image.GetHeight();
		return result;
	}

	//‰æ‘œƒf[ƒ^‚ð“n‚·i‹óƒf[ƒ^‚à‰Â”\j
	void Texture::SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, textureColor, width, height, 0, dataColor, bType, data);
		//‰æ‘œ‚ÌŠO‘¤‚Ì•`‰æ‚É‚Â‚¢‚ÄÝ’è
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//Šg‘åk¬Žž‚Ì•âŠ®‚É‚Â‚¢‚ÄÝ’è
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		this->width = width;
		this->height = height;
	}

	void Texture::SetFilter(bool isFiltering) {
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		if (isFiltering) {
			//Šg‘åk¬Žž‚Ì•âŠ®
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//ƒ~ƒbƒvƒ}ƒbƒv‚ðì¬
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			//Šg‘åk¬Žž‚Ì•âŠ®‚É‚Â‚¢‚ÄÝ’è
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
	}


	GLuint Texture::GetTextureID() {
		return this->textureID;
	}
	unsigned int Texture::GetWidth() {
		return this->width;
	}
	unsigned int Texture::GetHeight() {
		return this->height;
	}

}
