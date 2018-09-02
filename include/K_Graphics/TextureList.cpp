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
		//同じ名前の再登録は認めない（同じテクスチャは一つにまとまる）
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return true;
		}
		Texture* texture = new Texture;
		try {
			//テクスチャ読み込み
			texture->Initialize();
			if (!texture->LoadImage(fileName.data())) {
				delete texture;
				return false;
			}

			this->textureList[textureName] = texture;
		}
		catch (std::exception& e) {
			//例外処理
			if (texture != nullptr) {
				delete texture;
			}
			throw e;
		}
		return true;
	}
	bool TextureList::AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight) {
		//同じ名前の再登録は認めない（同じテクスチャは一つにまとまる）
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
		//同じ名前の再登録は認めない（同じテクスチャは一つにまとまる）
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
		//無い物は削除できない
		if (this->textureList.find(textureName) == this->textureList.end()) {
			return;
		}

		delete this->textureList[textureName];
		this->textureList.erase(textureName);
	}

}