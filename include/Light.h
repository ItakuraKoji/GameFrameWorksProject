#pragma once

#include<Eigen\Core>
#include"ShaderClass.h"
#include"MyMathFanctions.h"

namespace K_Graphics {

	//ライトの設定を記録するクラス群、正直同じクラスにまとめたほうがよさそう。環境光がないライトとかありえないし。

	class AmbientLight {
	public:
		AmbientLight();
		void SetLight(ShaderClass* shader);

		void SetPower(float power);
		void SetColor(float r, float g, float b, float a);

		float GetPower();
		const K_Math::Vector4& GetColor();

	protected:
		float    power;
		K_Math::Vector4 color;
	};

	class DirectionalLight {
	public:
		DirectionalLight();
		void SetLight(ShaderClass* shader);

		void SetPower(float power);
		void SetColor(float r, float g, float b, float a);

		float GetPower();
		const K_Math::Vector4& GetColor();

		void  SetDirection(float x, float y, float z);
		const K_Math::Vector3& GetDirection();

	protected:
		float    power;
		K_Math::Vector4 color;
		K_Math::Vector3 direction;
	};
}

