#include"ShaderClass.h"

namespace K_Graphics {
	////////
	//public
	////
	ShaderClass::ShaderClass(GLuint vertexShader, GLuint fragmentShader) {
		this->shaderProgram = 0;
		if (!Initialize(vertexShader, fragmentShader)) {
			Finalize();
			throw;
		}
	}
	ShaderClass::~ShaderClass() {
		Finalize();
	}


	bool ShaderClass::Initialize(GLuint vertexShader, GLuint fragmentShader) {
		Finalize();

		//シェーダーをくっつけるプログラムを生成
		shaderProgram = glCreateProgram();
		//シェーダーをアタッチ
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);


		//プログラムをリンク
		glLinkProgram(shaderProgram);

		//エラーチェック
		GLint status;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			ShowLinkErrors(shaderProgram);
			return false;
		}
		glUseProgram(shaderProgram);
		return true;
	}

	//開放
	void ShaderClass::Finalize() {
		glDeleteProgram(shaderProgram);
	}

	//UseProgram
	void ShaderClass::UseShader() {
		glUseProgram(this->shaderProgram);
	}

	//world * view * projection の合成行列を渡す
	void ShaderClass::SetMatrix(const K_Math::Matrix4x4& mat) {
		this->SetValue("matrixWVP", mat);
	}
	//ワールド変換行列だけを渡す
	void ShaderClass::SetWorldMatrix(const K_Math::Matrix4x4& world) {
		this->SetValue("worldMatrix", world);
	}
	//テクスチャリソースの番号を渡す
	void ShaderClass::SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber) {
		GLuint location;
		location = GetUniformLocation(uniformName);
		if (location == -1) {
			return;
		}
		glActiveTexture(GL_TEXTURE0 + textureLayer);
		glUniform1i(location, textureLayer);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
	}

	//ディレクショナルライト
	void ShaderClass::SetDirectionalLight(float power, const K_Math::Vector4& color, const K_Math::Vector3& direction) {
		SetValue("directionalPower", power);
		SetValue("directionalColor", color);
		SetValue("directionalDir", direction);
	}
	//環境光
	void ShaderClass::SetAmbientLight(float power, const K_Math::Vector4& color) {
		SetValue("ambientPower", power);
		SetValue("ambientColor", color);
	}

	//汎用受け渡し
	void ShaderClass::SetValue(const std::string& uniformName, int value) {
		GLint location = GetUniformLocation(uniformName);

		if (location != -1) {
			glUniform1i(location, value);
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, float value) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform1f(location, value);
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, const K_Math::Matrix4x4& value) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, const K_Math::Vector4& value) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform4fv(location, 1, glm::value_ptr(value));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, const K_Math::Vector3& value) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform3fv(location, 1, glm::value_ptr(value));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, const K_Math::Vector2& value) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform2fv(location, 1, glm::value_ptr(value));
		}
	}

	//汎用配列受け渡し
	void ShaderClass::SetValue(const std::string& uniformName, int* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform1iv(location, numArray, value);
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, float* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			glUniform1fv(location, numArray, value);
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, K_Math::Matrix4x4* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			//ベクトル型は配列みたいに要素がちゃんと順番に並んでる
			glUniformMatrix4fv(location, numArray, GL_FALSE, glm::value_ptr(value[0]));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, K_Math::Vector4* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			//ベクトル型は配列みたいに要素がちゃんと順番に並んでる
			glUniform4fv(location, numArray, glm::value_ptr(value[0]));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, K_Math::Vector3* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			//ベクトル型は配列みたいに要素がちゃんと順番に並んでる
			glUniform3fv(location, numArray, glm::value_ptr(value[0]));
		}
	}
	void ShaderClass::SetValue(const std::string& uniformName, K_Math::Vector2* value, int numArray) {
		GLint location = GetUniformLocation(uniformName);
		if (location != -1) {
			//ベクトル型は配列みたいに要素がちゃんと順番に並んでる
			glUniform2fv(location, numArray, glm::value_ptr(value[0]));
		}
	}

	//動的シェーダー設定
	void ShaderClass::SetVertexShaderSubroutine(const std::string& subroutineFunctionName) {
		GLuint index = GetVertexSubroutine(subroutineFunctionName);
		if (index != GL_INVALID_INDEX) {
			glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &index);
		}
	}
	void ShaderClass::SetFragmentShaderSubroutine(const std::string& subroutineFunctionName) {
		GLuint index = GetPixelSubroutine(subroutineFunctionName);
		if (index != GL_INVALID_INDEX) {
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);
		}
	}

	////////
	//private
	////

	GLint ShaderClass::GetUniformLocation(const std::string& name) {
		GLint location;
		if (this->uniformLocation.find(name) == this->uniformLocation.end()) {
			//記録がない場合は、新しく登録
			location = glGetUniformLocation(this->shaderProgram, name.data());
			this->uniformLocation[name] = location;
		}
		else {
			//記録がある場合はそれを使う
			location = this->uniformLocation[name];
		}
		return location;
	}
	//頂点シェーダーのサブルーチンを獲得
	GLuint ShaderClass::GetVertexSubroutine(const std::string& name) {
		GLuint location;
		if (this->vertexSubroutine.find(name) == this->vertexSubroutine.end()) {
			//記録がない場合は、新しく登録
			location = glGetSubroutineIndex(this->shaderProgram, GL_VERTEX_SHADER, name.data());
			this->vertexSubroutine[name] = location;
		}
		else {
			//記録がある場合はそれを使う
			location = this->vertexSubroutine[name];
		}
		return location;
	}
	//ピクセルシェーダーのサブルーチンを獲得
	GLuint ShaderClass::GetPixelSubroutine(const std::string& name) {
		GLuint location;
		if (this->pixelSubroutine.find(name) == this->pixelSubroutine.end()) {
			//記録がない場合は、新しく登録
			location = glGetSubroutineIndex(this->shaderProgram, GL_FRAGMENT_SHADER, name.data());
			this->pixelSubroutine[name] = location;
		}
		else {
			//記録がある場合はそれを使う
			location = this->pixelSubroutine[name];
		}
		return location;
	}


	//シェーダーのテキストファイルを読み込んで文字列を格納したバッファの先頭ポインタを返す
	char* ShaderClass::LoadTxtResource(const std::string& fileName) {
		std::ifstream fin;
		char* returnSource;
		char input;
		int fileSize;

		fin.open(fileName);

		if (fin.fail()) {
			return 0;
		}

		fileSize = 0;

		fin.get();
		while (!fin.eof()) {
			fin.get(input);
			++fileSize;
		}
		fin.close();

		returnSource = new char[fileSize + 1];
		if (returnSource == nullptr) {
			return 0;
		}

		fin.open(fileName);
		fin.read(returnSource, fileSize);
		fin.close();


		returnSource[fileSize] = '\0';

		return returnSource;
	}

	//エラーメッセージをコンソールへ出力
	void ShaderClass::ShowShaderErrors(GLuint shaderID) {
		GLint length;
		char* errorLog;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		errorLog = new char[length + 1];

		glGetShaderInfoLog(shaderID, length, NULL, errorLog);

		for (int i = 0; i < length; ++i) {
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
	}
	//エラーメッセージをコンソールへ出力
	void ShaderClass::ShowLinkErrors(GLuint shaderID) {
		GLint length;
		char* errorLog;

		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		errorLog = new char[length + 1];

		glGetProgramInfoLog(shaderID, length, NULL, errorLog);

		for (int i = 0; i < length; ++i) {
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
	}
}
