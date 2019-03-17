#include"bulletDebugDraw.h"

namespace K_Physics {
	float Distance(const btVector3& l, const K_Math::Vector3& r) {
		return K_Math::Norm(K_Math::Vector3(l.x() - r.x, l.y() - r.y, l.z() - r.z));
	}




	////////
	//public
	////
	bulletDebugDraw::bulletDebugDraw() {
		glLineWidth(0.5f);

	}
	bulletDebugDraw::~bulletDebugDraw() {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
	}

	void bulletDebugDraw::StartDraw(K_Graphics::CameraClass* camera) {
		this->camera = camera;
		this->lineData.clear();
	}


	void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		//遠い奴を適当に弾く
		if (Distance(from, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
			if (Distance(to, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
				return;
			}
		}

		DebugLine line;
		line.from = from;
		line.to = to;
		line.fromColor = btVector4(color.x(), color.y(), color.z(), 1.0f);
		line.toColor = btVector4(color.x(), color.y(), color.z(), 1.0f);

		this->lineData.push_back(line);
	}


	void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) {
		//遠い奴を適当に弾く
		if (Distance(from, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
			if (Distance(to, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
				return;
			}
		}

		DebugLine line;
		line.from = from;
		line.to = to;
		line.fromColor = btVector4(fromColor.x(), fromColor.y(), fromColor.z(), 1.0f);
		line.toColor = btVector4(toColor.x(), toColor.y(), toColor.z(), 1.0f);

		this->lineData.push_back(line);
	}

	void bulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {
		//遠い奴を適当に弾く
		if (Distance(PointOnB, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
			if (Distance(PointOnB + normalOnB, this->camera->GetPosition()) > this->camera->GetScreenFar()) {
				return;
			}
		}

		DebugLine line;
		line.from = PointOnB;
		line.to = PointOnB + normalOnB;
		line.fromColor = btVector4(color.x(), color.y(), color.z(), 1.0f);
		line.toColor = btVector4(color.x(), color.y(), color.z(), 1.0f);

		this->lineData.push_back(line);
	}


	void bulletDebugDraw::reportErrorWarning(const char* warningString) {

		/* 警告表示 */
		printf("%s\n", warningString);
	}

	void bulletDebugDraw::draw3dText(const btVector3& location, const char* textString) {

		/* 指定空間座標に文字列表示 */

	}


	void bulletDebugDraw::setDebugMode(int debugMode) {

		/* デバッグモード指定 */
		this->debug_mode = debugMode;

	}

	int bulletDebugDraw::getDebugMode() const {

		/* 現在のデバッグモードを返却 */
		return this->debug_mode;
	}


	void bulletDebugDraw::DrawAllLine() {
		////全ての線をまとめて描画

		glDeleteBuffers(1, &this->VBO);
		glDeleteVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->lineData.size() * (sizeof(DebugLine)), this->lineData.data(), GL_DYNAMIC_DRAW);

		//pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(btVector3) + sizeof(btVector4), 0);
		//color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(btVector3) + sizeof(btVector4), (unsigned char*)NULL + (sizeof(btVector3)));
		glBindVertexArray(0);

		glBindVertexArray(this->VAO);
		glDrawArrays(GL_LINES, 0, (GLsizei)this->lineData.size() * 2);
		glBindVertexArray(0);
	}

}
