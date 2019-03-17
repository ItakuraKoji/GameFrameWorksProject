#include"MapPolygon.h"
#include"K_Graphics/FbxModelLoader.h"

namespace K_Physics {

	////////
	//public
	////
	MapPolygon::MapPolygon(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask) {
		this->collisionMesh = nullptr;

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
		this->polygon.polygon.clear();

		this->physics = physics;

		//���f���ǂݍ���
		LoadModel(fbxFilePath);

		//�R���W�����쐬
		SetCollisionWorld(physics, this->polygon, myselfMask, giveMask);

		return true;
	}
	bool MapPolygon::Initialize(const K_Loader::K3MDHierarchy* modelData, BulletPhysics *physics, int myselfMask, int giveMask) {
		Finalize();
		this->polygon.polygon.clear();

		this->physics = physics;

		//���f���ǂݍ���
		LoadModel(modelData);

		//�R���W�����쐬
		SetCollisionWorld(physics, this->polygon, myselfMask, giveMask);

		return true;
	}


	//���
	void MapPolygon::Finalize() {
		//�R���W���������[���h�����菜��
		if (this->collisionMesh != nullptr) {
			delete collisionMesh;
			this->collisionMesh = nullptr;
			this->physics->RemoveCollision(&this->rigid);
		}
	}

	K_Physics::RigidBodyData* MapPolygon::GetRigidBody() {
		return this->rigid;
	}

	void MapPolygon::SetScaling(const K_Math::Vector3& scale) {
		this->shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}


	////////
	//private
	////

	bool MapPolygon::LoadModel(const char *filename) {
		K_Loader::FbxModelLoader loader;
		loader.LoadFBX(filename);
		auto modelData = loader.GetModelData();
		LoadModel(modelData);
		return true;
	}

	bool MapPolygon::LoadModel(const K_Loader::K3MDHierarchy* modelData) {
		int counter = 0;

		//���b�V���z��
		for (auto& hierarchy : modelData->modelHierarchy) {
			//�}�e���A��
			for (auto& material : hierarchy.materialData) {
				PolygonType vertexPos;
				//���_�C���f�b�N�X����|���S���쐬
				for (auto& index : material.vertexIndex) {
					vertexPos.point[counter] = hierarchy.vertexData[index].position;
					++counter;
					//�O�p�|���S������o���オ������z��ɒǉ�
					if (counter > 2) {
						counter = 0;
						this->polygon.polygon.push_back(vertexPos);
					}
				}
			}
		}
		return true;
	}


	//bullet�ɒn�`�̎O�p���b�V�������̂Ƃ��Ēǉ�
	void MapPolygon::SetCollisionWorld(BulletPhysics *physics, const PolygonData& polygonData, int myselfMask, int giveMask) {
		std::vector<K_Math::Vector3> vectices;
		for (int i = 0; i < polygonData.polygon.size(); ++i) {
			for (int k = 0; k < 3; ++k) {
				vectices.push_back(polygonData.polygon[i].point[k]);
			}
		}

		this->collisionMesh = physics->CreateTriangleMesh(vectices.data(), (int)polygonData.polygon.size());
		this->shape = physics->CreateTriangleMeshShape(this->collisionMesh);
		this->rigid = physics->CreateRigidBody(shape, 0.0f, false, myselfMask, giveMask, K_Math::Vector3(0.0f, 0.0f, 0.0f));
	}

}
