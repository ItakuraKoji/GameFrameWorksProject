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

	//パースペクティブ投影
	void CameraList::AddPerspectiveCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far, float fieldOfView) {
		//名前被り
		if (this->cameraList.find(name) != this->cameraList.end()) {
			return;
		}

		CameraClass* camera = new CameraClass(CameraType::Perspective, width, height, neer, far, fieldOfView);
		camera->SetPosition(position.x(), position.y(), position.z());
		camera->SetTarget(target.x(), target.y(), target.z());
		camera->Draw();
		this->cameraList[name] = camera;
	}

	//平行投影
	void CameraList::AddOrthoCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int width, int height, float neer, float far) {
		//名前被り
		if (this->cameraList.find(name) != this->cameraList.end()) {
			return;
		}

		CameraClass* camera = new CameraClass(CameraType::Ortho, width, height, neer, far, 0.0f);
		camera->SetPosition(position.x(), position.y(), position.z());
		camera->SetTarget(target.x(), target.y(), target.z());
		camera->Draw();
		this->cameraList[name] = camera;
	}

	CameraClass* CameraList::GetCamera(const std::string& name) {
		if (this->cameraList.find(name) == this->cameraList.end()) {
			return nullptr;
		}
		return this->cameraList[name];
	}
}
