#include"FbxModelLoader.h"

namespace K_Loader {
	////////
	//public
	////
	FbxModelLoader::FbxModelLoader() {
		this->fbxData = nullptr;
		this->bufferData = nullptr;
		this->materialData = nullptr;
		this->animationData = nullptr;
		this->boneData = nullptr;

		this->loaded = false;
	}
	FbxModelLoader::~FbxModelLoader() {
		Finalize();
	}

	bool FbxModelLoader::LoadFBX(const std::string& fileName, K_Graphics::TextureList* list) {
		this->fbxData = new K_Graphics::FbxData;
		this->bufferData = new K_Graphics::VertexData;
		this->materialData = new K_Graphics::MaterialData;
		this->animationData = new K_Graphics::AnimationData;
		this->boneData = new K_Graphics::BoneData;

		this->textureList = list;

		if (!InitializeFBX(fileName)) {
			return false;
		}

		printf("LoadModel... : %s\n", fileName.data());
		for (int i = 0; i < fileName.size() - 1; ++i) {
			this->fileRoot[i] = fileName.data()[i];
			if (fileName.data()[i] == '\\' || fileName.data()[i] == '/') {
				this->fileRoot[i + 1] = '\0';
				break;
			}
		}

		FbxNode *rootNode = this->fbxData->GetScene()->GetRootNode();
		if (!RecursiveNode(rootNode)) {
			return false;
		}
		this->loaded = true;
		return true;
	}

	//FBX�|�C���^�̏��n
	K_Graphics::FbxData* FbxModelLoader::PassFbxData() {
		K_Graphics::FbxData* returnData = this->fbxData;
		this->fbxData = nullptr;
		return returnData;
	}
	//���_�o�b�t�@���̏��n
	K_Graphics::VertexData* FbxModelLoader::PassVertexBuffer() {
		K_Graphics::VertexData* returnData = this->bufferData;
		this->bufferData = nullptr;
		return returnData;
	}
	//�}�e���A�����̏��n
	K_Graphics::MaterialData* FbxModelLoader::PassMaterialData() {
		K_Graphics::MaterialData* returnData = this->materialData;
		this->materialData = nullptr;
		return returnData;
	}
	//�A�j���[�V�������̏��n
	K_Graphics::AnimationData* FbxModelLoader::PassAnimationData() {
		K_Graphics::AnimationData* returnData = this->animationData;
		this->animationData = nullptr;
		return returnData;
	}
	//�{�[���s����̏��n
	K_Graphics::BoneData* FbxModelLoader::PassBoneData() {
		K_Graphics::BoneData* returnData = this->boneData;
		this->boneData = nullptr;
		return returnData;
	}

	////////
	//private
	////
	bool FbxModelLoader::RecursiveNode(FbxNode* node) {
		FbxNodeAttribute* attr;
		attr = node->GetNodeAttribute();

		if (attr != NULL) {
			if (attr->GetAttributeType() == FbxNodeAttribute::eMesh) {
				if (!LoadFbxMesh(node->GetMesh())) {
					return false;
				}
			}
		}
		//�ċN
		int numChild = node->GetChildCount();
		for (int i = 0; i < numChild; ++i) {
			if (!RecursiveNode(node->GetChild(i))) {
				return false;
			}
		}
		return true;
	}

	bool FbxModelLoader::LoadFbxMesh(FbxMesh* mesh) {
		Vertex* vertex = nullptr;
		PolygonTable* table = nullptr;
		try {
			//�|���S�����������_�𑝂₷
			mesh->SplitPoints();

			this->numVertex = mesh->GetControlPointsCount();
			this->numFace = mesh->GetPolygonCount();
			this->numUV = mesh->GetTextureUVCount();
			vertex = new Vertex[this->numVertex];


			//���_
			LoadVertex(mesh, vertex);
			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//�}�e���A��
			FbxNode* node = mesh->GetNode();
			int numMaterial = node->GetMaterialCount();
			std::vector<K_Graphics::Material> material;
			std::vector<GLuint> IBOs;
			LoadMaterial(mesh, material, IBOs);

			//�{�[��
			this->LoadBones(mesh, vertex, table);

			if (numUV > this->numVertex) {
				this->numVertex = numUV;
			}
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, this->numVertex * sizeof(Vertex), vertex, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);


			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			//position
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			//textureUV
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));
			//normal
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (5 * sizeof(float)));
			//boneWeight
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (8 * sizeof(float)));
			//boneIndex
			glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + (12 * sizeof(float)));


			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);


			K_Graphics::VertexBuffer buffer;
			buffer.VAO = VAO;
			buffer.VBO = VBO;
			buffer.IBOs = IBOs;
			buffer.numMaterial = numMaterial;
			buffer.numFace = numFace;
			this->bufferData->Add(buffer);

			delete[] vertex;
		}
		catch (std::string& eText) {
			if (vertex) {
				delete[] vertex;
			}
			if (table) {
				delete[] table;
			}
			Finalize();
			throw(eText);
		}
		return true;
	}

	void FbxModelLoader::Finalize() {
		if (this->fbxData != nullptr) {
			delete this->fbxData;
			this->fbxData = nullptr;
		}
		if (this->bufferData != nullptr) {
			delete this->bufferData;
			this->bufferData = nullptr;
		}
		if (this->materialData != nullptr) {
			delete this->materialData;
			this->materialData = nullptr;
		}
		if (this->animationData != nullptr) {
			delete this->animationData;
			this->animationData = nullptr;
		}
		if (this->boneData != nullptr) {
			delete this->boneData;
			this->boneData = nullptr;
		}
	}

	//�ʃx�[�X�Ń��b�V���ǂݍ���
	void FbxModelLoader::LoadVertex(FbxMesh* mesh, Vertex* vertex) {
		printf("vertex:%d face:%d uv:%d\n", this->numVertex, this->numFace, this->numUV);

		for (int i = 0; i < this->numFace; ++i) {
			//�O�p�|���S���̈꒸�_���i�[
			for (int p = 0; p < 3; ++p) {
				int vertexIndex;
				//���_�C���f�b�N�X��ʂ��瓾��
				int polygonCount = mesh->GetPolygonVertexIndex(i);
				int *polygonVertex = mesh->GetPolygonVertices();
				vertexIndex = polygonVertex[polygonCount + p];

				//���_
				FbxVector4 *pCoord = mesh->GetControlPoints();
				int index = mesh->GetPolygonVertex(i, p);
				vertex[vertexIndex].position.x() = (float)pCoord[index][0];
				vertex[vertexIndex].position.y() = (float)pCoord[index][1];
				vertex[vertexIndex].position.z() = (float)pCoord[index][2];

				//�@��
				FbxVector4 normal;
				mesh->GetPolygonVertexNormal(i, p, normal);
				vertex[vertexIndex].normal.x() = (float)normal[0];
				vertex[vertexIndex].normal.y() = (float)normal[1];
				vertex[vertexIndex].normal.z() = (float)normal[2];
				vertex[vertexIndex].normal.normalize();

				//UV
				if (!this->numUV) {
					continue;
				}
				FbxLayerElementUV *pUV;
				pUV = mesh->GetLayer(0)->GetUVs();
				if (pUV->GetMappingMode() == FbxLayerElementUV::eByPolygonVertex) {
					int uvIndex = mesh->GetTextureUVIndex(i, p, FbxLayerElement::eTextureDiffuse);
					FbxVector2 v2 = pUV->GetDirectArray().GetAt(uvIndex);
					vertex[vertexIndex].texcoord.x() = (float)v2[0];
					vertex[vertexIndex].texcoord.y() = (float)v2[1];

				}
			}
		}

		//UV�^�C�v��eByControlPoint�̎�

		if (this->numUV) {
			FbxVector2 v2;
			FbxLayerElementUV *pUV;
			pUV = mesh->GetLayer(0)->GetUVs();
			if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint) {
				pUV = mesh->GetLayer(0)->GetUVs();
				for (int k = 0; k < this->numUV; ++k) {
					v2 = pUV->GetDirectArray().GetAt(k);
					vertex[k].texcoord.x() = (float)v2[0];
					vertex[k].texcoord.y() = (float)v2[1];
				}
			}
		}
	}

	//�}�e���A���ǂݍ���
	void FbxModelLoader::LoadMaterial(FbxMesh* mesh, std::vector<K_Graphics::Material>& material, std::vector<GLuint>& IBOs) {
		FbxNode* node = mesh->GetNode();
		int numMaterial = node->GetMaterialCount();

		material.resize(numMaterial);
		IBOs.resize(numMaterial);

		for (int i = 0; i < numMaterial; ++i) {
			FbxSurfaceMaterial *pMaterial = node->GetMaterial(i);
			FbxClassId materialType = pMaterial->GetClassId();

			if (materialType.Is(FbxSurfacePhong::ClassId)) {
				FbxSurfacePhong *pPhong = (FbxSurfacePhong*)pMaterial;

				material[i].ambient(0) = (float)(pPhong->Ambient.Get()[0]);
				material[i].ambient(1) = (float)(pPhong->Ambient.Get()[1]);
				material[i].ambient(2) = (float)(pPhong->Ambient.Get()[2]);
				material[i].ambient(3) = 1.0f;

				material[i].diffuse(0) = (float)(pPhong->Diffuse.Get()[0]);
				material[i].diffuse(1) = (float)(pPhong->Diffuse.Get()[1]);
				material[i].diffuse(2) = (float)(pPhong->Diffuse.Get()[2]);
				material[i].diffuse(3) = 1.0f;

				material[i].specular(0) = (float)(pPhong->Specular.Get()[0]);
				material[i].specular(1) = (float)(pPhong->Specular.Get()[1]);
				material[i].specular(2) = (float)(pPhong->Specular.Get()[2]);
				material[i].specular(3) = 1.0f;

				//���ʔ��ˋ��x
				material[i].power = (float)(pPhong->Shininess.Get());

			}
			else if (materialType.Is(FbxSurfaceLambert::ClassId)) {
				FbxSurfaceLambert *pLambert = (FbxSurfaceLambert*)pMaterial;

				material[i].ambient(0) = (float)(pLambert->Ambient.Get()[0]);
				material[i].ambient(1) = (float)(pLambert->Ambient.Get()[1]);
				material[i].ambient(2) = (float)(pLambert->Ambient.Get()[2]);
				material[i].ambient(3) = 1.0f;

				material[i].diffuse(0) = (float)(pLambert->Diffuse.Get()[0]);
				material[i].diffuse(1) = (float)(pLambert->Diffuse.Get()[1]);
				material[i].diffuse(2) = (float)(pLambert->Diffuse.Get()[2]);
				material[i].diffuse(3) = 1.0f;

				material[i].specular(0) = 0.0f;
				material[i].specular(1) = 0.0f;
				material[i].specular(2) = 0.0f;
				material[i].specular(3) = 1.0f;

				//���ʔ��ˋ��x
				material[i].power = 1.0f;
			}

			//�e�N�X�`��
			FbxProperty lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse, 0);
			FbxFileTexture *pTexture = FbxCast<FbxFileTexture>(lProperty.GetSrcObject<FbxFileTexture>(0));
			material[i].texture = nullptr;
			if (!pTexture) {
			}
			else {
				const char *fullName = pTexture->GetRelativeFileName();
				//�ŏI�I�Ɏg�p����t�@�C����
				char fileName[120] = "";
				//Blender����ǂݎ�������΃p�X�̃f�B���N�g��
				char directory[100] = "";
				//Blender����ǂݎ�������O
				char name[100] = "";
				//�g���q
				char ext[10] = "";
				//�t�@�C�������擾(�t�@�C�����Ɗg���q�̂�)
				_splitpath_s(fullName, 0, 0, directory, 100, name, 100, ext, 10);

				strcat_s(fileName, this->fileRoot);
				strcat_s(fileName, directory);
				strcat_s(fileName, name);
				strcat_s(fileName, ext);

				if (strcmp(ext, ".tga") == 0) {

					if (!this->textureList->LoadTexture(fileName, fileName)) {
						throw("Texture Load Failed : " + std::string(fileName));
					}
					material[i].texture = this->textureList->GetTexture(fileName);
					printf("Texture : %s\n", fileName);
				}
			}
			//�C���f�b�N�X�o�b�t�@
			int indexCount = 0;
			int *pIndex = new int[this->numFace * 3];
			for (int k = 0; k < this->numFace; ++k) {
				FbxLayerElementMaterial *material = mesh->GetLayer(0)->GetMaterials();

				int matId = material->GetIndexArray().GetAt(k);
				if (matId == i) {
					pIndex[indexCount] = mesh->GetPolygonVertex(k, 0);
					pIndex[indexCount + 1] = mesh->GetPolygonVertex(k, 1);
					pIndex[indexCount + 2] = mesh->GetPolygonVertex(k, 2);
					indexCount += 3;
				}
			}
			material[i].numFace = indexCount / 3;

			glGenBuffers(1, &IBOs[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexCount, pIndex, GL_STATIC_DRAW);
			delete[] pIndex;
		}
		this->materialData->Add(material);
	}

	bool FbxModelLoader::LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table) {
		FbxDeformer *deformer = mesh->GetDeformer(0);
		if (!deformer) {
			//�{�[�������݂��Ȃ�������A�j���[�V�����֘A�̃f�[�^�ɕʂ��������
			delete this->animationData;
			delete this->boneData;
			this->animationData = 0;
			this->boneData = 0;
			return false;
		}
		FbxSkin *skin = static_cast<FbxSkin*>(deformer);

		int numBone = skin->GetClusterCount();
		std::vector<FbxCluster*> cluster;
		cluster.resize(numBone);
		for (int i = 0; i < numBone; ++i) {
			cluster[i] = skin->GetCluster(i);
		}

		//���_�x�[�X�̃��f���̏ꍇ
		for (int i = 0; i < numBone; ++i) {
			int *index = cluster[i]->GetControlPointIndices();
			double *weight = cluster[i]->GetControlPointWeights();
			int vertexCount = cluster[i]->GetControlPointIndicesCount();

			for (int k = 0; k < vertexCount; ++k) {
				for (int m = 0; m < 4; ++m) {
					if (vertex[index[k]].boneWeight[m] != 0.0f) {
						continue;
					}
					vertex[index[k]].boneIndex.data[m] = i;
					vertex[index[k]].boneWeight[m] = (float)weight[k];
					break;
				}
			}
		}

		//�o�C���h�|�[�Y���擾
		std::vector<K_Graphics::Bone> bone;
		bone.resize(numBone);
		for (int i = 0; i < numBone; ++i) {
			FbxAMatrix mat;
			cluster[i]->GetTransformLinkMatrix(mat);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					bone[i].bindMat(x + y * 4) = (float)mat.Get(y, x);
					bone[i].cluster = cluster[i];
				}
			}
		}
		this->boneData->Add(bone);

		return true;
	}



	bool FbxModelLoader::InitializeFBX(const std::string& fileName) {

		//�}�l�[�W���[�𐶐������f���f�[�^���C���|�[�g
		FbxManager  *manager;
		FbxImporter *importer;
		FbxScene    *scene;
		manager = FbxManager::Create();
		if (!manager) {
			return false;
		}

		importer = FbxImporter::Create(manager, "");
		scene = FbxScene::Create(manager, "");

		this->fbxData->Add(manager, importer, scene);

		if (!importer || !scene) {
			return false;
		}

		//�������ƃC���|�[�g
		if (!importer->Initialize(fileName.data())) {
			return false;
		}
		if (!importer->Import(scene)) {
			return false;
		}

		//�ʂ��O�p���A�]�v�Ȗʂ���菜��
		FbxGeometryConverter converter(manager);
		converter.Triangulate(scene, true);
		converter.RemoveBadPolygonsFromMeshes(scene);

		//�A�j���[�V���������擾�A���O���L�[�ɂ��ĕێ�
		int numAnim = importer->GetAnimStackCount();
		for (int i = 0; i < numAnim; ++i) {
			//�擾
			FbxTakeInfo *take = importer->GetTakeInfo(i);
			K_Graphics::AnimType anim;
			auto start = take->mLocalTimeSpan.GetStart();
			auto end = take->mLocalTimeSpan.GetStop();
			anim.animName = take->mName.Buffer();
			anim.animID = i;
			anim.startTime = (int)(start.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));
			anim.endTime = (int)(end.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));

			//�ǉ�
			this->animationData->Add(anim);
		}

		return true;
	}

}