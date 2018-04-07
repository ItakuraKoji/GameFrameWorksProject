#pragma once

#include<unordered_map>
#include<string>
#include"FrameBuffer.h"
#include"TextureList.h"

namespace K_Graphics {

	//フレームバッファ生成＆管理クラス、テクスチャを扱うのでTextureListなどのテクスチャクラスとの連携必須
	class FrameBufferList {
	public:
		FrameBufferList(TextureList* list);
		~FrameBufferList();


		//テクスチャリストに新規作成を依頼し、フレームバッファ作成＆リスト登録
		//深度バッファを使いまわすときは使いまわすフレームバッファの名前を引数に渡す
		bool CreateFrameBuffer(const std::string& name, int width, int height);
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBuffer, int width, int height);
		//ビューポートを設定し、指定色で画面をクリアする
		void BeginDraw(const std::string& name, float r, float g, float b, bool notDeleteDepthStencil = false);
		//バックバッファへの書き込みを指定
		void BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil = false);
		//バインドを解いて描画終了
		void EndDraw();

	private:
		void ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil);

	private:
		std::unordered_map<std::string, Framebuffer*> frameBuffers;

		//参照用、デストラクタなどで使う
		TextureList* list;
	};
}
