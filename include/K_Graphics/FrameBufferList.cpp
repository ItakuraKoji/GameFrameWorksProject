#include"FrameBufferList.h"

namespace K_Graphics {

	////////
	//public
	////

	FrameBufferList::FrameBufferList(TextureList* list) {
		this->list = list;
	}
	FrameBufferList::~FrameBufferList() {
		for (auto i : this->frameBuffers) {
			this->list->DeleteTexture(i.first);
			delete i.second;
		}
	}

	bool FrameBufferList::CreateFrameBuffer(const std::string& name, int width, int height) {
		//テクスチャ生成失敗＆名前被りは失敗
		if (this->frameBuffers.find(name) != this->frameBuffers.end()) {
			return false;
		}
		if (!this->list->AddEmptyTexture(name, width, height)) {
			return false;
		}

		Framebuffer* frame = new Framebuffer(this->list->GetTexture(name), name);
		this->frameBuffers[name] = frame;
		return true;
	}

	bool FrameBufferList::CreateFrameBuffer(const std::string& name, const std::string& depthBuffer, int width, int height) {
		//テクスチャ生成失敗＆名前被りは失敗
		if (this->frameBuffers.find(name) != this->frameBuffers.end()) {
			return false;
		}
		//使いまわすフレームバッファが見つからないのも失敗
		if (this->frameBuffers.find(depthBuffer) == this->frameBuffers.end()) {
			return false;
		}
		if (!this->list->AddEmptyTexture(name, width, height)) {
			return false;
		}

		Framebuffer* frame = new Framebuffer(this->list->GetTexture(name), name, this->frameBuffers[depthBuffer]->GetDepthBufferHandle());
		this->frameBuffers[name] = frame;
		return true;
	}


	void FrameBufferList::BeginDraw(const std::string& name, float r, float g, float b, bool notDeleteDepthStencil) {
		if (this->frameBuffers.find(name) == this->frameBuffers.end()) {
			return;
		}
		this->frameBuffers[name]->Bind();
		ClearBuffer(this->frameBuffers[name]->GetWidth(), this->frameBuffers[name]->GetHeight(), r, g, b, notDeleteDepthStencil);
	}
	void FrameBufferList::BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearBuffer(viewPortWidth, viewPortHeight, r, g, b, notDeleteDepthStencil);
	}


	void FrameBufferList::EndDraw() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	////////
	//private
	////

	void FrameBufferList::ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, bool notDeleteDepthStencil) {
		int bitFrag;
		if (notDeleteDepthStencil) {
			bitFrag = GL_COLOR_BUFFER_BIT;
		}
		else {
			bitFrag = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		}
		glClearColor(r, g, b, 1.0f);
		glClear(bitFrag);
		glViewport(0, 0, viewPortWidth, viewPortHeight);
	}
}
