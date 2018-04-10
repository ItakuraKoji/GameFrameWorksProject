#pragma once

#include<unordered_map>
#include"ShaderClass.h"

namespace K_Graphics {
	///@brief シェーダークラスをリストとして持ち、解放責任をもつクラス
	class ShaderList {
	public:
		///@brief Initialize()を呼ぶ
		ShaderList();
		///@brief Initialize()を呼ぶ
		~ShaderList();
		///@brief リストのシェーダーを全て開放する
		void Initialize();

		///@brief シェーダーへのポインタを取得
		///@param[in] shaderName シェーダーの名前
		///@return 名前に対応したシェーダー
		ShaderClass* GetShader(const std::string& shaderName);
		///@brief シェーダーを描画用に設定し、そのシェーダーへのポインタを返す
		///@param[in] shaderName シェーダーの名前
		///@return 名前に対応したシェーダー
		ShaderClass* UseShader(const std::string& shaderName);

		///@brief 頂点シェーダーを読み込み、失敗するとstring型のエラーメッセージを例外で送出する
		///@param[in] fileName シェーダーのパス
		void LoadVertexShader(const std::string& fileName);
		///@brief フラグメントシェーダーを読み込み、失敗するとstring型のエラーメッセージを例外で送出する
		///@param[in] filename シェーダーのパス
		void LoadFragmentShader(const std::string& fileName);

		///@brief 頂点シェーダーとフラグメントシェーダーを組み合わせてシェーダーを作成
		///元となるシェーダーがない場合はstring型のエラーメッセージを例外で送出する
		///@param[in] shaderName 作成するシェーダーのユーザー定義名
		///@param[in] vertexShader 元となる頂点シェーダーへのパス（LoadVertexShader()の時にパスでシェーダー名が登録されている）
		///@param[in] vertexShader 元となるフラグメントシェーダーへのパス（LoadFragmentShader()の時にパスでシェーダー名が登録されている）
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
