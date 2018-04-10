#include"Light.h"

namespace K_Graphics {
	////////
	//public
	////
	AmbientLight::AmbientLight() {
		this->power = 1.0f;
		this->color = K_Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	void AmbientLight::SetLight(ShaderClass* shader) {
		shader->SetAmbientLight(this->power, this->color);
	}
	void AmbientLight::SetPower(float power) {
		this->power = power;
	}
	void AmbientLight::SetColor(float r, float g, float b, float a) {
		this->color = K_Math::Vector4(r, g, b, a);
	}
	float AmbientLight::GetPower() {
		return this->power;
	}
	const K_Math::Vector4& AmbientLight::GetColor() {
		return this->color;
	}
}
