#pragma once

#include<GLEW\glew.h>
#include"Texture.h"


namespace K_Graphics {

	//オフスクリーンレンダリングのためのフレームバッファクラス(テクスチャ)
	class Framebuffer {
	public:
		Framebuffer(Texture* texture, const std::string& name);
		Framebuffer(Texture* texture, const std::string& name, GLuint depthBuffer);
		~Framebuffer();
		bool Initialize(Texture* texture, const std::string& name, GLuint depthBuffer);
		void Finalize();
		void Bind();
		int GetWidth();
		int GetHeight();
		int GetFrameBufferHandle();
		int GetDepthBufferHandle();

	private:

	private:
		std::string TextureName;
		int textureWidth;
		int textureHeight;
		GLuint frameBuffer;
		GLuint depthBuffer;
		bool reUseDepth;
	};
}
