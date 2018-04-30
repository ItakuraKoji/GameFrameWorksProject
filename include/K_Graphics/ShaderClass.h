#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	//!@brief シェーダープログラムの管理とuniform変数の受け渡しを担当するクラス
	//!uniform変数名は固定的な機能を除くと、関数化にキリがないので汎用的な関数を用意
	class ShaderClass {
	public:
		//!@brief Initialize()を呼ぶ
		ShaderClass(GLuint vertexShader, GLuint fragmentShader);
		//!@brief Finalize()を呼ぶ
		~ShaderClass();

		//!@brief GLSLシェーダープログラムを作成
		//!@param[in] vertexShader 頂点シェーダーのインデックス
		//!@param[in] fragmentShader フラグメントシェーダーのインデックス
		//!@return 成功するとtrue
		bool Initialize(GLuint vertexShader, GLuint fragmentShader);
		//!@brief シェーダーを開放する
		void Finalize();

		//!@brief シェーダーを描画用に設定
		void UseShader();
		//!@brief シェーダーに ワールド x ビュー x プロジェクション の行列を渡す
		//!@param[in] mat 渡す変換行列
		void SetMatrix(const K_Math::Matrix4x4& mat);
		//!@brief シェーダーにワールド行列だけを渡す
		//!@param[in] mat 渡す変換行列
		void SetWorldMatrix(const K_Math::Matrix4x4& world);
		//!@brief シェーダーにテクスチャを渡す、モデル描画時に以下の番号が使われることがある
		//!レイヤー０番：モデルのマテリアルテクスチャ
		//!レイヤー１番：モデルのボーン行列テクスチャ
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] textureLayer テクスチャのレイヤー、複数枚のテクスチャを渡すときは数字を変える
		//!@param[in] textureNumber テクスチャのインデックス
		void SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber);
		//!@brief ディレクショナルライト情報を渡す
		//!@param[in] power 光の強さ
		//!@param[in] color 光の色
		//!@param[in] direction 光の向き
		void SetDirectionalLight(float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);
		//!@brief アンビエントライト情報を渡す
		//!@param[in] power 光の強さ
		//!@param[in] color 光の色
		void SetAmbientLight(float power, const K_Math::Vector4& color);

		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, int value);
		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, float value);
		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, const K_Math::Matrix4x4& value);
		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, const K_Math::Vector4& value);
		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, const K_Math::Vector3& value);
		//!@brief 汎用的なuniform変数への受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す値
		void SetValue(const std::string& uniformName, const K_Math::Vector2& value);

		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, int* value, int numArray);
		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, float* value, int numArray);
		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, K_Math::Matrix4x4* value, int numArray);
		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, K_Math::Vector4* value, int numArray);
		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, K_Math::Vector3* value, int numArray);
		//!@brief 汎用的なuniform変数への配列の受け渡し
		//!@param[in] uniformName GLSLのテクスチャを指すUniform変数の名前
		//!@param[in] value 渡す配列
		//!@param[in] numArray 配列の要素数
		void SetValue(const std::string& uniformName, K_Math::Vector2* value, int numArray);

		//!@brief subroutineの設定(vertex shader)、指定した名前の関数が選択される
		//!@param subroutineFunctionName 使用するサブルーチン関数の名前
		void SetVertexShaderSubroutine(const std::string& subroutineFunctionName);
		//!@brief subroutineの設定(fragment shader)、指定した名前の関数が選択される
		//!@param subroutineFunctionName 使用するサブルーチン関数の名前
		void SetFragmentShaderSubroutine(const std::string& subroutineFunctionName);

	private:
		char* LoadTxtResource(const std::string& fileName);
		void ShowShaderErrors(GLuint shaderID);
		void ShowLinkErrors(GLuint shaderID);

		GLuint shaderProgram;
	};
}
