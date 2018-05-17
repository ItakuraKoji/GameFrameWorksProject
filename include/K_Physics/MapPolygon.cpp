#include"MapPolygon.h"

namespace K_Physics {

	////////
	//public
	////
	MapPolygon::MapPolygon(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask) {
		if (!Initialize(fbxFilePath, physics, myselfMask, giveMask)) {
			throw("meshData Load Failed" + std::string(fbxFilePath));
		}
	}
	MapPolygon::~MapPolygon() {
		Finalize();
	}

	//�z��̃[���N���A
	bool MapPolygon::Initialize(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask) {
		Finalize();
		this->polygonStack.clear();
		this->polygonStack.shrink_to_fit();

		if (!LoadModel(fbxFilePath)) {
			return false;
		}
		SetCollisionWorld(physics, myselfMask, giveMask);
		return true;
	}
	//���
	void MapPolygon::Finalize() {
		for (auto it = this->polygonStack.begin(); it < this->polygonStack.end(); ++it) {
			delete[](*it).polygon;
		}
		//���������͎����ŊJ�����Ȃ��Ƃ����Ȃ�
		if (this->collisionMesh != nullptr) {
			delete collisionMesh;
			this->collisionMesh = nullptr;
		}
	}



	//�|���S�������l��
	int MapPolygon::GetNumFace() {
		return numFace;
	}


	K_Physics::RigidBodyData* MapPolygon::GetRigidBody() {
		return this->rigid;
	}

	void MapPolygon::SetScaling(const K_Math::Vector3& scale) {
		this->shape->setLocalScaling(btVector3(scale.x(), scale.y(), scale.z()));
	}


	////////
	//private
	////

	bool MapPolygon::LoadModel(const char *filename) {
		FbxManager* manager = nullptr;
		FbxScene* scene = nullptr;
		if (!InitializeFBX(&manager, &scene, filename)) {
			FinalizeFBX(&manager);
			return false;
		}

		if (!LoadFBXNodeRecursive(scene->GetRootNode())) {
			FinalizeFBX(&manager);
			return false;
		}

		FinalizeFBX(&manager);
		return true;
	}

	bool MapPolygon::InitializeFBX(FbxManager** manager, FbxScene** scene, const char* filename) {
		FbxImporter *importer;
		(*manager) = FbxManager::Create();
		if ((*manager) == nullptr) {
			return false;
		}
		importer = FbxImporter::Create((*manager), "");
		if (importer == nullptr) {
			return false;
		}
		(*scene) = FbxScene::Create((*manager), "");
		if (scene == nullptr) {
			return false;
		}
		if (!importer->Initialize(filename)) {
			return false;
		}
		if (!importer->Import((*scene))) {
			return false;
		}
		//�ʂ��O�p���A�]�v�Ȗʂ���菜��
		FbxGeometryConverter converter((*manager));
		converter.Triangulate((*scene), true);
		converter.RemoveBadPolygonsFromMeshes((*scene));
		//�C���|�[�^�[�͂�������Ȃ�
		importer->Destroy();
		return true;
	}

	void MapPolygon::FinalizeFBX(FbxManager** manager) {
		if ((*manager) != nullptr) {
			(*manager)->Destroy();
			(*manager) = nullptr;
		}
	}


	bool MapPolygon::LoadFBXNodeRecursive(FbxNode *node) {
		FbxNodeAttribute* attr;
		//�m�[�h�������擾
		attr = node->GetNodeAttribute();
		if (attr != NULL) {
			FbxNodeAttribute::EType type;
			type = attr->GetAttributeType();
			if (type == FbxNodeAttribute::eMesh) {
				if (!LoadFBX(node->GetMesh())) {
					return false;
				}
			}
		}
		//�q������Ȃ�q�ōċN
		int numChild = node->GetChildCount();
		for (int i = 0; i < numChild; ++i) {
			FbxNode* child = node->GetChild(i);
			if (!LoadFBXNodeRecursive(child)) {
				return false;
			}
		}
		return true;
	}

	//FBX���璸�_�𔲂��o���A����p�Ȃ̂ł����܂Œ��_���W�̂�
	bool MapPolygon::LoadFBX(FbxMesh *mesh) {
		int numFace = mesh->GetPolygonCount();
		int numVertex = mesh->GetControlPointsCount();

		//�|���S���̃f�[�^
		PolygonData data;
		data.polygon = new PolygonType[numFace];
		data.numPolygon = numFace;

		FbxVector4 *pCoord = mesh->GetControlPoints();

		//UV�x�[�X�̓ǂݍ��݂͑z�肵�Ȃ�
		for (int i = 0; i < numFace; ++i) {
			int  polygonCount = mesh->GetPolygonVertexIndex(i);
			int* vertexIndex = mesh->GetPolygonVertices();

			int polygonIndices[3] = {};
			polygonIndices[0] = vertexIndex[polygonCount];
			polygonIndices[1] = vertexIndex[polygonCount + 1];
			polygonIndices[2] = vertexIndex[polygonCount + 2];

			for (int k = 0; k < 3; ++k) {
				//�u�����_�[�̃��f���͂Ȃ����X�O�x�X���̂ō��W�������܂��ݒ肵�Ă���Ă�A���������
				int index = mesh->GetPolygonVertex(i, k);
				data.polygon[i].point[k][0] = (float)-pCoord[index][0];
				data.polygon[i].point[k][1] = (float)pCoord[index][1];
				data.polygon[i].point[k][2] = (float)pCoord[index][2];
			}
		}
		polygonStack.push_back(data);
		this->numFace = numFace;
		return true;
	}


	//bullet�ɒn�`�̎O�p���b�V�������̂Ƃ��Ēǉ�
	void MapPolygon::SetCollisionWorld(BulletPhysics *physics, int myselfMask, int giveMask) {
		std::vector<K_Math::Vector3> vectices;
		for (int i = 0; i < this->numFace; ++i) {
			for (int k = 0; k < 3; ++k) {
				vectices.push_back(polygonStack[0].polygon[i].point[k]);
			}
		}

		this->collisionMesh = physics->CreateTriangleMesh(vectices.data(), this->numFace);
		this->shape = physics->CreateTriangleMeshShape(this->collisionMesh);
		this->rigid = physics->CreateRigidBody(shape, 0.0f, false, myselfMask, giveMask, K_Math::Vector3(0.0f, 0.0f, 0.0f));
		this->rigid->SetCollisionRotation(K_Math::Vector3(K_Math::DegToRad(-90.0), 0.0f, 0.0f));
	}

}
