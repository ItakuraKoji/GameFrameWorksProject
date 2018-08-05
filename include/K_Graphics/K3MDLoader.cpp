#include "K3MDLoader.h"

namespace K_Loader {

	/////////
	//public
	////
	K3MDLoader::K3MDLoader(){

	}

	K3MDLoader::~K3MDLoader(){
	}

	void K3MDLoader::CreateBinaryFile(K3MDHierarchy* data, const char* fileName){
		//K3MD�`����3D���f���f�[�^��f���o��
		std::ofstream ofs(fileName, std::ios::binary | std::ios::trunc | std::ios::out);
		if (!ofs) {
			return;
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
			int numVertex = modelData.vertexData.size();
			ofs.write((char*)&numVertex, sizeof(int));
			ofs.write((char*)modelData.vertexData.data(), sizeof(K_Graphics::Vertex) * numVertex);
			//�}�e���A��
			int numMaterial = modelData.materialData.size();
			ofs.write((char*)&numMaterial, sizeof(int));
			for (int i = 0; i < numMaterial; ++i) {
				int numIndex = modelData.materialData[i].vertexIndex.size();
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

	K_Graphics::ModelResource* K3MDLoader::LoadModel(const char* filepath, K_Graphics::TextureList* list){
		this->bufferData = nullptr;
		this->boneData = nullptr;
		this->materialData = nullptr;
		std::ifstream ifs(filepath, std::ios::in | std::ios::binary);
		if (!ifs) {
			return nullptr;
		}
		K3MDHierarchy* data = new K3MDHierarchy;


		//�K�w��
		int numHierarchy;
		ifs.read((char*)&numHierarchy, sizeof(int));
		data->modelHierarchy.resize(numHierarchy);

		//�A�j���[�V����
		int numAnim;
		ifs.read((char*)&numAnim, sizeof(int));
		data->animNameList.resize(numAnim);
		for (int j = 0; j < numAnim; ++j) {
			//�A�j���[�V��������ǉ�
			int numString;
			ifs.read((char*)&numString, sizeof(int));
			data->animNameList[j].resize(numString);
			ifs.read((char*)data->animNameList[j].data(), sizeof(char) * numString);

			//�A�j���[�V�����̏ڍׂ�ǉ�
			K_Graphics::AnimType animData;
			animData.animID = j;
			animData.animName = data->animNameList[j];
			ifs.read((char*)&animData.startTime, sizeof(int));
			ifs.read((char*)&animData.endTime, sizeof(int));
			data->animList[animData.animName] = animData;
		}

		//���_���Ƃ��������m��
		for (int i = 0; i < numHierarchy; ++i) {

			//���_
			int numVertex;
			ifs.read((char*)&numVertex, sizeof(int));
			data->modelHierarchy[i].vertexData.resize(numVertex);
			ifs.read((char*)data->modelHierarchy[i].vertexData.data(), sizeof(K_Graphics::Vertex) * numVertex);

			//�}�e���A�����
			int numMaterial;
			ifs.read((char*)&numMaterial, sizeof(int));
			data->modelHierarchy[i].materialData.resize(numMaterial);
			for (int j = 0; j < numMaterial; ++j) {
				int numIndex;
				ifs.read((char*)&numIndex, sizeof(int));
				data->modelHierarchy[i].materialData[j].vertexIndex.resize(numIndex);
				ifs.read((char*)data->modelHierarchy[i].materialData[j].vertexIndex.data(), sizeof(unsigned int) * numIndex);

				//�}�e���A�����ƃe�N�X�`���̃p�X
				int numString;
				ifs.read((char*)&numString, sizeof(int));
				data->modelHierarchy[i].materialData[j].materialName.resize(numString);
				ifs.read((char*)data->modelHierarchy[i].materialData[j].materialName.data(), sizeof(char) * numString);
				ifs.read((char*)&numString, sizeof(int));
				data->modelHierarchy[i].materialData[j].texturePath.resize(numString);
				ifs.read((char*)data->modelHierarchy[i].materialData[j].texturePath.data(), sizeof(char) * numString);

				//�}�e���A�����
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].diffuse, sizeof(K_Math::Vector4));
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].ambient, sizeof(K_Math::Vector4));
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].ambientPower, sizeof(float));
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].specular, sizeof(K_Math::Vector4));
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].specurarShininess, sizeof(float));
				ifs.read((char*)&data->modelHierarchy[i].materialData[j].specurarPower, sizeof(float));
			}

			//�{�[��
			int numBone;
			ifs.read((char*)&numBone, sizeof(int));
			data->modelHierarchy[i].boneData.resize(numBone);
			for (int j = 0; j < numBone; ++j) {
				//�o�C���h�{�[��
				ifs.read((char*)&data->modelHierarchy[i].boneData[j].bindMat, sizeof(K3MDMat));
				data->modelHierarchy[i].boneData[j].mat.resize(numAnim);
				//�A�j�����Ń��[�v
				for (int animIndex = 0; animIndex < numAnim; ++animIndex) {
					int startTime = data->animList[data->animNameList[animIndex]].startTime;
					int endTime = data->animList[data->animNameList[animIndex]].endTime;
					data->modelHierarchy[i].boneData[j].mat[animIndex].resize(startTime + endTime);

					//�A�j���[�V�����̍s����A�j���t���[���������擾
					for (int frame = startTime; frame < endTime; ++frame) {
						ifs.read((char*)&data->modelHierarchy[i].boneData[j].mat[animIndex][frame], sizeof(K3MDMat));
					}
				}
			}
		}

		K_Graphics::ModelResource* resource = LoadModel(data, list);
		delete data;
		return resource;
	}

	K_Graphics::ModelResource* K3MDLoader::LoadModel(K3MDHierarchy* data, K_Graphics::TextureList* list) {
		this->bufferData = nullptr;
		this->boneData = nullptr;
		this->materialData = nullptr;
		printf("load from K3MD\n");
		K_Graphics::ModelResource* resource = new K_Graphics::ModelResource;
		this->textureList = list;

		//���f���f�[�^���K�w����ǂݍ���
		for (auto modelData : data->modelHierarchy) {
			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			std::vector<GLuint> IBOs;
			CreateMaterial(&modelData, IBOs);
			CreateBone(data, &modelData);
			CreateVertex(&modelData, VAO, IBOs);
		}
		resource->vertexBuffer = this->bufferData;
		resource->material = this->materialData;
		resource->bone = this->boneData;
		return resource;
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
		buffer.numFace = modelData->vertexData.size() / 3;
		this->bufferData->Add(buffer);
	}
	void K3MDLoader::CreateMaterial(K3MDData * modelData, std::vector<GLuint>& IBOs){
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
					this->textureList->LoadTexture(modelData->materialData[i].texturePath, modelData->materialData[i].texturePath);

				}
				catch (std::exception& e) {
					throw e;
				}

				material[i].texture = this->textureList->GetTexture(modelData->materialData[i].texturePath);
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

}
