#include"FbxModelLoader.h"

namespace K_Loader {
	////////
	//public
	////
	FbxModelLoader::FbxModelLoader() {
		this->fbxData = nullptr;
		this->loaded = false;
	}
	FbxModelLoader::~FbxModelLoader() {
		Finalize();
	}

	bool FbxModelLoader::LoadFBX(const std::string& fileName) {
		try {
			this->modelData = new K3MDHierarchy;
			this->modelData->modelHierarchy.reserve(100);

			this->fbxData = new K_Graphics::FbxData;

			printf("LoadModel... : %s\n", fileName.data());
			InitializeFBX(fileName);

			//�t�@�C���p�X���L�^���ă��f���t�@�C�����N�_�Ƃ����ǂݍ��݃p�X�����o��
			int position = fileName.find_last_of("\\/");
			if (position == std::string::npos) {
				//������Ȃ������ꍇ�͋N�_�p�X�͋�
				this->fileRoot = "";
			}
			else {
				this->fileRoot = fileName.substr(0, position + 1);
			}


			//�m�[�h�擾
			FbxNode *rootNode = this->fbxData->GetScene()->GetRootNode();
			RecursiveNode(rootNode);


			//�A�j���[�V���������擾�A���O���L�[�ɂ��ĕێ�
			FbxImporter* importer = this->fbxData->GetInporter();
			int numAnim = importer->GetAnimStackCount();

			this->modelData->animNameList.reserve(numAnim);
			for (int i = 0; i < numAnim; ++i) {
				//�擾
				FbxTakeInfo *take = importer->GetTakeInfo(i);
				K_Graphics::AnimType anim;
				auto start = take->mLocalTimeSpan.GetStart();
				auto end = take->mLocalTimeSpan.GetStop();
				//�Ȃ񂩃o�C�i���`�����Ɓu"bone|Action"�v�݂����ɂȂ�̂�"bone|"�̕�������菜��
				int position = 0;
				for (int k = (int)take->mName.GetLen() - 1; k >= 0; --k) {
					char c = take->mName.Buffer()[k];
					if (take->mName.Buffer()[k] == '|') {
						position = k + 1;
					}
				}
				anim.animName = take->mName.Mid(position);
				anim.animID = i;
				anim.startTime = (int)(start.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));
				anim.endTime = (int)(end.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));

				//�ǉ�
				this->modelData->animNameList.push_back(anim.animName);
				this->modelData->animList[anim.animName] = anim;
			}

			this->loaded = true;
		}
		catch (std::exception& e) {
			printf("%s\n", e.what());
			this->loaded = false;
			return false;
		}
		return true;
	}

	//FBX�|�C���^�̏��n
	K_Graphics::FbxData* FbxModelLoader::PassFbxData() {
		K_Graphics::FbxData* returnData = this->fbxData;
		this->fbxData = nullptr;
		return returnData;
	}
	K3MDHierarchy* FbxModelLoader::GetModelData() {
		return this->modelData;
	}


	////////
	//private
	////
	void FbxModelLoader::RecursiveNode(FbxNode* node) {
		FbxNodeAttribute* attr;
		attr = node->GetNodeAttribute();

		if (attr != NULL) {
			if (attr->GetAttributeType() == FbxNodeAttribute::eMesh) {
				LoadFbxMesh(node->GetMesh());
			}
		}
		//�ċN
		int numChild = node->GetChildCount();
		for (int i = 0; i < numChild; ++i) {
			RecursiveNode(node->GetChild(i));
		}
	}

	void FbxModelLoader::LoadFbxMesh(FbxMesh* mesh) {
		this->modelData->modelHierarchy.emplace_back();
		int currentHierarchy = (int)this->modelData->modelHierarchy.size() - 1;

		K_Graphics::Vertex* vertex = nullptr;
		PolygonTable* table = nullptr;
		try {
			this->numVertex = mesh->GetControlPointsCount();
			this->numFace = mesh->GetPolygonCount();
			this->numUV = mesh->GetTextureUVCount();

			vertex = new K_Graphics::Vertex[this->numVertex];

			//���_
			LoadVertex(mesh, vertex);

			//�{�[��
			std::vector<K_Graphics::Bone> bone;
			this->LoadBones(mesh, vertex, this->modelData->modelHierarchy[currentHierarchy].boneData, table);

			//UV�x�[�X�Œ��_����蒼��
			VertexUVs uvMap;
			int newNumVertex;
			if (this->numUV) {
				newNumVertex = CreateUVBaseVertex(mesh, uvMap);
				this->modelData->modelHierarchy[currentHierarchy].vertexData.reserve(newNumVertex);
				int count = 0;
				for (int i = 0; i < uvMap.GetSize(); ++i) {
					for (int j = 0; j < uvMap[i].uv.GetCount(); ++j) {
						//K3MD�`���f�[�^
						this->modelData->modelHierarchy[currentHierarchy].vertexData.push_back(vertex[i]);
						this->modelData->modelHierarchy[currentHierarchy].vertexData[count].texcoord.x = (float)uvMap[i].uv[j][0];
						this->modelData->modelHierarchy[currentHierarchy].vertexData[count].texcoord.y = (float)uvMap[i].uv[j][1];
						++count;
					}
				}
			}
			else {
				newNumVertex = this->numVertex;
				this->modelData->modelHierarchy[currentHierarchy].vertexData.reserve(newNumVertex);
				for (int i = 0; i < this->numVertex; ++i) {
					//K3MD�`���f�[�^
					this->modelData->modelHierarchy[currentHierarchy].vertexData.push_back(vertex[i]);
				}
			}

			//�}�e���A���Ƃ��̒��_�C���f�b�N�X
			LoadMaterial(mesh, uvMap, this->modelData->modelHierarchy[currentHierarchy].materialData);

			if (table) {
				delete[] table;
			}
			delete[] vertex;
		}
		catch (std::exception& e) {
			if (vertex) {
				delete[] vertex;
			}
			if (table) {
				delete[] table;
			}
			Finalize();
			//��O�����͏�ɔC����
			throw e;
		}
	}

	void FbxModelLoader::Finalize() {
		if (this->fbxData != nullptr) {
			delete this->fbxData;
			this->fbxData = nullptr;
		}
		if (this->modelData != nullptr) {
			delete this->modelData;
			this->modelData = nullptr;
		}
	}


	//�ʃx�[�X�Ń��b�V���ǂݍ���
	void FbxModelLoader::LoadVertex(FbxMesh* mesh, K_Graphics::Vertex* vertex) {
		printf("vertex:%d face:%d uv:%d\n", this->numVertex, this->numFace, this->numUV);

		for (int i = 0; i < this->numFace; ++i) {
			//�O�p�|���S���̈꒸�_���i�[
			for (int p = 0; p < 3; ++p) {
				//���_�C���f�b�N�X��ʂ��瓾��
				int polygonCount = mesh->GetPolygonVertexIndex(i);
				int *polygonVertex = mesh->GetPolygonVertices();
				int vertexIndex = polygonVertex[polygonCount + p];

				//���_
				FbxVector4 *pCoord = mesh->GetControlPoints();
				int index = mesh->GetPolygonVertex(i, p);
				vertex[vertexIndex].position.x = (float)pCoord[index][0];
				vertex[vertexIndex].position.y = (float)pCoord[index][2];
				vertex[vertexIndex].position.z = (float)pCoord[index][1];

				//�@��
				FbxVector4 normal;
				mesh->GetPolygonVertexNormal(i, p, normal);
				if (glm::length(vertex[vertexIndex].normal) == 0.0f) {
					vertex[vertexIndex].normal.x = (float)normal[0];
					vertex[vertexIndex].normal.y = (float)normal[2];
					vertex[vertexIndex].normal.z = (float)normal[1];
					vertex[vertexIndex].normal = glm::normalize(vertex[vertexIndex].normal);
				}

				//UV
				if (!numUV) {
					continue;
				}
				FbxLayerElementUV *pUV;
				pUV = mesh->GetLayer(0)->GetUVs();
				if (pUV->GetMappingMode() == FbxLayerElementUV::eByPolygonVertex) {
					int uvIndex = mesh->GetTextureUVIndex(i, p, FbxLayerElement::eTextureDiffuse);
					FbxVector2 v2 = pUV->GetDirectArray().GetAt(uvIndex);
					vertex[vertexIndex].texcoord.x = (float)v2[0];
					vertex[vertexIndex].texcoord.y = (float)v2[1];
				}
			}
		}


		//UV�^�C�v��eByControlPoint�̎�
		FbxVector2 v2;
		FbxLayerElementUV *pUV;
		pUV = mesh->GetLayer(0)->GetUVs();
		if (numUV) {
			if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint) {
				pUV = mesh->GetLayer(0)->GetUVs();
				for (int k = 0; k < numUV; ++k) {
					v2 = pUV->GetDirectArray().GetAt(k);
					vertex[k].texcoord.x = (float)v2[0];
					vertex[k].texcoord.y = (float)v2[1];
				}
			}
		}
	}
	
	//�}�e���A���ǂݍ���
	void FbxModelLoader::LoadMaterial(FbxMesh* mesh, VertexUVs& vertexData, std::vector<K3MDMaterial>& material) {
		FbxNode* node = mesh->GetNode();
		int numMaterial = node->GetMaterialCount();

		material.resize(numMaterial);

		for (int i = 0; i < numMaterial; ++i) {
			FbxSurfaceMaterial *pMaterial = node->GetMaterial(i);
			FbxClassId materialType = pMaterial->GetClassId();
			
			if (materialType.Is(FbxSurfacePhong::ClassId)) {
				FbxSurfacePhong *pPhong = (FbxSurfacePhong*)pMaterial;
				//����
				for (int j = 0; j < 3; ++j) {
					material[i].ambient[j] = (float)(pPhong->Ambient.Get()[j]);
				}
				material[i].ambient[3] = 1.0f;
				//�������x
				material[i].ambientPower = (float)(pPhong->AmbientFactor.Get());

				//�g�U��
				for (int j = 0; j < 3; ++j) {
					material[i].diffuse[j] = (float)(pPhong->Diffuse.Get()[j]);
				}
				material[i].diffuse[3] = 1.0f;

				//���ʔ���
				for (int j = 0; j < 3; ++j) {
					material[i].specular[j] = (float)(pPhong->Specular.Get()[j]);
				}
				material[i].specular[3] = 1.0f;

				//���ʔ��ˋ��x
				material[i].specurarShininess = (float)(pPhong->Shininess.Get());
				material[i].specurarPower = (float)(pPhong->SpecularFactor.Get());
			}
			else if (materialType.Is(FbxSurfaceLambert::ClassId)) {
				FbxSurfaceLambert *pLambert = (FbxSurfaceLambert*)pMaterial;

				//����
				for (int j = 0; j < 3; ++j) {
					material[i].ambient[j] = (float)(pLambert->Ambient.Get()[j]);
				}
				material[i].ambient[3] = 1.0f;
				//�������x
				material[i].ambientPower = (float)(pLambert->AmbientFactor.Get());

				//�g�U��
				for (int j = 0; j < 3; ++j) {
					material[i].diffuse[j] = (float)(pLambert->Diffuse.Get()[j]);
				}
				material[i].diffuse[3] = 1.0f;

				//���ʔ���
				for (int j = 0; j < 3; ++j) {
					material[i].specular[j] = 0.0f;
				}
				material[i].specular[3] = 1.0f;

				//���ʔ��ˋ��x
				material[i].specurarShininess = 1.0f;
				material[i].specurarPower = 0.0f;
			}

			//�e�N�X�`��
			FbxProperty lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse, 0);
			FbxFileTexture *pTexture = FbxCast<FbxFileTexture>(lProperty.GetSrcObject<FbxFileTexture>(0));
			if (!pTexture) {
				material[i].texturePath = "";
			}
			else {
				const char *fullName = pTexture->GetRelativeFileName();


				//Blender����ǂݎ�������΃p�X�̃f�B���N�g��
				char directory[100] = "";
				//Blender����ǂݎ�������O
				char name[100] = "";
				//�g���q
				char ext[10] = "";
				//�t�@�C�������擾(�t�@�C�����Ɗg���q�̂�)
				_splitpath_s(fullName, 0, 0, directory, 100, name, 100, ext, 10);

				int pathSize = (int)this->fileRoot.size() + (int)strlen(directory) + (int)strlen(name) + (int)strlen(ext) + 10;

				//�ŏI�I�Ɏg�p����t�@�C����
				char* fileName = new char[pathSize];
				fileName[0] = '\0';

				strcat_s(fileName, pathSize, this->fileRoot.data());
				strcat_s(fileName, pathSize, directory);
				strcat_s(fileName, pathSize, name);
				strcat_s(fileName, pathSize, ext);

				material[i].texturePath = fileName;

				printf("Texture : %s\n", fileName);
				delete[] fileName;
			}

			//�C���f�b�N�X�o�b�t�@
			int indexCount = 0;
			int *pIndex = new int[numFace * 3];
			material[i].vertexIndex.reserve(numFace * 3);
			for (int k = 0; k < numFace; ++k) {
				FbxLayerElementMaterial *fbxmaterial = mesh->GetLayer(0)->GetMaterials();

				int matId = fbxmaterial->GetIndexArray().GetAt(k);
				if (matId == i) {
					for (int p = 0; p < 3; ++p) {
						int index = mesh->GetPolygonVertex(k, p);

						//���ʗp��UV���擾
						FbxLayerElementUV* uv = mesh->GetLayer(0)->GetUVs();
						int arrPos = -1;
						if (uv) {
							int uvIndex = mesh->GetTextureUVIndex(k, p, FbxLayerElement::eTextureDiffuse);
							FbxVector2 v2 = uv->GetDirectArray().GetAt(uvIndex);
							arrPos = vertexData[index].uv.Find(v2);
						}
						if (arrPos == -1) {
							//UV�������Ȃ��ꍇ�͂�����ʂ�
							pIndex[indexCount + p] = index;
							material[i].vertexIndex.push_back(index);
						}
						else {
							//UV�ɂ���ăT�u���b�V������̒��_�C���f�b�N�X�����
							pIndex[indexCount + p] = vertexData[index].index[arrPos];
							material[i].vertexIndex.push_back(vertexData[index].index[arrPos]);
						}
					}
					indexCount += 3;
				}
			}
			delete[] pIndex;
		}
	}

	//���_��UV���W��S�Ċi�[���A���̑��v�𒸓_���Ƃ��ĕԂ�
	int FbxModelLoader::CreateUVBaseVertex(FbxMesh* mesh, VertexUVs& uvMap) {
		if (!this->numUV) {
			return 0;
		}
		FbxLayerElementUV *pUV = mesh->GetLayer(0)->GetUVs();
		if (pUV->GetMappingMode() == FbxLayerElementUV::eByPolygonVertex) {
			for (int i = 0; i < this->numFace; ++i) {
				for (int j = 0; j < 3; ++j) {
					//���_�C���f�b�N�X��ʂ��瓾��
					int polygonCount = mesh->GetPolygonVertexIndex(i);
					int *polygonVertex = mesh->GetPolygonVertices();
					int vertexIndex = polygonVertex[polygonCount + j];

					//UV�𒲂ׂ�
					int uvIndex = mesh->GetTextureUVIndex(i, j, FbxLayerElement::eTextureDiffuse);
					FbxVector2 v2 = pUV->GetDirectArray().GetAt(uvIndex);
					if (uvMap[vertexIndex].uv.Find(v2) == -1) {
						uvMap[vertexIndex].uv.Add(v2);
					}
				}
			}
		}
		else if (pUV->GetMappingMode() == FbxLayerElementUV::eByControlPoint) {
			for (int k = 0; k < numUV; ++k) {
				FbxVector2 v2 = pUV->GetDirectArray().GetAt(k);
				uvMap[k].uv.Add(v2);
			}
		}

		//�����𒲂ׂ�
		int numVertexInUV = 0;
		for (int i = 0; i < uvMap.GetSize(); ++i) {
			for (int j = 0; j < uvMap[i].uv.GetCount(); ++j) {
				//�V���Ȓ��_�C���f�b�N�X���L�^
				uvMap[i].index.Add(numVertexInUV);
				++numVertexInUV;
			}
		}
		return numVertexInUV;
	}


	//���_���ǂ̃|���S���ɑ����邩���L�^
	FbxModelLoader::PolygonTable* FbxModelLoader::CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace) {
		PolygonTable *pt = new PolygonTable[numVertex];
		for (int vertex = 0; vertex < numVertex; ++vertex) {
			for (int face = 0; face < numFace; ++face) {
				for (int polygon123 = 0; polygon123 < 3; ++polygon123) {

					if (mesh->GetPolygonVertex(face, polygon123) != vertex) {
						continue;
					}
					pt[vertex].polygonIndex[pt[vertex].numPolygon] = face;
					pt[vertex].polygon123[pt[vertex].numPolygon] = polygon123;
					++pt[vertex].numPolygon;

				}
			}
		}
		return pt;
	}

	void FbxModelLoader::LoadBones(FbxMesh* mesh, K_Graphics::Vertex* vertex, std::vector<K3MDBone>& bone, PolygonTable *table) {
		FbxDeformer *deformer = mesh->GetDeformer(0);
		if (!deformer) {
			//�{�[�������݂��Ȃ�������A��
			return;
		}

		FbxSkin* skin = (FbxSkin*)deformer;

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
		bone.resize(numBone);
		for (int i = 0; i < numBone; ++i) {
			FbxAMatrix mat;
			K_Math::Matrix4x4 mat44;
			cluster[i]->GetTransformLinkMatrix(mat);
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					mat44[y][x] = (float)mat.Get(y, x);
				}
			}
			mat44 = glm::inverse(mat44);
	
			glm::quat rot = glm::angleAxis(K_Math::DegToRad(90.0f), K_Math::Vector3(1.0f, 0.0f, 0.0f));
			K_Math::Matrix4x4 scale = glm::scale(K_Math::Matrix4x4(), K_Math::Vector3(1.0f, -1.0f, 1.0f));
			mat44 = mat44 * scale * glm::toMat4(rot);

			for (int lineIndex = 0; lineIndex < 4; ++lineIndex) {
				bone[i].bindMat.line[lineIndex] = mat44[lineIndex];
			}
		}
		CalcCurrentBoneMatrix(bone, cluster);
	}



	void FbxModelLoader::InitializeFBX(const std::string& fileName) {
		size_t pathSize = 0;
		char* path;
		try {
			FbxAnsiToUTF8(fileName.data(), path, &pathSize);

			//�}�l�[�W���[�𐶐������f���f�[�^���C���|�[�g
			FbxManager  *manager;
			FbxImporter *importer;
			FbxScene    *scene;
			manager = FbxManager::Create();
			if (!manager) {
				throw std::runtime_error("FBXSDK initialize failed " + fileName);
			}

			importer = FbxImporter::Create(manager, "");
			scene = FbxScene::Create(manager, "");

			this->fbxData->Add(manager, importer, scene);

			if (!importer || !scene) {
				throw std::runtime_error("FBXSDK initialize failed " + fileName);
			}

			//�������ƃC���|�[�g
			printf("%s\n", fileName.data());
			if (!importer->Initialize(path)) {

				throw std::runtime_error("FBXSDK file load failed " + std::string(importer->GetStatus().GetErrorString()) + " " + fileName);
			}
			if (!importer->Import(scene)) {
				throw std::runtime_error("FBXSDK import failed " + fileName);
			}

			//�ʂ��O�p���A�]�v�Ȗʂ���菜��
			FbxGeometryConverter converter(manager);
			converter.Triangulate(scene, true);
			converter.RemoveBadPolygonsFromMeshes(scene);
			//�ϊ������p�X�̕�����͉�����Ȃ��Ă͂Ȃ�Ȃ��̂ŉ��
			FbxFree(path);
		}
		catch (std::exception& e) {
			if (pathSize > 0) {
				//�ϊ������p�X�̕�����͉�����Ȃ��Ă͂Ȃ�Ȃ��̂ŉ��
				FbxFree(path);
			}
			//�㏈�����I�����̂ŗ�O���܂�������
			throw e;
		}
	}

	void FbxModelLoader::CalcCurrentBoneMatrix(std::vector<K3MDBone>& bone, std::vector<FbxCluster*>& cluster) {
		//�A�j���[�V�������ƂɃ{�[���s���ۑ�
		FbxImporter* importer = this->fbxData->GetInporter();
		int numAnim = importer->GetAnimStackCount();

		for (int animID = 0; animID < numAnim; ++animID) {
			//�擾
			FbxTakeInfo *take = importer->GetTakeInfo(animID);
			auto start = take->mLocalTimeSpan.GetStart();
			auto end = take->mLocalTimeSpan.GetStop();

			int startTime = (int)(start.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));
			int endTime = (int)(end.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));



			//�܂����݂̃A�j���[�V�������Z�b�g
			FbxAnimStack *pStack = this->fbxData->GetScene()->GetSrcObject<FbxAnimStack>(animID);
			this->fbxData->GetScene()->SetCurrentAnimationStack(pStack);

			//�s���1�t���[��(60fps)���擾
			for (int time = startTime; time < endTime; ++time) {
				FbxTime fbxTime;
				fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);

				int numBone = (int)bone.size();
				for (int k = 0; k < numBone; ++k) {
					bone[k].mat.resize(numAnim);
					bone[k].mat[animID].resize(endTime + startTime);
					//�s��擾
					FbxAMatrix& mat = cluster[k]->GetLink()->EvaluateGlobalTransform(fbxTime);
					K_Math::Matrix4x4 mat44;
					//���
					for (int x = 0; x < 4; ++x) {
						for (int y = 0; y < 4; ++y) {
							mat44[y][x] = (float)mat.Get(y, x);
						}
					}
					for (int lineIndex = 0; lineIndex < 4; ++lineIndex) {
						bone[k].mat[animID][time].line[lineIndex] = mat44[lineIndex];
					}
				}
			}

		}
	}

}
