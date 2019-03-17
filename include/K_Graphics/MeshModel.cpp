#include"MeshModel.h"

namespace K_Graphics {
	////////
	//public
	////
	MeshModel::MeshModel(ModelDataSuper* data) {
		this->data = nullptr;
		this->boneTexture = nullptr;

		if (!Initialize(data)) {
			Finalize();
			throw std::runtime_error("モデルデータが不十分です、読み込みができているか確認してください");
		}
	}
	MeshModel::~MeshModel() {
		Finalize();
	}

	void MeshModel::GetAnimList(std::vector<std::string>& out){
		out.clear();
		if (this->data->bone == nullptr) {
			return;
		}
		//コピー
		out = std::vector<std::string>(this->data->bone->GetAnimList());
	}

	//初期化
	bool MeshModel::Initialize(ModelDataSuper* data) {
		Finalize();

		//頂点とマテリアルは必須
		if (!data->vertexBuffer && !data->material) {
			return false;
		}
		this->data = data;
		this->boneTexture = new Texture;
		this->boneTexture->Initialize();

		//ボーンテクスチャ初期化
		int numArray = this->data->vertexBuffer->GetNumBuffer();
		for (int i = 0; i < numArray; ++i) {
			UpdateBone(i);
		}
		return true;
	}

	//開放
	void MeshModel::Finalize() {
		if (this->data != nullptr) {
			//開放方法はデータの種類に依存
			delete this->data;
			this->data = nullptr;
		}
		if (this->boneTexture != nullptr) {
			delete this->boneTexture;
			this->boneTexture = nullptr;
		}
	}

	void MeshModel::SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames) {
		if (this->data->animation == nullptr) {
			return;
		}
		this->data->animation->SetAnimation(animationName, playOnce, isLoop, interpolationFrames);
	}
	void MeshModel::SetSpeed(float speed) {
		if (this->data->animation == nullptr) {
			return;
		}
		this->data->animation->SetSpeed(speed);
	}

	void MeshModel::SetTexture(Texture* texture, int arrayIndex, int materialIndex) {
		this->data->material->SetTexture(texture, arrayIndex, materialIndex);
	}

	void MeshModel::UpdateAnimation(float timeSpeed) {
		if (this->data->animation == nullptr) {
			return;
		}
		this->data->animation->UpdateAnimation(timeSpeed);

		int numArray = this->data->vertexBuffer->GetNumBuffer();
		for (int i = 0; i < numArray; ++i) {
			UpdateBone(i);
		}
	}

	//描画
	void MeshModel::Draw(ShaderClass* shader) {
		if (this->data->animation == nullptr) {
			shader->SetVertexShaderSubroutine("NotSkinning");
		}
		else {
			shader->SetVertexShaderSubroutine("CalcBoneMat");
		}

		int numArray = this->data->vertexBuffer->GetNumBuffer();
		for (int i = 0; i < numArray; ++i) {
			if (this->data->animation != nullptr) {
				SetBone(i, shader);
			}
			DrawBuffers(i, shader);
		}
	}

	//インスタシング描画(メッシュ階層の一番上の一つ目のマテリアルのみ)
	void MeshModel::InstanceDraw(int numInstance, ShaderClass* shader) {
		GLuint VAO = this->data->vertexBuffer->GetVAO(0);
		glBindVertexArray(VAO);

		Texture* texture = this->data->material->GetMaterial(0, 0).texture;
		if (texture != nullptr) {
			GLuint TextureID = texture->GetTextureID();
			shader->SetTexture("sampler", 0, TextureID);
		}
		GLuint IBO = this->data->vertexBuffer->GetIBO(0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElementsInstanced(GL_TRIANGLES, this->data->material->GetNumFace(0, 0) * 3, GL_UNSIGNED_INT, 0, numInstance);
	}


	////////
	//private
	////

	void MeshModel::UpdateBone(int hierarchyIndex) {
		if (this->data->animation == nullptr) {
			return;
		}
		//ボーンの再計算は1回だけ
		this->data->animation->SetMatrixTextureData(hierarchyIndex, this->boneTexture);
	}

	void MeshModel::SetBone(int hierarchyIndex, ShaderClass* shader) {
		shader->SetTexture("boneTex", 1, this->boneTexture->GetTextureID());
		shader->SetValue("numBone", this->data->animation->GetNumBone(hierarchyIndex));
	}

	//マテリアルごとに描画
	void MeshModel::DrawBuffers(int arrayIndex, ShaderClass* shader) {

		GLuint VAO = this->data->vertexBuffer->GetVAO(arrayIndex);
		glBindVertexArray(VAO);

		int numMaterial = this->data->material->GetNumMaterial(arrayIndex);
		for (int k = 0; k < numMaterial; ++k) {
			Material& material = this->data->material->GetMaterial(arrayIndex, k);
			Texture* texture = material.texture;
			if (texture) {
				GLuint TextureID = texture->GetTextureID();
				shader->SetTexture("sampler", 0, TextureID);
			}
			shader->SetValue("materialDiffuse", material.diffuse);
			shader->SetValue("materialSpecurar", material.specular);
			shader->SetValue("materialSpecurarShininess", material.specurarShininess);
			shader->SetValue("materialSpecurarPower", material.specurarPower);
			shader->SetValue("materialAmbient", material.ambient);
			shader->SetValue("materialAmbientPower", material.ambientPower);


			GLuint IBO = this->data->vertexBuffer->GetIBO(arrayIndex, k);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, this->data->material->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}

}
