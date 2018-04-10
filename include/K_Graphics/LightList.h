#pragma once

#include<unordered_map>
#include"Light.h"
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	///@brief 光源情報をまとめたクラス、複数光源はコストが高いので数を絞る
	///アンビエント：１つ
	///その他：４つ（まだ予定）
	class LightList {
	public:
		///@brief Initialize()を呼ぶ
		LightList();
		///@brief Initialize()を呼ぶ
		~LightList();
		///@brief リスト初期化
		void Initialize();

		///@brief シェーダーにアンビエントライト情報を渡す
		///@param[in] lightName アンビエントライトの名前
		///@param[in] shader 情報を渡すシェーダーへのポインタ
		void SetAmbient(const std::string& lightName, ShaderClass* shader);
		///@brief シェーダーにディレクショナルライト情報を渡す
		///@param[in] lightName ディレクショナルライトの名前
		///@param[in] shader 情報を渡すシェーダーへのポインタ
		void SetDirectional(const std::string& lightName, ShaderClass* shader);

		///@brief 新しいアンビエントライトを追加
		///@param[in] lightName アンビエントライトのユーザー定義名
		///@param[in] power 光の強さ
		///@param[in] color 光の色
		void AddAmbient(const std::string& lightName, float power, const K_Math::Vector4& color);
		///@brief 新しいディレクショナルライトを追加
		///@param[in] lightName ディレクショナルライトのユーザー定義名
		///@param[in] power 光の強さ
		///@param[in] color 光の色
		///@param[in] direction 光の方向
		void AddDirectional(const std::string& lightName, float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);

	private:
		std::unordered_map<std::string, AmbientLight> ambient;
		std::unordered_map<std::string, DirectionalLight> directional;
	};
}
