#include"FrameBufferList.h"

namespace K_Graphics {

	////////
	//public
	////

	FrameBufferList::FrameBufferList(TextureList* list) {
		this->list = list;
		this->colorClear = true;
		this->depthClear = true;
		this->stencilClear = true;
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
			throw std::runtime_error("FrameBufferListError name has already existed");
		}
		this->list->AddEmptyTexture(name, width, height, TextureType::Float, TextureColorType::RGBA, TextureColorType::RGBA);

		Framebuffer* frame = new Framebuffer(this->list, this->list->GetTexture(name), name);
		this->frameBuffers[name] = frame;
		return true;
	}

	bool FrameBufferList::CreateFrameBuffer(const std::string& name, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor) {
		//テクスチャ生成失敗＆名前被りは失敗
		if (this->frameBuffers.find(name) != this->frameBuffers.end()) {
			throw std::runtime_error("FrameBufferListError name has already existed");
		}
		this->list->AddEmptyTexture(name, width, height, dataType, color, dataColor);

		Framebuffer* frame = new Framebuffer(this->list, this->list->GetTexture(name), name);
		this->frameBuffers[name] = frame;
		return true;
	}


	//使いまわす場合
	bool FrameBufferList::CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height) {
		//名前被りは失敗
		if (this->frameBuffers.find(name) != this->frameBuffers.end()) {
			throw std::runtime_error("FrameBufferListError name has already existed");
		}

		this->list->AddEmptyTexture(name, width, height);

		this->list->AddEmptyTexture(depthBufferTextureName, width, height, TextureType::Unsigned_Byte, TextureColorType::DepthComponent, TextureColorType::DepthComponent);

		Framebuffer* frame = new Framebuffer(this->list, this->list->GetTexture(name), this->list->GetTexture(depthBufferTextureName), name);
		this->frameBuffers[name] = frame;
		return true;
	}
	bool FrameBufferList::CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor) {
		//テクスチャ生成失敗＆名前被りは失敗
		if (this->frameBuffers.find(name) != this->frameBuffers.end()) {
			throw std::runtime_error("FrameBufferListError name has already existed");
		}

		this->list->AddEmptyTexture(name, width, height, dataType, color, dataColor);
		this->list->AddEmptyTexture(depthBufferTextureName, width, height);

		Framebuffer* frame = new Framebuffer(this->list, this->list->GetTexture(name), this->list->GetTexture(depthBufferTextureName), name);
		this->frameBuffers[name] = frame;
		return true;
	}


	void FrameBufferList::BeginDraw(const std::string& name, float r, float g, float b, float a) {
		if (this->frameBuffers.find(name) == this->frameBuffers.end()) {
			throw std::runtime_error("FrameBufferListError name is not exists");
		}
		this->frameBuffers[name]->Bind();
		ClearBuffer(this->frameBuffers[name]->GetWidth(), this->frameBuffers[name]->GetHeight(), r, g, b, a);
	}

	void FrameBufferList::BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearBuffer(viewPortWidth, viewPortHeight, r, g, b, a);
	}

	void FrameBufferList::EndDraw() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBufferList::SetClearFlag(bool color, bool depth, bool stencil){
		this->colorClear = color;
		this->depthClear = depth;
		this->stencilClear = stencil;
	}

	Framebuffer* FrameBufferList::GetFrameBuffer(const std::string& name) {
		if (this->frameBuffers.find(name) == this->frameBuffers.end()) {
			throw std::runtime_error("FrameBufferListError name is not exists");
		}
		return this->frameBuffers[name];
	}


	////////
	//private
	////

	void FrameBufferList::ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a) {
		int bitFrag = 0;

		//消去のフラグをチェック
		if (this->colorClear) {
			bitFrag |= GL_COLOR_BUFFER_BIT;
		}
		if (this->depthClear) {
			bitFrag |= GL_DEPTH_BUFFER_BIT;
		}
		if (this->stencilClear) {
			bitFrag |= GL_STENCIL_BUFFER_BIT;
		}

		glClearColor(r, g, b, a);
		glClear(bitFrag);
		glViewport(0, 0, viewPortWidth, viewPortHeight);
	}
}
