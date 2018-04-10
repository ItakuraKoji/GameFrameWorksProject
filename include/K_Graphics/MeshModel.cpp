#include"MeshModel.h"

namespace K_Graphics {
	////////
	//public
	////
	MeshModel::MeshModel(ModelDatas* data) {
		this->data = nullptr;
		this->boneTexture = nullptr;

		if (!Initialize(data)) {
			Finalize();
			throw "モデルデータが不十分です、読み込みができているか確認してください";
		}
		this->isBoneProcessed = false;
	}
	MeshModel::~MeshModel() {
		Finalize();
	}

	//初期化
	bool MeshModel::Initialize(ModelDatas* data) {
		Finalize();

		//頂点とマテリアルは必須
		if (!data->vertexBuffer && !data->material) {
			return false;
		}
		this->data = data;
		this->boneTexture = new Texture;
		this->boneTexture->Initialize();
		return true;
	}

	//開放
	void MeshModel::Finalize() {
		if (this->data != nullptr) {
			delete this->data;
			this->data = nullptr;
		}
		if (this->boneTexture != nullptr) {
			delete this->boneTexture;
			this->boneTexture = nullptr;
		}
	}

	void MeshModel::SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames) {
		if (this->data->bone == nullptr) {
			return;
		}
		this->data->animation->SetAnimation(animationName, this->data->fbxData->GetScene(), playOnce, (bool)interpolationFrames, isLoop);
		if (this->data->animation->IsStartInterpolation()) {
			this->data->bone->StartInterporation(interpolationFrames);
		}

	}
	void MeshModel::SetSpeed(float speed) {
		if (this->data->bone == nullptr) {
			return;
		}
		this->data->animation->SetSpeed(speed);
	}

	void MeshModel::SetTexture(Texture* texture, int arrayIndex, int materialIndex) {
		//this->data->material->SetTextureName(textureName, arrayIndex, materialIndex);
		this->data->material->SetTexture(texture, arrayIndex, materialIndex);
	}

	void MeshModel::UpdateAnimation() {
		if (this->data->bone == nullptr) {
			return;
		}
		this->data->animation->UpdateAnimation();
		this->isBoneProcessed = false;
	}

	//描画
	void MeshModel::Draw(ShaderClass* shader) {
		if (this->data->bone == nullptr) {
			shader->SetVertexShaderSubroutine("NotSkinning");
		}
		else {
			shader->SetVertexShaderSubroutine("CalcBoneMat");
		}

		int numArray = this->data->vertexBuffer->GetNumBuffer();
		for (int i = 0; i < numArray; ++i) {
			if (this->data->bone != nullptr) {
				SetBone(i, shader);
			}
			DrawBuffers(i, shader);
		}
		this->isBoneProcessed = true;
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

	void MeshModel::SetBone(int arrayIndex, ShaderClass* shader) {
		//ボーンの再計算は1回だけ
		if (!this->isBoneProcessed) {
			int time = (int)this->data->animation->GetCurrentAnimTime();
			this->data->bone->SetClurrentBoneData(arrayIndex, time);
			this->data->bone->SetMatrixTextureData(arrayIndex, this->boneTexture);
		}
		shader->SetTexture("boneTex", 1, this->boneTexture->GetTextureID());
		shader->SetValue("numBone", this->data->bone->GetNumBone(arrayIndex));
	}

	//マテリアルごとに描画
	void MeshModel::DrawBuffers(int arrayIndex, ShaderClass* shader) {

		GLuint VAO = this->data->vertexBuffer->GetVAO(arrayIndex);
		glBindVertexArray(VAO);

		int numMaterial = this->data->material->GetNumMaterial(arrayIndex);
		for (int k = 0; k < numMaterial; ++k) {
			Texture* texture = this->data->material->GetMaterial(arrayIndex, k).texture;
			if (texture) {
				GLuint TextureID = texture->GetTextureID();
				shader->SetTexture("sampler", 0, TextureID);
			}

			GLuint IBO = this->data->vertexBuffer->GetIBO(arrayIndex, k);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, this->data->material->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
	}

}
