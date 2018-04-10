#include"ModelData.h"


namespace K_Graphics {

	////////
	//public
	////
	VertexData::VertexData() {

	}
	VertexData::~VertexData() {
		//ƒoƒCƒ“ƒh‚ð‰ðœ
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		for (auto i : this->bufferArray) {
			glDeleteVertexArrays(1, &i.VAO);
			glDeleteBuffers(1, &i.VBO);
			for (int k = 0; k < i.numMaterial; ++k) {
				glDeleteBuffers(1, &i.IBOs[k]);
			}
		}
	}

	void VertexData::Add(VertexBuffer &buffer) {
		this->bufferArray.push_back(buffer);
	}

	int VertexData::GetNumBuffer() {
		return (int)this->bufferArray.size();
	}

	GLuint VertexData::GetVAO(int arrayIndex) {
		return this->bufferArray[arrayIndex].VAO;
	}
	GLuint VertexData::GetVBO(int arrayIndex) {
		return this->bufferArray[arrayIndex].VBO;
	}
	GLuint VertexData::GetIBO(int arrayIndex, int materialIndex) {
		return this->bufferArray[arrayIndex].IBOs[materialIndex];
	}
	int VertexData::GetNumFace(int arrayIndex) {
		return this->bufferArray[arrayIndex].numFace;
	}

}

