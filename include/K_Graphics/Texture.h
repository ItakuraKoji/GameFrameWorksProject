#pragma once
#include<GLEW\glew.h>
#include<string>
#include"ImageLoader.h"

namespace K_Graphics {
	///@brief テクスチャが扱う値の型
	enum TextureType {
		Unsigned_Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
	};
	///@brief テクスチャに渡される情報の解釈
	enum TextureColorType {
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA32F = GL_RGBA32F,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RED = GL_RED,
	};

	///@brief 外部から読み込まれた、あるいは作られたテクスチャを保持する\nこのクラスは、保持しているテクスチャの解放責任を持つ
	class Texture {
	public:
		///@brief Initialize()を呼ぶ
		Texture();
		///@brief テクスチャを開放する
		~Texture();
		///@brief 空のデータでテクスチャを初期化
		bool Initialize();
		///@brief 画像を読み込んでテクスチャに渡す
		///@param fileName 画像のファイルパス
		///@return 成功するとtrue
		bool LoadImage(const std::string& fileName);
		///@brief 配列データを画像として渡す
		///@param[in] data データの入った配列
		///@param[in] width 画像幅
		///@param[in] height 画像高さ
		///@param[in] bType テクスチャの数値型の設定
		///@param[in] textureColor テクスチャの色の形式
		///@param[in] dataColor テクスチャが利用する画像の色の形式
		void SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor);
		///@brief 画像の拡縮時にフィルターをかけるか設定
		///@param[in] isFiltering フィルタリングをするかのフラグ
		void SetFilter(bool isFiltering);
		///@return シェーダーに渡す際のテクスチャインデックス
		GLuint GetTextureID();
		///@return テクスチャ画像幅
		unsigned int GetWidth();
		///@return テクスチャ画像高さ
		unsigned int GetHeight();

	private:
		GLuint textureID;
		unsigned int width;
		unsigned int height;
	};
}
