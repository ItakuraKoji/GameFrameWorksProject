#include"FrameBuffer.h"

namespace K_Graphics {

	////////
	//public
	////
	Framebuffer::Framebuffer(Texture* texture, const std::string& name) {
		glGenRenderbuffers(1, &this->depthBuffer);
		Initialize(texture, name, this->depthBuffer);
		this->reUseDepth = false;

	}
	Framebuffer::Framebuffer(Texture* texture, const std::string& name, GLuint depthBuffer) {
		Initialize(texture, name, depthBuffer);
		this->reUseDepth = true;
	}
	Framebuffer::~Framebuffer() {
		Finalize();
	}

	bool Framebuffer::Initialize(Texture* texture, const std::string& name, GLuint depthBuffer) {
		//生成しバインド
		glGenFramebuffers(1, &this->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

		//深度情報を持たせるレンダーバッファ
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texture->GetWidth(), texture->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		//テクスチャ、これにカラー情報を持たせる
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);

		//GPU次第ではここでエラーが出る場合がある
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->TextureName = name;
		this->textureWidth = texture->GetWidth();
		this->textureHeight = texture->GetHeight();

		return true;
	}

	void Framebuffer::Finalize() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &this->frameBuffer);

		//深度バッファは使いまわしていない場合のみ削除
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		if (!this->reUseDepth) {
			glDeleteRenderbuffers(1, &this->depthBuffer);
		}
	}

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
	}


	int Framebuffer::GetWidth() {
		return this->textureWidth;
	}
	int Framebuffer::GetHeight() {
		return this->textureHeight;
	}
	int Framebuffer::GetFrameBufferHandle() {
		return this->frameBuffer;
	}
	int Framebuffer::GetDepthBufferHandle() {
		return this->depthBuffer;
	}

}
