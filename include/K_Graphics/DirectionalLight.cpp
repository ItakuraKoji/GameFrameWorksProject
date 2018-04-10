#include"Light.h"

namespace K_Graphics {
	////////
	//public
	////
	DirectionalLight::DirectionalLight() {
		this->power = 1.0f;
		this->color = K_Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		this->direction = K_Math::Vector3(0.0f, 0.0f, 1.0f);
	}

	void DirectionalLight::SetLight(ShaderClass* shader) {
		shader->SetDirectionalLight(this->power, this->color, this->direction);
		shader->SetValue("specularColor", K_Math::Vector4(1, 1, 1, 1));
		shader->SetValue("specularPower", 1.0f);
		shader->SetValue("specularLength", 20.0f);
	}

	void DirectionalLight::SetPower(float power) {
		this->power = power;
	}
	void DirectionalLight::SetColor(float r, float g, float b, float a) {
		this->color = K_Math::Vector4(r, g, b, a);
	}
	void DirectionalLight::SetDirection(float x, float y, float z) {
		this->direction = K_Math::Vector3(x, y, z);
	}

	float DirectionalLight::GetPower() {
		return this->power;
	}
	const K_Math::Vector4& DirectionalLight::GetColor() {
		return this->color;
	}
	const K_Math::Vector3& DirectionalLight::GetDirection() {
		return this->direction;
	}

}