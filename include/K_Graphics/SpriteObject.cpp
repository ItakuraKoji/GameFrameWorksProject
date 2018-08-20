#include"MeshModel.h"

namespace K_Graphics {

	////////
	//public
	////

	SpriteObject::SpriteObject(Texture* texture, float controlPointX, float controlPointY) {
		this->drawModel = nullptr;

		if (!Initialize(texture, controlPointX, controlPointY)) {
			throw std::runtime_error("SpriteObjectError ModelData Initialize Failed");
		}
	}
	SpriteObject::~SpriteObject() {
		Finalize();
	}


	bool SpriteObject::Initialize(Texture* texture, float controlPointX, float controlPointY) {
		Finalize();

		if (!this->drawModel) {
			ModelDataFactory factory;
			MeshModel* model = new MeshModel(factory.CreateSquareModel(1.0f, 1.0f, texture, false));
			if (model == nullptr) {
				return false;
			}
			this->drawModel = model;
		}
		this->controlPoint = K_Math::Vector2(controlPointX, controlPointY);

		SetTexture(texture);
		return true;
	}
	void SpriteObject::Finalize() {
		if (this->drawModel != nullptr) {
			delete this->drawModel;
			this->drawModel = nullptr;
		}
	}

	bool SpriteObject::SetTexture(Texture* texture) {
		this->cullentTexture = texture;
		this->drawModel->SetTexture(texture, 0, 0);
		return true;
	}

	void SpriteObject::Draw2D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Box2D& draw, float rotation) {
		//���߂�L���A�J�����O��؂�
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//�ό`��ݒ�
		K_Math::Vector3 position(draw.x - camera->GetScreenWidth() / 2.0f, -(draw.y - camera->GetScreenHeight() / 2.0f), 0.0f);
		K_Math::Vector3 scale((float)draw.w, (float)draw.h, 1.0f);
		K_Math::Vector3 rot(0.0f, 0.0f, K_Math::DegToRad(rotation));
		K_Math::Vector2 control = this->controlPoint;

		//�V�F�[�_�[�ɍs���ݒ�
		SetMatrix(camera, shader, position, rot, scale, control, false);
		if (this->cullentTexture != nullptr) {
			shader->SetValue("textureSize", K_Math::Vector2(this->cullentTexture->GetWidth(), this->cullentTexture->GetHeight()));
		}
		//�e�N�X�`�����𑗂荞��
		shader->SetValue("spriteUV", K_Math::Vector4((float)src.x, (float)src.y, (float)src.w, (float)src.h));

		this->drawModel->Draw(shader);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void SpriteObject::Draw3D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale) {
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		SetMatrix(camera, shader, position, rotation, K_Math::Vector3(scale.x * (float)src.w, scale.y * (float)src.h, scale.z), this->controlPoint, true);
		if (this->cullentTexture != nullptr) {
			shader->SetValue("textureSize", K_Math::Vector2(this->cullentTexture->GetWidth(), this->cullentTexture->GetHeight()));
		}
		shader->SetValue("spriteUV", K_Math::Vector4((float)src.x, (float)src.y, (float)src.w, (float)src.h));
		this->drawModel->Draw(shader);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}

	K_Math::Matrix4x4 SpriteObject::CreateWorldMatrix(CameraClass* camera, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling, const K_Math::Vector2& controlPoint, bool billBoard) {
		K_Math::Matrix4x4 world;
		//�ړ�
		K_Math::Matrix4x4 transMat = glm::translate(world, position);
		//���_���R���g���[���|�C���g�Ɉړ�������i�g��O�ɍs���̂Ŏl�p�`��ӂ̒����ɍ��킹�� 0.0f ~ 1.0f �͈͂ɕ␳�j
		K_Math::Matrix4x4 controlTrans = glm::translate(world, K_Math::Vector3(-controlPoint.x / fabsf(scaling.x), controlPoint.y / fabsf(scaling.y), 0.0f));
		//��]����YXZ
		K_Math::Quaternion rot;
		rot = glm::angleAxis(0.0f, K_Math::Vector3(0.0f, 0.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.y, K_Math::Vector3(0.0f, 1.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.x, K_Math::Vector3(1.0f, 0.0f, 0.0f));
		rot = rot * glm::angleAxis(rotation.z, K_Math::Vector3(0.0f, 0.0f, 1.0f));
		K_Math::Matrix4x4 rotMat = glm::toMat4(rot);

		//�X�P�[��
		K_Math::Matrix4x4 scaleMat = glm::scale(world, scaling);

		K_Math::Matrix4x4 cameraMat;
		if (billBoard) {
			cameraMat = camera->GetCameraMatrix();
			cameraMat[3][0] = 0.0f;
			cameraMat[3][1] = 0.0f;
			cameraMat[3][2] = 0.0f;
		}

		world = transMat * cameraMat * rotMat * scaleMat * controlTrans;
		return world;
	}

	////////
	//private
	////

	void SpriteObject::SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling, const K_Math::Vector2& controlPoint, bool billBoard) {

		const K_Math::Matrix4x4& world = CreateWorldMatrix(camera, position, rotation, scaling, controlPoint, billBoard);

		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		shader->SetMatrix(projection * view * world);
	}

}
