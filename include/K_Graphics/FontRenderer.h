#pragma once
#include"FontGenerator.h"
#include"MeshModel.h"
#include"K_Math\MyMathFanctions.h"
#include<map>

namespace K_Graphics {

	///@brief フォントを生成し、描画するクラス(ワイド文字列)
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

		///@brief 描画に使用するスプライトを初期化
		FontRenderer();
		///@brief スプライトのポインタを開放
		~FontRenderer();

		///@brief 事前に呼ばれた描画命令をここで行う
		///@param[in] camera 使用するカメラ
		///@param[in] shader 使用するシェーダー（スプライトが描画できるもの）
		void Draw(CameraClass* camera, ShaderClass* shader);

		///@brief フォントを読み込む
		///@param[in] fontName フォントのユーザー定義名
		///@param[in] filePass フォントのファイルパス
		bool LoadFont(const char* fontName, const char* filePass);
		///@brief 使用するフォントを設定、失敗するとfalseを返し、セットされない
		///@param[in] fontName フォント名
		///@return 成功するとtrue
		bool SetCullentFont(const char* fontName);

		///@brief 指定座標からテキストの描画予約（位置はスクリーンの座標）
		///@param[in] text 描画するテキスト
		///@param[in] fontSize フォントのサイズ
		///@param[in] posX 描画座標
		///@param[in] posY 描画座標
		void DrawString2D(const std::string& text, int fontSize, int posX, int posY);
		///@brief 指定座標からテキストの描画予約（位置は3D空間の座標）
		///@param[in] text 描画するテキスト
		///@param[in] fontSize フォントのサイズ
		///@param[in] posX 描画座標
		///@param[in] posY 描画座標
		///@param[in] posZ 描画座標
		void DrawString3D(const std::string& text, int fontSize, float posX, float posY, float posZ);

		///@brief フォント描画に利用しているテクスチャを削除する
		///@param[in] size 削除するフォントのサイズ
		void ClearFontTexture(int size);
		///@brief フォント描画に利用しているテクスチャをすべて削除する
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
