#pragma once

#include<unordered_map>
#include"ShaderClass.h"

namespace K_Graphics {
	//シェーダークラスをリストとして持ち、解放責任をもつクラス
	class ShaderList {
	public:
		ShaderList();
		~ShaderList();
		void Initialize();

		ShaderClass* GetShader(const std::string& shaderName);
		ShaderClass* UseShader(const std::string& shaderName);

		void LoadVertexShader(const std::string& fileName);
		void LoadFragmentShader(const std::string& fileName);

		void CreateShaderProgram(const std::string& shaderName, const std::string& vertexShader, const std::string& fragmentShader);

	private:
		char* LoadTxtResource(const std::string& fileName);
		void ShowShaderErrors(GLuint shaderID);

	private:
		std::unordered_map<std::string, ShaderClass*> shaderList;
		std::unordered_map<std::string, GLuint> vertexShaders;
		std::unordered_map<std::string, GLuint> pixelShaders;

	};
}
