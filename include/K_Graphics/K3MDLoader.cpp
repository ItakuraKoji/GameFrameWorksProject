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
		//K3MD�`����3D���f���f�[�^��f���o��
		std::ofstream ofs(outputFilePath, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!ofs) {
			throw std::runtime_error("fileOpenFailed : " + outputFilePath);
		}
		//�K�w��
		int numHierarchy = (int)data->modelHierarchy.size();
		ofs.write((char*)&numHierarchy, sizeof(int));
		//�A�j���[�V����
		int numAnim = (int)data->animNameList.size();
		ofs.write((char*)&numAnim, sizeof(int));
		for (int i = 0; i < numAnim; ++i) {
			int numString = (int)data->animNameList[i].size();
			//�A�j����
			ofs.write((char*)&numString, sizeof(int));
			ofs.write((char*)data->animNameList[i].data(), sizeof(char) * numString);
			//�J�n���ԁA�I������
			ofs.write((char*)&data->animList[data->animNameList[i]].startTime, sizeof(int));
			ofs.write((char*)&data->animList[data->animNameList[i]].endTime, sizeof(int));
		}


		for (auto& modelData : data->modelHierarchy) {
			//���_���
			int numVertex = (int)modelData.vertexData.size();
			ofs.write((char*)&numVertex, sizeof(int));
			ofs.write((char*)modelData.vertexData.data(), sizeof(K_Graphics::Vertex) * numVertex);
			//�}�e���A��
			int numMaterial = (int)modelData.materialData.size();
			ofs.write((char*)&numMaterial, sizeof(int));
			for (int i = 0; i < numMaterial; ++i) {
				int numIndex = (int)modelData.materialData[i].vertexIndex.size();
				ofs.write((char*)&numIndex, sizeof(int));
				ofs.write((char*)modelData.materialData[i].vertexIndex.data(), sizeof(unsigned int) * numIndex);

				//�}�e���A�����ƃe�N�X�`���̃p�X
				int numString = (int)modelData.materialData[i].materialName.size();
				ofs.write((char*)&numString, sizeof(int));
				ofs.write((char*)modelData.materialData[i].materialName.data(), sizeof(char) * numString);
				numString = (int)modelData.materialData[i].texturePath.size();
				ofs.write((char*)&numString, sizeof(int));
				ofs.write((char*)modelData.materialData[i].texturePath.data(), sizeof(char) * numString);

				//�}�e���A�����
				ofs.write((char*)&modelData.materialData[i].diffuse, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].ambient, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].ambientPower, sizeof(float));
				ofs.write((char*)&modelData.materialData[i].specular, sizeof(K_Math::Vector4));
				ofs.write((char*)&modelData.materialData[i].specurarShininess, sizeof(float));
				ofs.write((char*)&modelData.materialData[i].specurarPower, sizeof(float));

			}

			//�{�[��
			int numBone = (int)modelData.boneData.size();
			ofs.write((char*)&numBone, sizeof(int));
			for (int j = 0; j < numBone; ++j) {
				//�o�C���h�{�[��
				ofs.write((char*)&modelData.boneData[j].bindMat, sizeof(K3MDMat));
				//�A�j�����Ń��[�v
				for (int animIndex = 0; animIndex < numAnim; ++animIndex) {
					int startTime = data->animList[data->animNameList[animIndex]].startTime;
					int endTime = data->animList[data->animNameList[animIndex]].endTime;
					modelData.boneData[j].mat[animIndex].resize(startTime + endTime);

					//�A�j���[�V�����̍s����A�j���t���[���������擾
					for (int frame = startTime; frame < endTime; ++frame) {
						ofs.write((char*)&modelData.boneData[j].mat[animIndex][frame], sizeof(K3MDMat));
					}
				}
			}

		}
		ofs.close();
	}

	void K3MDLoader::CreateBinaryFile(K3MDHierarchy* data, const std::string& fileName, const std::string& outputFilePath){
		//K3MD�`����3D���f���f�[�^��f���o��
		CreateBinaryFile(data, outputFilePath + "/" + fileName + ".k3md");
	}

	K_Graphics::ModelResource* K3MDLoader::LoadModel(const char* filepath, K_Graphics::TextureList* list){
		K_Graphics::ModelResource* resource = nullptr;
		K3MDHierarchy* data = nullptr;
		try {
			//�\���̂��쐬���āA�������烂�f���쐬
			this->bufferData = nullptr;
			this->boneData = nullptr;
			this->materialData = nullptr;

			data = LoadK3MDHierarchy(filepath);

			resource = LoadModel(data, list);
			delete data;
		}
		catch (std::exception& e) {
			//��O����
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


		//�\���̂��烂�f���쐬
		this->bufferData = nullptr;
		this->boneData = nullptr;
		this->materialData = nullptr;
		printf("load from K3MD\n");
		K_Graphics::ModelResource* resource = new K_Graphics::ModelResource;

		try {
			this->textureList = list;

			//���f���f�[�^���K�w����ǂݍ���
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
			//��O����
			if (resource != nullptr) {
				delete resource;
			}
			throw e;
		}

		return resource;
	}

	K3MDHierarchy* K3MDLoader::LoadK3MDHierarchy(const char* filepath) {

		//�����ō��ꂽK3MDHierarchy�͉�����K�v
		std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
		if (!ifs) {
			throw std::runtime_error("model data file open failed : " + std::string(filepath));
		}

		//�t�@�C���p�X���L�^���ă��f���t�@�C�����N�_�Ƃ����ǂݍ��݃p�X�����o��
		{
			std::string path = std::string(filepath);
			int position = (int)path.find_last_of("\\/");
			if (position == std::string::npos) {
				//������Ȃ������ꍇ�͋N�_�p�X�͋�
				this->fileRoot = "";
			}
			else {
				this->fileRoot = path.substr(0, position + 1);
			}
		}


		//�t�@�C���T�C�Y�擾
		ifs.seekg(0, std::ios::end);
		size_t binarySize = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		ifs.clear();

		K3MDHierarchy* data = nullptr;
		char* binaryData = new char[binarySize];
		try {
			//�o�C�i����S���ǂ݂���
			ifs.read(binaryData, binarySize);
			ifs.close();

			//����������f�[�^�쐬
			data = LoadK3MDHierarchyFromMemory(binaryData);
			data->modelPath = this->fileRoot;
		}
		catch (std::exception& e) {
			//��O����
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
		//����������f�[�^�쐬
		K3MDHierarchy* data = new K3MDHierarchy;

		try {

			//�o�C�i�����ǂ��܂œǂݍ��񂾂��̃J�E���^
			size_t binaryPosition = 0;

			//�K�w��
			int numHierarchy;
			ReadBinary((char*)&numHierarchy, binaryData, &binaryPosition, sizeof(int));
			data->modelHierarchy.resize(numHierarchy);

			//�A�j���[�V����
			int numAnim;
			ReadBinary((char*)&numAnim, binaryData, &binaryPosition, sizeof(int));
			data->animNameList.resize(numAnim);
			for (int j = 0; j < numAnim; ++j) {
				//�A�j���[�V��������ǉ�
				int numString;
				ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
				data->animNameList[j].resize(numString);
				ReadBinary((char*)data->animNameList[j].data(), binaryData, &binaryPosition, sizeof(char) * numString);

				//�A�j���[�V�����̏ڍׂ�ǉ�
				K_Graphics::AnimType animData;
				animData.animID = j;
				animData.animName = data->animNameList[j];
				ReadBinary((char*)&animData.startTime, binaryData, &binaryPosition, sizeof(int));
				ReadBinary((char*)&animData.endTime, binaryData, &binaryPosition, sizeof(int));
				data->animList[animData.animName] = animData;
			}

			//���_���Ƃ��������m��
			for (int i = 0; i < numHierarchy; ++i) {

				//���_
				int numVertex;
				ReadBinary((char*)&numVertex, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].vertexData.resize(numVertex);
				ReadBinary((char*)data->modelHierarchy[i].vertexData.data(), binaryData, &binaryPosition, sizeof(K_Graphics::Vertex) * numVertex);

				//�}�e���A�����
				int numMaterial;
				ReadBinary((char*)&numMaterial, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].materialData.resize(numMaterial);
				for (int j = 0; j < numMaterial; ++j) {
					//���_�C���f�b�N�X
					int numIndex;
					ReadBinary((char*)&numIndex, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].vertexIndex.resize(numIndex);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].vertexIndex.data(), binaryData, &binaryPosition, sizeof(unsigned int) * numIndex);

					//�}�e���A����
					int numString;
					ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].materialName.resize(numString);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].materialName.data(), binaryData, &binaryPosition, sizeof(char) * numString);
					//�e�N�X�`���p�X
					ReadBinary((char*)&numString, binaryData, &binaryPosition, sizeof(int));
					data->modelHierarchy[i].materialData[j].texturePath.resize(numString);
					ReadBinary((char*)data->modelHierarchy[i].materialData[j].texturePath.data(), binaryData, &binaryPosition, sizeof(char) * numString);
					//���f���f�[�^�̏ꏊ����Ƃ����p�X�ɕϊ�
					data->modelHierarchy[i].materialData[j].texturePath = data->modelHierarchy[i].materialData[j].texturePath;


					//�}�e���A�����
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].diffuse, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].ambient, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].ambientPower, binaryData, &binaryPosition, sizeof(float));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specular, binaryData, &binaryPosition, sizeof(K_Math::Vector4));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specurarShininess, binaryData, &binaryPosition, sizeof(float));
					ReadBinary((char*)&data->modelHierarchy[i].materialData[j].specurarPower, binaryData, &binaryPosition, sizeof(float));
				}

				//�{�[��
				int numBone;
				ReadBinary((char*)&numBone, binaryData, &binaryPosition, sizeof(int));
				data->modelHierarchy[i].boneData.resize(numBone);
				for (int j = 0; j < numBone; ++j) {
					//�o�C���h�{�[��
					ReadBinary((char*)&data->modelHierarchy[i].boneData[j].bindMat, binaryData, &binaryPosition, sizeof(K3MDMat));
					data->modelHierarchy[i].boneData[j].mat.resize(numAnim);
					//�A�j�����Ń��[�v
					for (int animIndex = 0; animIndex < numAnim; ++animIndex) {
						int startTime = data->animList[data->animNameList[animIndex]].startTime;
						int endTime = data->animList[data->animNameList[animIndex]].endTime;
						data->modelHierarchy[i].boneData[j].mat[animIndex].resize(startTime + endTime);

						//�A�j���[�V�����̍s����A�j���t���[���������擾
						for (int frame = startTime; frame < endTime; ++frame) {
							ReadBinary((char*)&data->modelHierarchy[i].boneData[j].mat[animIndex][frame], binaryData, &binaryPosition, sizeof(K3MDMat));
						}
					}
				}
			}
		}
		catch (std::exception& e) {
			//��O����
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
		//���_�o�b�t�@�ݒ�
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
		//�}�e���A��
		this->materialData = new K_Graphics::MaterialData;

		std::vector<K_Graphics::Material> material;
		int numMaterial = (int)modelData->materialData.size();
		material.resize(numMaterial);
		IBOs.resize(numMaterial);

		for (int i = 0; i < numMaterial; ++i) {

			//����
			material[i].ambient = modelData->materialData[i].ambient;
			//�������x
			material[i].ambientPower = modelData->materialData[i].ambientPower;

			//�g�U��
			material[i].diffuse = modelData->materialData[i].diffuse;

			//���ʔ���
			material[i].specular = modelData->materialData[i].specular;

			//���ʔ��ˋ��x
			material[i].specurarShininess = modelData->materialData[i].specurarShininess;
			material[i].specurarPower = modelData->materialData[i].specurarPower;


			//�e�N�X�`��
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

			//�C���f�b�N�X�o�b�t�@
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

		//�N�H�[�^�j�I�����s��֕ϊ�
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
		//�ǂ݂��񂾕������ʒu��i�߂�
		(*position) += size;
	}

}
