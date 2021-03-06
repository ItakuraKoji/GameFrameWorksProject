#include"CameraList.h"

namespace K_Graphics {

	////////
	//public
	////
	CameraList::CameraList() {
		Initialize();
	}
	CameraList::~CameraList() {
		Initialize();
	}
	void CameraList::Initialize() {
		for (auto i : this->cameraList) {
			delete i.second;
		}
		this->cameraList.clear();
	}

	//�p�[�X�y�N�e�B�u���e
	void CameraList::AddPerspectiveCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far, float fieldOfView) {
		//���O���
		if (this->cameraList.find(name) != this->cameraList.end()) {
			throw std::runtime_error("CameraListError PerspectiveCamera name has already existed : " + name);
		}

		CameraClass* camera = new CameraClass(CameraType::Perspective, width, height, neer, far, fieldOfView);
		camera->SetPosition(position.x, position.y, position.z);
		camera->SetTarget(target.x, target.y, target.z);
		camera->Draw();
		this->cameraList[name] = camera;
	}

	//���s���e
	void CameraList::AddOrthoCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far) {
		//���O���
		if (this->cameraList.find(name) != this->cameraList.end()) {
			throw std::runtime_error("CameraListError OrthoCamera name has already existed : " + name);
		}

		CameraClass* camera = new CameraClass(CameraType::Ortho, width, height, neer, far, 0.0f);
		camera->SetPosition(position.x, position.y, position.z);
		camera->SetTarget(target.x, target.y, target.z);
		camera->Draw();
		this->cameraList[name] = camera;
	}

	CameraClass* CameraList::GetCamera(const std::string& name) {
		if (this->cameraList.find(name) == this->cameraList.end()) {
			throw std::runtime_error("CameraListError Camera name is not exists : " + name);
		}
		return this->cameraList[name];
	}
}
