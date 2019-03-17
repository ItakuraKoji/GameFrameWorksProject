#include"MeshModel.h"

namespace K_Graphics {

	////////
	//public
	////

	MeshObject::MeshObject(MeshModel* model) {
		this->drawModel = nullptr;

		if (!Initialize(model)) {
			throw std::runtime_error("modelData is nullptr");
		}
	}
	MeshObject::~MeshObject() {
		Finalize();
	}

	void MeshObject::GetAnimList(std::vector<std::string>& out){
		this->drawModel->GetAnimList(out);
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

	void MeshObject::SetTexture(Texture* texture, int hierarchyIndex, int materialIndex) {
		this->drawModel->SetTexture(texture, hierarchyIndex, materialIndex);
	}


	void MeshObject::UpdateAnimation(float timeSpeed) {
		this->drawModel->UpdateAnimation(timeSpeed);
	}

	void MeshObject::Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->Draw(shader);
	}
	void MeshObject::Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->Draw(shader);
	}
	void MeshObject::Draw(CameraClass * camera, ShaderClass * shader, const K_Math::Matrix4x4 & mat){
		//シェーダーにWVP行列とW行列をセット(クォータニオン回転)
		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		shader->SetMatrix(projection * view * mat);
		shader->SetWorldMatrix(mat);
		this->drawModel->Draw(shader);
	}
	void MeshObject::InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale) {
		SetMatrix(camera, shader, position, rotation, scale);
		this->drawModel->InstanceDraw(numDraw, shader);
	}

	////////
	//private
	////

	void MeshObject::SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling) {
		//シェーダーにWVP行列とW行列をセット(オイラー回転)
		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		K_Math::Matrix4x4 world;
		K_Math::CreateWorldMatrix(world, position, rotation, scaling);
		shader->SetMatrix(projection * view * world);
		shader->SetWorldMatrix(world);
	}
	void MeshObject::SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scaling) {
		//シェーダーにWVP行列とW行列をセット(クォータニオン回転)
		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		K_Math::Matrix4x4 world;
		K_Math::CreateWorldMatrix(world, position, rotation, scaling);
		shader->SetMatrix(projection * view * world);
		shader->SetWorldMatrix(world);
	}

}
