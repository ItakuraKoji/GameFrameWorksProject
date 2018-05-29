#include"MeshModel.h"

namespace K_Graphics {

	////////
	//public
	////

	MeshObject::MeshObject(MeshModel* model) {
		this->drawModel = nullptr;

		if (!Initialize(model)) {
			throw("modelData is nullptr");
		}
	}
	MeshObject::~MeshObject() {
		Finalize();
	}

	bool MeshObject::Initialize(MeshModel* model) {
		Finalize();
		if (model == nullptr) {
			return false;
		}
		this->drawModel = model;
		return true;
	}
	void MeshObject::Finalize() {
		if (this->drawModel != nullptr) {
			delete this->drawModel;
			this->drawModel = nullptr;
		}
	}

	void MeshObject::SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames) {
		this->drawModel->SetAnimation(animationName, playOnce, isLoop, interpolationFrames);
	}
	void MeshObject::SetSpeed(float speed) {
		this->drawModel->SetSpeed(speed);
	}

	void MeshObject::UpdateAnimation() {
		this->drawModel->UpdateAnimation();
	}

	void MeshObject::Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->Draw(shader);
	}
	void MeshObject::InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->InstanceDraw(numDraw, shader);
	}

	K_Math::Matrix4x4 MeshObject::CreateWorldMatrix(const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling) {
		K_Math::Matrix4x4 world;
		//ˆÚ“®
		K_Math::Matrix4x4 transMat = glm::translate(world, position);
		//‰ñ“]‡‚ÍYXZ
		K_Math::Quaternion rot;
		rot = glm::angleAxis(0.0f, K_Math::Vector3(0.0f, 0.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.y, K_Math::Vector3(0.0f, 1.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.x, K_Math::Vector3(1.0f, 0.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.z, K_Math::Vector3(0.0f, 0.0f, 1.0f));
		K_Math::Matrix4x4 rotMat = glm::toMat4(rot);

		//ƒXƒP[ƒ‹
		K_Math::Matrix4x4 scaleMat = glm::scale(world, K_Math::Vector3(scaling.x, scaling.y, scaling.z));

		world = transMat * rotMat * scaleMat;
		return world;
	}


	////////
	//protected
	////

	void MeshObject::SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling) {

		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		K_Math::Matrix4x4& world = CreateWorldMatrix(position, rotation, scaling);
		shader->SetMatrix(projection * view * world);
		shader->SetWorldMatrix(world);
	}

}
