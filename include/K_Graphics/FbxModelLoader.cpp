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
		this->vertexData = nullptr;

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

		printf("LoadModel... : %s\n", fileName.data());
		if (!InitializeFBX(fileName)) {
			return false;
		}

		//ファイルパスを記録して相対パスを作り出す("../は使えない")
		{
			int position = 0;
			int loopMax = (int)fileName.size();
			int i;
			//後ろから数えて、パスを取得
			for (i = loopMax - 1; i >= 0; --i) {
				if (fileName.data()[i] == '\\' || fileName.data()[i] == '/') {
					position = i + 1;
					break;
				}
			}
			//取得したパスまでの位置を実際に文字取得
			for (i = 0; i < position; ++i) {
				this->fileRoot[i] = fileName.data()[i];
			}
			this->fileRoot[i] = '\0';
		}

		FbxNode *rootNode = this->fbxData->GetScene()->GetRootNode();
		if (!RecursiveNode(rootNode)) {
			return false;
		}
		this->loaded = true;
		return true;
	}

	//FBXポインタの譲渡
	K_Graphics::FbxData* FbxModelLoader::PassFbxData() {
		K_Graphics::FbxData* returnData = this->fbxData;
		this->fbxData = nullptr;
		return returnData;
	}
	//頂点バッファ情報の譲渡
	K_Graphics::VertexData* FbxModelLoader::PassVertexBuffer() {
		K_Graphics::VertexData* returnData = this->bufferData;
		this->bufferData = nullptr;
		return returnData;
	}
	//マテリアル情報の譲渡
	K_Graphics::MaterialData* FbxModelLoader::PassMaterialData() {
		K_Graphics::MaterialData* returnData = this->materialData;
		this->materialData = nullptr;
		return returnData;
	}
	//アニメーション情報の譲渡
	K_Graphics::AnimationData* FbxModelLoader::PassAnimationData() {
		K_Graphics::AnimationData* returnData = this->animationData;
		this->animationData = nullptr;
		return returnData;
	}
	//ボーン行列情報の譲渡
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
		//再起
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
			this->numVertex = mesh->GetControlPointsCount();
			this->numFace = mesh->GetPolygonCount();
			this->numUV = mesh->GetTextureUVCount();

			if (this->numUV > this->numVertex) {
				vertex = new Vertex[this->numUV];
			}
			else {
				vertex = new Vertex[this->numVertex];
			}

			//頂点
			LoadVertex(mesh, vertex);
			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//ボーン
			if (this->numUV > this->numVertex) {
				table = CreatePolygonTable(mesh, numVertex, numFace);
			}
			this->LoadBones(mesh, vertex, table);

			//UVベースで頂点を作り直す
			VertexUVs uvMap;
			int newNumVertex = CreateUVBaseVertex(mesh, uvMap);
			this->vertexData = new Vertex[newNumVertex];
			int count = 0;
			for (int i = 0; i < uvMap.GetSize(); ++i) {
				for (int j = 0; j < uvMap[i].uv.GetCount(); ++j) {
					this->vertexData[count] = vertex[i];
					this->vertexData[count].texcoord.x() = (float)uvMap[i].uv[j][0];
					this->vertexData[count].texcoord.y() = (float)uvMap[i].uv[j][1];
					++count;
				}
			}

			//マテリアルとその頂点インデックス
			FbxNode* node = mesh->GetNode();
			int numMaterial = node->GetMaterialCount();
			std::vector<K_Graphics::Material> material;
			std::vector<GLuint> IBOs;
			LoadMaterial(mesh, uvMap, material, IBOs);

			if (this->numUV > this->numVertex) {
				this->numVertex = numUV;
			}
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, newNumVertex * sizeof(Vertex), this->vertexData, GL_STATIC_DRAW);

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

			delete[] table;
			delete[] vertex;
			delete[] this->vertexData;
			this->vertexData = nullptr;
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
		if (this->vertexData != nullptr) {
			delete this->vertexData;
			this->vertexData = nullptr;
		}
	}


	//面ベースでメッシュ読み込み
	void FbxModelLoader::LoadVertex(FbxMesh* mesh, Vertex* vertex) {
		printf("vertex:%d face:%d uv:%d\n", this->numVertex, this->numFace, this->numUV);

		for (int i = 0; i < this->numFace; ++i) {
			//三角ポリゴンの一頂点ずつ格納
			for (int p = 0; p < 3; ++p) {
				int vertexIndex;
				//頂点インデックスを面から得る
				if (numUV > numVertex) {
					vertexIndex = mesh->GetTextureUVIndex(i, p, FbxLayerElement::eTextureDiffuse);
				}
				else {
					int polygonCount = mesh->GetPolygonVertexIndex(i);
					int *polygonVertex = mesh->GetPolygonVertices();
					vertexIndex = polygonVertex[polygonCount + p];
				}

				//頂点
				FbxVector4 *pCoord = mesh->GetControlPoints();
				int index = mesh->GetPolygonVertex(i, p);
				vertex[vertexIndex].position.x() = (float)pCoord[index][0];
				vertex[vertexIndex].position.y() = (float)pCoord[index][1];
				vertex[vertexIndex].position.z() = (float)pCoord[index][2];

				//法線
				FbxVector4 normal;
				mesh->GetPolygonVertexNormal(i, p, normal);
				vertex[vertexIndex].normal.x() = (float)normal[0];
				vertex[vertexIndex].normal.y() = (float)normal[1];
				vertex[vertexIndex].normal.z() = (float)normal[2];
				vertex[vertexIndex].normal.normalize();

				//UV
				if (!numUV) {
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


		//UVタイプがeByControlPointの時
		FbxVector2 v2;
		FbxLayerElementUV *pUV;
		pUV = mesh->GetLayer(0)->GetUVs();
		if (numUV) {
			if (pUV->GetMappingMode() == FbxLayerElement::eByControlPoint) {
				pUV = mesh->GetLayer(0)->GetUVs();
				for (int k = 0; k < numUV; ++k) {
					v2 = pUV->GetDirectArray().GetAt(k);
					vertex[k].texcoord.x() = (float)v2[0];
					vertex[k].texcoord.y() = (float)v2[1];
				}
			}
		}
	}
	
	//マテリアル読み込み
	void FbxModelLoader::LoadMaterial(FbxMesh* mesh, VertexUVs& vertexData, std::vector<K_Graphics::Material>& material, std::vector<GLuint>& IBOs) {
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
				//環境光強度
				material[i].ambientPower = (float)(pPhong->AmbientFactor.Get());

				material[i].diffuse(0) = (float)(pPhong->Diffuse.Get()[0]);
				material[i].diffuse(1) = (float)(pPhong->Diffuse.Get()[1]);
				material[i].diffuse(2) = (float)(pPhong->Diffuse.Get()[2]);
				material[i].diffuse(3) = 1.0f;

				material[i].specular(0) = (float)(pPhong->Specular.Get()[0]);
				material[i].specular(1) = (float)(pPhong->Specular.Get()[1]);
				material[i].specular(2) = (float)(pPhong->Specular.Get()[2]);
				material[i].specular(3) = 1.0f;

				//鏡面反射強度
				material[i].specurarShininess = (float)(pPhong->Shininess.Get());
				material[i].specurarPower = (float)(pPhong->SpecularFactor.Get());
			}
			else if (materialType.Is(FbxSurfaceLambert::ClassId)) {
				FbxSurfaceLambert *pLambert = (FbxSurfaceLambert*)pMaterial;

				material[i].ambient(0) = (float)(pLambert->Ambient.Get()[0]);
				material[i].ambient(1) = (float)(pLambert->Ambient.Get()[1]);
				material[i].ambient(2) = (float)(pLambert->Ambient.Get()[2]);
				material[i].ambient(3) = 1.0f;
				//環境光強度
				material[i].ambientPower = (float)(pLambert->AmbientFactor.Get());

				material[i].diffuse(0) = (float)(pLambert->Diffuse.Get()[0]);
				material[i].diffuse(1) = (float)(pLambert->Diffuse.Get()[1]);
				material[i].diffuse(2) = (float)(pLambert->Diffuse.Get()[2]);
				material[i].diffuse(3) = 1.0f;

				material[i].specular(0) = 0.0f;
				material[i].specular(1) = 0.0f;
				material[i].specular(2) = 0.0f;
				material[i].specular(3) = 1.0f;

				//鏡面反射強度
				material[i].specurarShininess = 1.0f;
				material[i].specurarPower = 0.0f;
			}

			//テクスチャ
			FbxProperty lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse, 0);
			FbxFileTexture *pTexture = FbxCast<FbxFileTexture>(lProperty.GetSrcObject<FbxFileTexture>(0));
			material[i].texture = nullptr;
			if (!pTexture) {
			}
			else {
				const char *fullName = pTexture->GetRelativeFileName();
				//最終的に使用するファイル名
				char fileName[120] = "";
				//Blenderから読み取った相対パスのディレクトリ
				char directory[100] = "";
				//Blenderから読み取った名前
				char name[100] = "";
				//拡張子
				char ext[10] = "";
				//ファイル名を取得(ファイル名と拡張子のみ)
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

			//インデックスバッファ
			int indexCount = 0;
			int *pIndex = new int[numFace * 3];
			for (int k = 0; k < numFace; ++k) {
				FbxLayerElementMaterial *material = mesh->GetLayer(0)->GetMaterials();

				int matId = material->GetIndexArray().GetAt(k);
				if (matId == i) {
					for (int p = 0; p < 3; ++p) {
						int index;
						if (numUV > numVertex) {
							index = mesh->GetTextureUVIndex(k, p, FbxLayerElement::eTextureDiffuse);
						}
						else {
							index = mesh->GetPolygonVertex(k, p);
						}
						//判別用にUVを取得
						FbxLayerElementUV* uv = mesh->GetLayer(0)->GetUVs();
						int uvIndex = mesh->GetTextureUVIndex(k, p, FbxLayerElement::eTextureDiffuse);
						FbxVector2 v2 = uv->GetDirectArray().GetAt(uvIndex);
						int arrPos = vertexData[index].uv.Find(v2);
						if (arrPos == -1) {
							//普通は通りえない
							pIndex[indexCount + p] = index;
						}
						else {
							//UVによってサブメッシュ化後の頂点インデックスを特定
							pIndex[indexCount + p] = vertexData[index].index[arrPos];
						}
					}

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

	//頂点のUV座標を全て格納し、その総計を頂点数として返す
	int FbxModelLoader::CreateUVBaseVertex(FbxMesh* mesh, VertexUVs& uvMap) {
		if (!this->numUV) {
			return 0;
		}
		FbxLayerElementUV *pUV = mesh->GetLayer(0)->GetUVs();
		if (pUV->GetMappingMode() == FbxLayerElementUV::eByPolygonVertex) {
			for (int i = 0; i < this->numFace; ++i) {
				for (int j = 0; j < 3; ++j) {
					int vertexIndex;
					//頂点インデックスを面から得る
					if (numUV > numVertex) {
						vertexIndex = mesh->GetTextureUVIndex(i, j, FbxLayerElement::eTextureDiffuse);
					}
					else {
						int polygonCount = mesh->GetPolygonVertexIndex(i);
						int *polygonVertex = mesh->GetPolygonVertices();
						vertexIndex = polygonVertex[polygonCount + j];
					}

					//UVを調べる
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

		//総数を調べる
		int numVertexInUV = 0;
		for (int i = 0; i < uvMap.GetSize(); ++i) {
			for (int j = 0; j < uvMap[i].uv.GetCount(); ++j) {
				//新たな頂点インデックスを記録
				uvMap[i].index.Add(numVertexInUV);
				++numVertexInUV;
			}
		}
		return numVertexInUV;
	}


	//頂点がどのポリゴンに属するかを記録
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

	bool FbxModelLoader::LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table) {
		FbxDeformer *deformer = mesh->GetDeformer(0);
		if (!deformer) {
			//ボーンが存在しなかったらアニメーション関連のデータに別れを告げる
			delete this->animationData;
			delete this->boneData;
			this->animationData = 0;
			this->boneData = 0;
			return false;
		}
		FbxSkin* skin = (FbxSkin*)deformer;

		int numBone = skin->GetClusterCount();
		std::vector<FbxCluster*> cluster;
		cluster.resize(numBone);
		for (int i = 0; i < numBone; ++i) {
			cluster[i] = skin->GetCluster(i);
		}

		if (this->numUV > this->numVertex) {
			//UVベース
			for (int i = 0; i < numBone; ++i) {
				int *indices = cluster[i]->GetControlPointIndices();
				int numIndices = cluster[i]->GetControlPointIndicesCount();
				double *weight = cluster[i]->GetControlPointWeights();

				for (int k = 0; k < numIndices; ++k) {
					for (int p = 0; p < table[indices[k]].numPolygon; ++p) {
						if (!weight[k]) {
							continue;
						}
						int polygonIndex = table[indices[k]].polygonIndex[p];
						int polygon123 = table[indices[k]].polygon123[p];
						int uvIndex = mesh->GetTextureUVIndex(polygonIndex, polygon123, FbxLayerElement::eTextureDiffuse);
						for (int m = 0; m < 4; ++m) {
							if (vertex[uvIndex].boneWeight[m] != 0.0f) {
								if (vertex[uvIndex].boneIndex.data[m] == i) {
									break;
								}
								continue;
							}
							vertex[uvIndex].boneIndex.data[m] = i;
							vertex[uvIndex].boneWeight[m] = (float)weight[k];
							break;
						}
					}
				}
			}
		}
		else {
			//頂点ベースのモデルの場合
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
		}

		//バインドポーズを取得
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

		//マネージャーを生成しモデルデータをインポート
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

		//初期化とインポート
		if (!importer->Initialize(fileName.data())) {
			return false;
		}
		if (!importer->Import(scene)) {
			return false;
		}

		//面を三角化、余計な面も取り除く
		FbxGeometryConverter converter(manager);
		converter.Triangulate(scene, true);
		converter.RemoveBadPolygonsFromMeshes(scene);

		//アニメーション情報を取得、名前をキーにして保持
		int numAnim = importer->GetAnimStackCount();
		for (int i = 0; i < numAnim; ++i) {
			//取得
			FbxTakeInfo *take = importer->GetTakeInfo(i);
			K_Graphics::AnimType anim;
			auto start = take->mLocalTimeSpan.GetStart();
			auto end = take->mLocalTimeSpan.GetStop();
			anim.animName = take->mName.Buffer();
			anim.animID = i;
			anim.startTime = (int)(start.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));
			anim.endTime = (int)(end.Get() / FbxTime::GetOneFrameValue(FbxTime::eFrames120));

			//追加
			this->animationData->Add(anim);
		}

		return true;
	}
}
