#include "K3MDLoader.h"

namespace K_Loader {

	/////////
	//public
	////
	K3MDLoader::K3MDLoader(){
		this->bufferData = nullptr;
		this->materialData = nullptr;
		this->boneData = nullptr;
	}

	K3MDLoader::~K3MDLoader(){
		if (this->bufferData != nullptr) {
			delete this->bufferData;
			this->bufferData = nullptr;
		}
		if (this->materialData != nullptr) {
			delete this->materialData;
			this->materialData = nullptr;
		}
		if (this->boneData != nullptr) {
			delete this->boneData;
			this->boneData = nullptr;
		}
	}
	void K3MDLoader::CreateBinaryFile(K3MDHierarchy* data, const std::string& outputFilePath) {
		//K3MD形式の3Dモデルデータを吐き出す
		std::ofstream ofs(outputFilePath, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!ofs) {
			throw std::runtime_error("fileOpenFailed : " + outputFilePath);
		}
		//階層数
		int numHierarchy = (int)data->modelHierarchy.size();
		ofs.write((char*)&numHierarchy, sizeof(int));
		//アニメーション
		int numAnim = (int)data->animNameList.size();
		ofs.write((char*)&numAnim, sizeof(int));
		for (int i = 0; i < numAnim; ++i) {
			int numString = (int)data->animNameList[i].size();
			//アニメ名
			ofs.write((char*)&numString, sizeof(int));
			ofs.write((char*)data->animNameList[i].data(), sizeof(char) * numString);
			//開始時間、終了時間
			ofs.write((char*)&data->animList[data->animNameList[i]].startTime, sizeof(int));
			ofs.write((char*)&data->animList[data->animNameList[i]].endTime, sizeof(int));
		}


		for (auto& modelData : data->modelHierarchy) {
			//頂点情報
			int numVertex = (int)modelData.vertexData.size();
			ofs.write((char*)&numVertex, sizeof(int));
			ofs.write((char*)modelData.vertexData.data(), sizeof(K_Graphics::Vertex) * numVertex);
			//マテリアル
			int numMaterial = (int)modelData.materialData.size();
			ofs.write((char*)&numMaterial, sizeof(int));
			for (int i = 0; i < numMaterial; ++i) {
				int numIndex = (int)modelData.materialData[i].vertexIndex.size();
				ofs.write((char*)&numIndex, sizeof(int));
				ofs.write((char*)modelData.materialData[i].vertexIndex.data(), sizeof(unsigned int) * numIndex);

				//マテリアル名とテクスチャのパス
				int numString = (int)modelData.materialData[i].materialName.size();
				ofs.write((char*)&numString, sizeof(int));
				ofs.write((char*)modelData.materialData[i].materialName.data(), sizeof(char) * numString);
				numString = (int)modelData.materialData[i].texturePath.size();
				ofs.write((char*)&numString, sizeof(int));
				ofs.write((char*)modelData.materialData[i].texturePath.data(), sizeof(char) * numString);

				//マテリアル情報
				ofs.write((char*)&modelData.materialData[i].diffuse, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].ambient, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].ambientPower, sizeof(float));
				ofs.write((char*)&modelData.materialData[i].specular, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].specurarShininess, sizeof(float));
				ofs.write((char*)&modelData.materialData[i].specurarPower, sizeof(float));

			}

			//ボーン
			int numBone = (int)modelData.boneData.size();
			ofs.write((char*)&numBone, sizeof(int));
			for (int j = 0; j < numBone; ++j) {
				//バインドボーン
				ofs.write((char*)&modelData.boneData[j].bindMat, sizeof(K3MDMat));
				//アニメ数でループ
				for (int animIndex = 0; animIndex < numAnim; ++animIndex) {
					int startTime = data->animList[data->animNameList[animIndex]].startTime;
					int endTime = data->animList[data->animNameList[animIndex]].endTime;
					modelData.boneData[j].mat[animIndex].resize(startTime + endTime);

					//アニメーションの行列をアニメフレーム数だけ取得
					for (int frame = startTime; frame < endTime; ++frame) {
						ofs.write((char*)&modelData.boneData[j].mat[animIndex][frame], sizeof(K3MDMat));
					}
				}
			}

		}
		ofs.close();
	}

	void K3MDLoader::CreateBinaryFile(K3MDHierarchy* data, const std::string& fileName, const std::string& outputFilePath){
		//K3MD形式の3Dモデルデータを吐き出す
		CreateBinaryFile(data, outputFilePath + "/" + fileName + ".k3md");
	}

	K_Graphics::ModelResource* K3MDLoader::LoadModel(const char* filepath, K_Graphics::TextureList* list){
		K_Graphics::ModelResource* resource = nullptr;
		K3MDHierarchy* data = nullptr;
		try {
			//構造体を作成して、そこからモデル作成
			this->bufferData = nullptr;
			this->boneData = nullptr;
			this->materialData = nullptr;

			data = LoadK3MDHierarchy(filepath);

			resource = LoadModel(data, list);
			delete data;
		}
		catch (std::exception& e) {
			//例外処理
			if (resource != nullptr) {
				delete resource;
			}
			if (data != nullptr) {
				delete data;
			}
			throw e;
		}
		return resource;

	}


	K_Graphics::ModelResource* K3MDLoader::LoadModel(K3MDHierarchy* data, K_Graphics::TextureList* list) {
		if (data == nullptr) {
			throw std::runtime_error("K3MDHierarchy data is nullptr : K3MDLoader::LoadModel()");
		}


		//構造体からモデル作成
		this->bufferData = nullptr;
		this->boneData = nullptr;
		this->materialData = nullptr;
		printf("load from K3MD\n");
		K_Graphics::ModelResource* resource = new K_Graphics::ModelResource;

		try {
			this->textureList = list;

			//モデルデータを階層一つずつ読み込む
			for (auto modelData : data->modelHierarchy) {
				GLuint VAO;
				glGenVertexArrays(1, &VAO);
				glBindVertexArray(VAO);


				std::vector<GLuint> IBOs;
				CreateMaterial(data->modelPath, &modelData, IBOs);
				CreateBone(data, &modelData);
				CreateVertex(&modelData, VAO, IBOs);
			}
			resource->vertexBuffer = this->bufferData;
			resource->material = this->materialData;
			resource->bone = this->boneData;

			this->bufferData = nullptr;
			this->boneData = nullptr;
			this->materialData = nullptr;
		}
		catch (std::exception& e) {
			//例外処理
			if (resource != nullptr) {
				delete resource;
			}
			throw e;
		}

		return resource;
	}

	K3MDHierarchy* K3MDLoader::LoadK3MDHierarchy(const char* filepath) {

		//ここで作られたK3MDHierarchyは解放が必要
		std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("model data file open failed : " + std::string(filepath));
		}

		//ファイルパスを記録してモデルファイルを起点とした読み込みパスを作り出す
		{
			std::string path = std::string(filepath);
			int position = (int)path.find_last_of("\\/");
			if (position == std::string::npos) {
				//見つからなかった場合は起点パスは空白
				this->fileRoot = "";
			}
			else {
				this->fileRoot = path.substr(0, position + 1);
			}
		}


		//ファイルサイズ取得
		ifs.seekg(0, std::ios::end);
		size_t binarySize = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		ifs.clear();

		K3MDHierarchy* data = nullptr;
		char* binaryData = new char[binarySize];
		try {
			//バイナリを全部読みこみ
			ifs.read(binaryData, binarySize);
			ifs.close();

			//メモリからデータ作成
			data = LoadK3MDHierarchyFromMemory(binaryData);
			data->modelPath = this->fileRoot;
		}
		catch (std::exception& e) {
			//例外処理
			if (data != nullptr) {
				delete data;
			}
			if (binaryData != nullptr) {
				delete[] binaryData;
			}

			throw e;
		}

		delete[] binaryData;
		return data;
	}

	K3MDHierarchy* K3MDLoader::LoadK3MDHierarchyFromMemory(char* binaryData) {
		//メモリからデータ作成
		K3MDHierarchy* data = new K3MDHierarchy;

		try {

			//バイナリをどこまで読み込んだかのカウンタ
			size_t binaryPosition = 0;

			//階層数
			int numHierarchy;
			ReadBinary((char*)&numHierarchy, binaryData, &binaryPosition, sizeof(int));
			data->modelHierarchy.resize(numHierarchy);

			//アニメーション
			int numAnim;
			ReadBinary((char*)&numAnim, binaryData, &binaryPosition, sizeof(int));
			data->animNameList.resize(numAnim);
			for (int j = 0; j < numAnim; ++j) {
				//アニメーション名を追加
				int numString;
				ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
				data->animNameList[j].resize(numString);
				ReadBinary((char*)data->animNameList[j].data(), binaryData, &binaryPosition, sizeof(char) * numString);

				//アニメーションの詳細を追加
				K_Graphics::AnimType animData;
				animData.animID = j;
				animData.animName = data->animNameList[j];
				ReadBinary((char*)&animData.startTime, binaryData, &binaryPosition, sizeof(int));
				ReadBinary((char*)&animData.endTime, binaryData, &binaryPosition, sizeof(int));
				data->animList[animData.animName] = animData;
			}

			//頂点数とかもろもろ確保
			for (int i = 0; i < numHierarchy; ++i) {

				//頂点
				int numVertex;
				ReadBinary((char*)&numVertex, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].vertexData.resize(numVertex);
				ReadBinary((char*)data->modelHierarchy[i].vertexData.data(), binaryData, &binaryPosition, sizeof(K_Graphics::Vertex) * numVertex);

				//マテリアル情報
				int numMaterial;
				ReadBinary((char*)&numMaterial, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].materialData.resize(numMaterial);
				for (int j = 0; j < numMaterial; ++j) {
					//頂点インデックス
					int numIndex;
					ReadBinary((char*)&numIndex, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].vertexIndex.resize(numIndex);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].vertexIndex.data(), binaryData, &binaryPosition, sizeof(unsigned int) * numIndex);

					//マテリアル名
					int numString;
					ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].materialName.resize(numString);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].materialName.data(), binaryData, &binaryPosition, sizeof(char) * numString);
					//テクスチャパス
					ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].texturePath.resize(numString);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].texturePath.data(), binaryData, &binaryPosition, sizeof(char) * numString);
					//モデルデータの場所を基準としたパスに変換
					data->modelHierarchy[i].materialData[j].texturePath = data->modelHierarchy[i].materialData[j].texturePath;


					//マテリアル情報
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].diffuse, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].ambient, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].ambientPower, binaryData, &binaryPosition, sizeof(float));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specular, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specurarShininess, binaryData, &binaryPosition, sizeof(float));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specurarPower, binaryData, &binaryPosition, sizeof(float));
				}

				//ボーン
				int numBone;
				ReadBinary((char*)&numBone, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].boneData.resize(numBone);
				for (int j = 0; j < numBone; ++j) {
					//バインドボーン
					ReadBinary((char*)&data->modelHierarchy[i].boneData[j].bindMat, binaryData, &binaryPosition, sizeof(K3MDMat));
					data->modelHierarchy[i].boneData[j].mat.resize(numAnim);
					//アニメ数でループ
					for (int animIndex = 0; animIndex < numAnim; ++animIndex) {
						int startTime = data->animList[data->animNameList[animIndex]].startTime;
						int endTime = data->animList[data->animNameList[animIndex]].endTime;
						data->modelHierarchy[i].boneData[j].mat[animIndex].resize(startTime + endTime);

						//アニメーションの行列をアニメフレーム数だけ取得
						for (int frame = startTime; frame < endTime; ++frame) {
							ReadBinary((char*)&data->modelHierarchy[i].boneData[j].mat[animIndex][frame], binaryData, &binaryPosition, sizeof(K3MDMat));
						}
					}
				}
			}
		}
		catch (std::exception& e) {
			//例外処理
			if (data != nullptr) {
				delete data;
			}
		}

		return data;
	}


	/////////
	//private
	////
	void K3MDLoader::CreateVertex(K3MDData* modelData, GLuint VAO, const std::vector<GLuint>& IBOs){
		//頂点バッファ設定
		this->bufferData = new K_Graphics::VertexData;
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, modelData->vertexData.size() * sizeof(K_Graphics::Vertex), modelData->vertexData.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);


		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(K_Graphics::Vertex), 0);
		//textureUV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(K_Graphics::Vertex), (unsigned char*)NULL + (3 * sizeof(float)));
		//normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(K_Graphics::Vertex), (unsigned char*)NULL + (5 * sizeof(float)));
		//boneWeight
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(K_Graphics::Vertex), (unsigned char*)NULL + (8 * sizeof(float)));
		//boneIndex
		glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(K_Graphics::Vertex), (unsigned char*)NULL + (12 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		K_Graphics::VertexBuffer buffer;
		buffer.VAO = VAO;
		buffer.VBO = VBO;
		buffer.IBOs = IBOs;
		buffer.numMaterial = (int)modelData->materialData.size();
		buffer.numFace = (int)modelData->vertexData.size() / 3;
		this->bufferData->Add(buffer);
	}

	void K3MDLoader::CreateMaterial(const std::string& fileRoot, K3MDData* modelData, std::vector<GLuint>& IBOs){
		//マテリアル
		this->materialData = new K_Graphics::MaterialData;

		std::vector<K_Graphics::Material> material;
		int numMaterial = (int)modelData->materialData.size();
		material.resize(numMaterial);
		IBOs.resize(numMaterial);

		for (int i = 0; i < numMaterial; ++i) {

			//環境光
			material[i].ambient = modelData->materialData[i].ambient;
			//環境光強度
			material[i].ambientPower = modelData->materialData[i].ambientPower;

			//拡散光
			material[i].diffuse = modelData->materialData[i].diffuse;

			//鏡面反射
			material[i].specular = modelData->materialData[i].specular;

			//鏡面反射強度
			material[i].specurarShininess = modelData->materialData[i].specurarShininess;
			material[i].specurarPower = modelData->materialData[i].specurarPower;


			//テクスチャ
			if (modelData->materialData[i].texturePath != "") {
				try {
					this->textureList->LoadTexture(fileRoot + modelData->materialData[i].texturePath, fileRoot + modelData->materialData[i].texturePath);

				}
				catch (std::exception& e) {
					throw e;
				}

				material[i].texture = this->textureList->GetTexture(fileRoot + modelData->materialData[i].texturePath);
				printf("Texture : %s\n", modelData->materialData[i].texturePath.data());
			}

			//インデックスバッファ
			material[i].numFace = (int)modelData->materialData[i].vertexIndex.size() / 3;

			glGenBuffers(1, &IBOs[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * material[i].numFace * 3, modelData->materialData[i].vertexIndex.data(), GL_STATIC_DRAW);
		}
		this->materialData->Add(material);
	}

	void K3MDLoader::CreateBone(K3MDHierarchy* data, K3MDData* modelData){
		if (data->animList.size() == 0) {
			return;
		}
		this->boneData = new K_Graphics::BoneData;

		//クォータニオンを行列へ変換
		std::vector<K_Graphics::Bone> bone;
		bone.resize(modelData->boneData.size());
		for (int i = 0; i < modelData->boneData.size(); ++i) {
			bone[i].bindMat[0] = K_Math::Vector4(modelData->boneData[i].bindMat.line[0], 0.0f);
			bone[i].bindMat[1] = K_Math::Vector4(modelData->boneData[i].bindMat.line[1], 0.0f);
			bone[i].bindMat[2] = K_Math::Vector4(modelData->boneData[i].bindMat.line[2], 0.0f);
			bone[i].bindMat[3] = K_Math::Vector4(modelData->boneData[i].bindMat.line[3], 1.0f);


			bone[i].mat.resize(modelData->boneData[i].mat.size());
			for (int j = 0; j < modelData->boneData[i].mat.size(); ++j) {
				bone[i].mat[j].resize(modelData->boneData[i].mat[j].size());
				for (int k = 0; k < modelData->boneData[i].mat[j].size(); ++k) {
					bone[i].mat[j][k][0] = K_Math::Vector4(modelData->boneData[i].mat[j][k].line[0], 0.0f);
					bone[i].mat[j][k][1] = K_Math::Vector4(modelData->boneData[i].mat[j][k].line[1], 0.0f);
					bone[i].mat[j][k][2] = K_Math::Vector4(modelData->boneData[i].mat[j][k].line[2], 0.0f);
					bone[i].mat[j][k][3] = K_Math::Vector4(modelData->boneData[i].mat[j][k].line[3], 1.0f);
				}
			}
		}
		this->boneData->AddBoneData(bone);
		for (auto i : data->animNameList) {
			this->boneData->AddAnimData(data->animList[i]);
		}
	}


	void K3MDLoader::ReadBinary(char* dest, char* src, size_t* position, size_t size) {
		memcpy(dest, src + (*position), size);
		//読みこんだ分だけ位置を進める
		(*position) += size;
	}

}
