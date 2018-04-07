#pragma once

#include<unordered_map>
#include"Light.h"
#include"MyMathFanctions.h"

namespace K_Graphics {

	//光源情報をまとめたクラス、複数光源はコストが高いので数を絞る
	//アンビエント：１つ
	//その他：４つ（まだ予定）
	class LightList {
	public:
		LightList();
		~LightList();
		void Initialize();

		void SetAmbient(const std::string& lightName, ShaderClass* shader);
		void SetDirectional(const std::string& lightName, ShaderClass* shader);

		void AddAmbient(const std::string& lightName, float power, const K_Math::Vector4& color);
		void AddDirectional(const std::string& lightName, float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);

	private:
		std::unordered_map<std::string, AmbientLight> ambient;
		std::unordered_map<std::string, DirectionalLight> directional;
	};
}
