#include"LightList.h"

namespace K_Graphics {

	LightList::LightList() {
		Initialize();
	}
	LightList::~LightList() {

	}

	void LightList::Initialize() {
		this->ambient.clear();
		this->directional.clear();
	}


	void LightList::SetAmbient(const std::string& lightName, ShaderClass* shader) {
		if (this->ambient.find(lightName) == this->ambient.end()) {
			throw std::runtime_error("LightError name is not exists");
		}
		this->ambient[lightName].SetLight(shader);
	}
	void LightList::SetDirectional(const std::string& lightName, ShaderClass* shader) {
		if (this->directional.find(lightName) == this->directional.end()) {
			throw std::runtime_error("LightError name is not exists");
		}
		this->directional[lightName].SetLight(shader);
	}

	AmbientLight& LightList::GetAmbient(const std::string& lightName) {
		if (this->ambient.find(lightName) == this->ambient.end()) {
			throw std::runtime_error("LightError name is not exists");
		}
		return this->ambient[lightName];
	}
	DirectionalLight& LightList::GetDirectional(const std::string& lightName) {
		if (this->directional.find(lightName) == this->directional.end()) {
			throw std::runtime_error("LightError name is not exists");
		}
		return this->directional[lightName];
	}

	void LightList::AddAmbient(const std::string& lightName, float power, const K_Math::Vector4& color) {
		AmbientLight addLight;
		addLight.SetPower(power);
		addLight.SetColor(color.x, color.y, color.z, color.w);
		this->ambient[lightName] = addLight;
	}
	void LightList::AddDirectional(const std::string& lightName, float power, const K_Math::Vector4& color, const K_Math::Vector3& direction) {
		DirectionalLight addLight;
		addLight.SetPower(power);
		addLight.SetColor(color.x, color.y, color.z, color.w);
		addLight.SetDirection(direction.x, direction.y, direction.z);
		this->directional[lightName] = addLight;
	}
}
