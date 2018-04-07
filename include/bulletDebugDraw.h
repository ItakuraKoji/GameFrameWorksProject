#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<GLEW\glew.h>
#include"ShaderClass.h"

namespace K_Physics {
	//bulletデバッグ用描画クラス(OpenGL)
	//シェーダーや行列などは外から渡す
	class bulletDebugDraw : public btIDebugDraw {
	public:
		bulletDebugDraw();
		~bulletDebugDraw();

		//OverRide
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		void reportErrorWarning(const char* warningString);
		void draw3dText(const btVector3& location, const char* textString);
		void setDebugMode(int debugMode);
		int getDebugMode() const;

	private:
		int debug_mode;
		GLuint VAO;
		GLuint VBO;
		GLuint IBO;
	};
}
