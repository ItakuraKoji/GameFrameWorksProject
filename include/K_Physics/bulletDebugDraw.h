#pragma once
#include<vector>
#include<bullet\btBulletDynamicsCommon.h>
#include<GLEW\glew.h>
#include"K_Graphics\ShaderClass.h"
#include"K_Graphics\CameraClass.h"

namespace K_Physics {
	struct DebugLine {
		btVector3 from;
		btVector4 fromColor;
		btVector3 to;
		btVector4 toColor;
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

		void StartDraw(K_Graphics::CameraClass* camera);
		void DrawAllLine();

		//OverRide
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
		void reportErrorWarning(const char* warningString);
		void draw3dText(const btVector3& location, const char* textString);
		void setDebugMode(int debugMode);
		int getDebugMode() const;

	private:

	private:
		int debug_mode;
		std::vector<DebugLine> lineData;
		std::vector<DebugLineIndex> lineIndex;
		int indexCount;

		GLuint VAO;
		GLuint VBO;
		GLuint IBO;

		//カリング用のカメラ
		K_Graphics::CameraClass* camera;
	};
}
