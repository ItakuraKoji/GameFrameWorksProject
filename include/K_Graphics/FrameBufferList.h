#pragma once

#include<unordered_map>
#include<string>
#include"FrameBuffer.h"
#include"TextureList.h"

namespace K_Graphics {

	//フレームバッファ生成＆管理クラス、テクスチャを扱うのでTextureListなどのテクスチャクラスとの連携必須
	class FrameBufferList {
	public:
		//!@brief テクスチャリストへのポインタを渡して初期化
		FrameBufferList(TextureList* list);
		//!@brief 終了処理をする
		~FrameBufferList();


		//!@briefテクスチャリストに新規作成を依頼し、フレームバッファ作成＆リスト登録
		//!深度バッファを使いまわすときはそのフレームバッファの名前を引数に渡す
		//!@param[in] name 作成するフレームバッファのユーザー定義名
		//!@param[in] width 作成するフレームバッファのビューポート幅
		//!@param[in] height 作成するフレームバッファのビューポート高さ
		bool CreateFrameBuffer(const std::string& name, int width, int height);
		bool CreateFrameBuffer(const std::string& name, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor);

		//!@briefテクスチャリストに新規作成を依頼し、フレームバッファ作成＆リスト登録
		//!深度バッファを使いまわすときはそのフレームバッファの名前を引数に渡す
		//!@param[in] name 作成するフレームバッファのユーザー定義名
		//!@param[in] depthBuffer 深度を使いまわすフレームバッファの名前
		//!@param[in] width 作成するフレームバッファのビューポート幅
		//!@param[in] height 作成するフレームバッファのビューポート高さ
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height);
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor);
		
		//!@brief 指定色でフレームバッファをクリアする
		//!@param[in] name クリアするフレームバッファの名前
		//!@param[in] r クリア色のR成分
		//!@param[in] g クリア色のG成分
		//!@param[in] b クリア色のB成分
		//!@param[in] notDeleteDepthStencil trueにすると深度とステンシルをクリアしない（省略時false）
		void BeginDraw(const std::string& name, float r, float g, float b, float a);
		
		//!@brief ビューポートを設定し、指定色でバックバッファをクリアする
		//!@param[in] viewPortWidth ビューポートの幅
		//!@param[in] viewPortHeight ビューポートの高さ
		//!@param[in] r クリア色のR成分
		//!@param[in] g クリア色のG成分
		//!@param[in] b クリア色のB成分
		//!@param[in] notDeleteDepthStencil trueにすると深度とステンシルをクリアしない（省略時false）
		void BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a);
		//!@brief バインドを解いて描画終了
		void EndDraw();

		//バッファ消去のフラグ設定
		void SetClearFlag(bool color, bool depth, bool stencil);

		Framebuffer* GetFrameBuffer(const std::string& name);

	private:
		void ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a);

	private:
		std::unordered_map<std::string, Framebuffer*> frameBuffers;

		//参照用、デストラクタなどで使う
		TextureList* list;

		//
		bool colorClear;
		bool depthClear;
		bool stencilClear;
	};
}
