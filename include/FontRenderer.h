#pragma once
#include"FontGenerator.h"
#include"MeshModel.h"
#include"MyMathFanctions.h"
#include<map>

namespace K_Graphics {

	//フォントを生成し、描画するクラス(ワイド文字列)
	class FontRenderer {
	private:
		struct FontDrawData {
			wchar_t* buffer;//出力する文字列
			int fontSize;//サイズ
			float posX;//X座標
			float posY;//Y座標
			float posZ;//Z座標（3D描画に利用）
		};

	public:
		FontRenderer();
		~FontRenderer();
		//事前に呼ばれた描画命令をここで行う
		void Draw(CameraClass* camera, ShaderClass* shader);

		//FontGeneratorを初期化
		bool LoadFont(const char* fontName, const char* filePass);
		//使用するフォントを設定、失敗するとfalseを返し、セットされない
		bool SetCullentFont(const char* fontName);

		//指定座標からテキストを描画（位置はスクリーンの座標）
		void DrawString2D(const std::string& text, int fontSize, int posX, int posY);
		//指定座標からテキストを描画（位置は空間の座標）
		void DrawString3D(const std::string& text, int fontSize, float posX, float posY, float posZ);

		//フォント描画に利用しているテクスチャを削除する
		void ClearFontTexture(int size);
		//フォント描画に利用しているテクスチャをすべて削除する
		void ClearAllFontTexture();

	private:
		bool CreateFontBuffer(wchar_t* result, const char* src, int numChar);
		void DrawBuffers3D(FontDrawData& data, CameraClass* camera, ShaderClass* shader);

	private:
		FontGenerator * cullentFont;
		std::unordered_map<std::string, FontGenerator*> generators;
		K_Graphics::SpriteObject* sprite;

		//呼ばれた描画命令に必要な情報を格納しておいて、Draw()の時に使用
		std::vector<FontDrawData> drawData2D;
		std::vector<FontDrawData> drawData3D;
	};

}
