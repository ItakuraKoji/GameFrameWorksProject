#pragma once

#include<GLEW\glew.h>
#include"TextureList.h"

namespace K_Graphics {

	//オフスクリーンレンダリングのためのフレームバッファクラス(テクスチャ)
	class Framebuffer {
	public:
		Framebuffer(TextureList* textureList, Texture* texture, const std::string& name);
		Framebuffer(TextureList* textureList, Texture* texture, Texture* depthTexture, const std::string& name);
		~Framebuffer();
		bool Initialize(Texture* texture, const std::string& name, GLuint depthBuffer);
		bool Initialize(Texture* texture, Texture* depthTexture, const std::string& name);
		void Finalize();
		void Bind();
		int GetWidth();
		int GetHeight();
		int GetFrameBufferHandle();

	private:
		void CreateDepthRenderBuffer(unsigned int width, unsigned int height);
		void CreateDepthTextureBuffer(Texture* texture);


		void CreateColorTextureBuffer(Texture* texture);


	private:
		TextureList* textureList;

		std::string colorTexture;
		std::string depthTexture;
		int textureWidth;
		int textureHeight;
		GLuint frameBuffer;
		GLuint depthBuffer;
	};
}
