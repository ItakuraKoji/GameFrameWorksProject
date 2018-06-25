#pragma once
#include<vector>
#include<bullet\btBulletDynamicsCommon.h>
#include<GLEW\glew.h>
#include"K_Graphics\ShaderClass.h"

namespace K_Physics {
	struct DebugLine {
		btVector3 pos;
		btVector4 color;
	};
	struct DebugLineIndex {
		unsigned int from;
		unsigned int to;
	};

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

		void DrawAllLine();

	private:
		int debug_mode;
		std::vector<DebugLine> lineData;
		std::vector<DebugLineIndex> lineIndex;
		GLuint VAO;
		GLuint VBO;
		GLuint IBO;
	};
}
