#include"MeshModel.h"

namespace K_Graphics {

	////////
	//public
	////

	SpriteObject::SpriteObject(Texture* texture, float controlPointX, float controlPointY) {
		this->drawModel = nullptr;

		if (!Initialize(texture, controlPointX, controlPointY)) {
			throw("SpriteObjectError ModelData Initialize Failed");
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
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		K_Math::Vector3 position(draw.x - camera->GetScreenWidth() / 2.0f, -(draw.y - camera->GetScreenHeight() / 2.0f), 0.0f);
		K_Math::Vector3 scale((float)draw.w, (float)draw.h, 1.0f);
		K_Math::Vector3 rot(0.0f, 0.0f, K_Math::DegToRad(rotation));
		SetMatrix(camera, shader, position, rot, scale, false);
		if (this->cullentTexture != nullptr) {
			shader->SetValue("textureSize", K_Math::Vector2(this->cullentTexture->GetWidth(), this->cullentTexture->GetHeight()));
		}
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
		
		SetMatrix(camera, shader, position, rotation, K_Math::Vector3(scale.x() * (float)src.w, scale.y() * (float)src.h, scale.z()), true);
		if (this->cullentTexture != nullptr) {
			shader->SetValue("textureSize", K_Math::Vector2(this->cullentTexture->GetWidth(), this->cullentTexture->GetHeight()));
		}
		shader->SetValue("spriteUV", K_Math::Vector4((float)src.x, (float)src.y, (float)src.w, (float)src.h));
		this->drawModel->Draw(shader);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}


	////////
	//protected
	////

	void SpriteObject::SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scaling, bool billBoard) {
		//ˆÚ“®
		K_Math::Translation trans = K_Math::Translation(position.x() + this->controlPoint.x(), position.y() - this->controlPoint.y(), position.z());
		K_Math::Translation controlTrans = K_Math::Translation(-this->controlPoint.x(), this->controlPoint.y(), 0.0f);
		//‰ñ“]‡‚ÍYXZ
		K_Math::Quaternion rot;
		rot = K_Math::AngleAxis(0, K_Math::Vector3::Zero());
		rot = rot * K_Math::AngleAxis(rotation.y(), K_Math::Vector3::UnitY());
		rot = rot * K_Math::AngleAxis(rotation.x(), K_Math::Vector3::UnitX());
		rot = rot * K_Math::AngleAxis(rotation.z(), K_Math::Vector3::UnitZ());
		//ƒXƒP[ƒ‹
		K_Math::DiagonalMatrix scale = K_Math::DiagonalMatrix(K_Math::Vector3(scaling.x(), scaling.y(), scaling.z()));

		K_Math::Matrix3x3 cameraMat;
		if (billBoard) {
			cameraMat = camera->GetCameraMatrix().block(0, 0, 3, 3);
		}
		else {
			cameraMat = K_Math::Matrix3x3::Identity();
		}

		K_Math::Affine3 world = trans * cameraMat * rot * controlTrans * scale;

		K_Math::Matrix4x4 view = camera->GetViewMatrix();
		K_Math::Matrix4x4 projection = camera->GetProjectionMatrix();

		shader->SetMatrix(projection * view * world.matrix());
	}

}
