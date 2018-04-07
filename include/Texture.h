#pragma once
#include<GLEW\glew.h>
#include<string>
#include"ImageLoader.h"

namespace K_Graphics {
	//テクスチャが扱う値の型
	enum TextureType {
		Unsigned_Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
	};
	//テクスチャに渡される情報の解釈
	enum TextureColorType {
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA32F = GL_RGBA32F,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RED = GL_RED,
	};

	//外部から読み込まれた、あるいは作られたテクスチャを保持する
	//このクラスは、保持しているテクスチャの解放責任を持つ
	class Texture {
	public:
		Texture();
		~Texture();
		bool Initialize();
		bool LoadImage(const std::string& fileName);
		void SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor);
		void SetFilter(bool isFiltering);
		GLuint GetTextureID();
		unsigned int GetWidth();
		unsigned int GetHeight();

	private:
		GLuint textureID;
		unsigned int width;
		unsigned int height;
	};
}
