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
		//�������o�C���h
		glGenFramebuffers(1, &this->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

		//�[�x������������e�N�X�`��
		CreateDepthTextureBuffer(depthTexture);

		//�e�N�X�`���A����ɃJ���[������������
		CreateColorTextureBuffer(texture);

		//GPU����ł͂����ŃG���[���o��ꍇ������
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}


	bool Framebuffer::Initialize(Texture* texture, const std::string& name, GLuint depthBuffer) {
		//�������o�C���h
		glGenFramebuffers(1, &this->frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

		//�[�x�����������郌���_�[�o�b�t�@
		CreateDepthRenderBuffer(texture->GetWidth(), texture->GetHeight());

		//�e�N�X�`���A����ɃJ���[������������
		CreateColorTextureBuffer(texture);

		//GPU����ł͂����ŃG���[���o��ꍇ������
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
		//�[�x�o�b�t�@�̃����_�[�o�b�t�@�����݂���ꍇ�͍폜
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
		//�[�x�����������郌���_�[�o�b�t�@
		GLuint depthBufferID;
		glGenRenderbuffers(1, &depthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
		
		//�[�x���������ނ��Ƃ�錾
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		//�A�^�b�`
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

		this->depthBuffer = depthBufferID;
	}

	void Framebuffer::CreateDepthTextureBuffer(Texture* texture) {
		//�e�N�X�`���A����ɐ[�x����������
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->GetTextureID(), 0);
	}

	void Framebuffer::CreateColorTextureBuffer(Texture* texture){
		//�e�N�X�`���A����ɃJ���[������������
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);
	}

}
