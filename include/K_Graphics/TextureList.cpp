#include"TextureList.h"

namespace K_Graphics {

	////////
	//public
	////
	TextureList::TextureList() {
		Initialize();
	}
	TextureList::~TextureList() {
		Initialize();
	}

	void TextureList::Initialize() {
		for (auto it = this->textureList.begin(); it != this->textureList.end(); ++it) {
			delete (*it).second;
		}
		this->textureList.clear();
	}

	Texture* TextureList::GetTexture(const std::string& textureName) {
		if (this->textureList.find(textureName) == this->textureList.end()) {
			return nullptr;
		}
		return this->textureList[textureName];
	}
	bool TextureList::LoadTexture(const std::string& textureName, const std::string& fileName) {
		//�������O�̍ēo�^�͔F�߂Ȃ��i�����e�N�X�`���͈�ɂ܂Ƃ܂�j
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return true;
		}
		Texture* texture = new Texture;
		try {
			//�e�N�X�`���ǂݍ���
			texture->Initialize();
			if (!texture->LoadImage(fileName.data())) {
				delete texture;
				return false;
			}

			this->textureList[textureName] = texture;
		}
		catch (std::exception& e) {
			//��O����
			if (texture != nullptr) {
				delete texture;
			}
			throw e;
		}
		return true;
	}
	bool TextureList::AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight) {
		//�������O�̍ēo�^�͔F�߂Ȃ��i�����e�N�X�`���͈�ɂ܂Ƃ܂�j
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return false;
		}

		Texture* texture = new Texture;
		texture->Initialize();
		texture->SetImageData(NULL, textureWidth, textureHeight, TextureType::Unsigned_Byte, TextureColorType::RGBA32F, TextureColorType::RGBA);

		this->textureList[textureName] = texture;
		return true;
	}
	bool TextureList::AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight, TextureType dataType, TextureColorType color, TextureColorType dataColor) {
		//�������O�̍ēo�^�͔F�߂Ȃ��i�����e�N�X�`���͈�ɂ܂Ƃ܂�j
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return false;
		}

		Texture* texture = new Texture;
		texture->Initialize();
		texture->SetImageData(NULL, textureWidth, textureHeight, dataType, color, dataColor);

		this->textureList[textureName] = texture;
		return true;
	}

	void TextureList::DeleteTexture(const std::string& textureName) {
		//�������͍폜�ł��Ȃ�
		if (this->textureList.find(textureName) == this->textureList.end()) {
			return;
		}

		delete this->textureList[textureName];
		this->textureList.erase(textureName);
	}

}