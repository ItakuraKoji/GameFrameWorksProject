#pragma once

#include<unordered_map>
#include"Texture.h"

namespace K_Graphics {
	///@brief テクスチャクラスをリストとして持ち、解放責任をもつクラス
	class TextureList {
	public:
		///@brief Initialize()を呼ぶ
		TextureList();
		///@brief Initialize()を呼ぶ
		~TextureList();

		///@brief リストのテクスチャを全て開放する
		void Initialize();
		///@brief テクスチャを取得
		///@param[in] textureName テクスチャの名前
		///@return その名前のテクスチャへのポインタ、無かったらnullptr
		Texture* GetTexture(const std::string& textureName);
		///@brief テクスチャ読み込み
		///@param[in] textureName 作成するテクスチャのユーザー定義名
		///@param[in] filePass 画像のファイルパス
		///@return 成功するとtrue
		bool LoadTexture(const std::string& textureName, const std::string& filePass);
		///@brief 空のテクスチャを作成
		///@param[in] textureName 作成するテクスチャのユーザー定義名
		///@param[in] textureWidth テクスチャの画像幅
		///@param[in] textureHeight テクスチャの画像高さ
		///@return 成功するとtrue
		bool AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight);
		///@brief テクスチャを削除
		///@param[in] textureName 削除するテクスチャの名前
		void DeleteTexture(const std::string& textureName);

	private:
		std::unordered_map<std::string, Texture*> textureList;

	};
}
