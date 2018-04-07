#pragma once

#include<unordered_map>
#include"Texture.h"

namespace K_Graphics {
	//テクスチャクラスをリストとして持ち、解放責任をもつクラス
	class TextureList {
	public:
		TextureList();
		~TextureList();

		void Initialize();
		Texture* GetTexture(const std::string& textureName);
		bool LoadTexture(const std::string& textureName, const std::string& filePass);
		bool AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight);
		void DeleteTexture(const std::string& textureName);

	private:
		std::unordered_map<std::string, Texture*> textureList;

	};
}
