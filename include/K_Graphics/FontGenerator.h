#pragma once

#include<unordered_map>
#include<FreeType\ft2build.h>
#include FT_FREETYPE_H

#include"Texture.h"
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	//フォントアトラスを作成・保持・更新するクラス
	//主にフォント描画クラスに使ってもらう
	//今のところ一つのフォントだけを扱う
	class FontGenerator {
	public:
		//フォントテクスチャの情報
		struct FontAtlas {
			Texture* texture;
			char*    buffer;
			unsigned int width;
			unsigned int height;
			unsigned int textureWidth;
			unsigned int textureHeight;
		};
		struct FontData {
			//Texture UV by Pixel Unit
			K_Math::Box2D locationUV;
			//Rendering Position("Y" direction)
			int fixPositionY;
			//next Font Position("X" direction)
			int advance;
		};

		//サイズは縦と横で同じものしか受け付けない
		//縦長とか横長とかはスケールいじって各自でやってくだせえ
		struct CharData {
			// key   : fontSize
			// value : fontAtlas UV
			std::unordered_map<int, FontData> fontData;
		};

	public:
		FontGenerator(const char* fontFilePass);
		~FontGenerator();

		//すでに文字が存在しているかを調べて、新しい文字なら追加
		void SetFont(wchar_t character, int fontSize);

		//テクスチャを取得
		Texture* GetFontAtlas(int fontSize);
		//指定の文字UVを取得
		//データがなかったらfalse返しの上で参照には一切の変化なし（新規作成は扱う側に任せる）
		bool GetFontData(FontData& result, wchar_t font, int fontSize);

		//指定したフォントサイズのテクスチャを空にする
		void ClearFontAtlas(int fontSize);
		//全てのフォントアトラスを削除
		void ClearAllFontAtlas();

	private:
		void ShowPixelType(FT_Bitmap bitmap);
		bool ExistFont(wchar_t character, int fontSize);
		void CreateNewTexture(int fontSize);
		void CreateNewFontAtlas(wchar_t character, int fontSize);
		//2の累乗値で適切なテクスチャサイズを算出して返す
		int CalclateTextureSize(int size);

	private:
		// key   : fontSize
		// value : Pointer to TextureClass and ImageData Array
		std::unordered_map<int, FontAtlas> fontAtlas;
		// key   : character
		// value : character's fontSize And fontAtlas UVs
		std::unordered_map<wchar_t, CharData> fontData;

		//FreeType
		FT_Library ftLib;
		FT_Face ftFace;
	};

}
