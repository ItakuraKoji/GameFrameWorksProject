#include"FrameBuffer.h"

namespace K_Graphics {

	////////
	//public
	////
	Framebuffer::Framebuffer(TextureList* textureList, Texture* texture, const std::string& name) {
		this->textureList = textureList;
		this->depthBuffer = 0;
		this->colorTexture = name;
		this->textureWidth = texture->GetWidth();
		this->textureHeight = texture->GetHeight();

		Initialize(texture, name, this->depthBuffer);
	}
	Framebuffer::Framebuffer(TextureList* textureList, Texture* texture, Texture* depthTexture, const std::string& name) {
		this->textureList = textureList;
		this->depthBuffer = 0;
		this->colorTexture = name;
		this->textureWidth = texture->GetWidth();
		this->textureHeight = texture->GetHeight();

		Initialize(texture, depthTexture, name);
	}
	Framebuffer::~Framebuffer() {
		Finalize();
	}

	bool Framebuffer::Initialize(Texture* texture, Texture* depthTexture, const std::string& name) {
		//生成しバインド
		glGenFramebuffers(1, &this->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

		//深度情報を持たせるテクスチャ
		CreateDepthTextureBuffer(depthTexture);

		//テクスチャ、これにカラー情報を持たせる
		CreateColorTextureBuffer(texture);

		//GPU次第ではここでエラーが出る場合がある
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}


	bool Framebuffer::Initialize(Texture* texture, const std::string& name, GLuint depthBuffer) {
		//生成しバインド
		glGenFramebuffers(1, &this->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

		//深度情報を持たせるレンダーバッファ
		CreateDepthRenderBuffer(texture->GetWidth(), texture->GetHeight());

		//テクスチャ、これにカラー情報を持たせる
		CreateColorTextureBuffer(texture);

		//GPU次第ではここでエラーが出る場合がある
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}


	void Framebuffer::Finalize() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &this->frameBuffer);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//深度バッファのレンダーバッファが存在する場合は削除
		if (this->depthBuffer != 0) {
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



	////////
	//private
	////
	void Framebuffer::CreateDepthRenderBuffer(unsigned int width, unsigned int height){
		//深度情報を持たせるレンダーバッファ
		GLuint depthBufferID;
		glGenRenderbuffers(1, &depthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
		
		//深度を書き込むことを宣言
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		//アタッチ
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

		this->depthBuffer = depthBufferID;
	}

	void Framebuffer::CreateDepthTextureBuffer(Texture* texture) {
		//テクスチャ、これに深度を書き込む
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->GetTextureID(), 0);
	}

	void Framebuffer::CreateColorTextureBuffer(Texture* texture){
		//テクスチャ、これにカラー情報を持たせる
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);
	}

}
