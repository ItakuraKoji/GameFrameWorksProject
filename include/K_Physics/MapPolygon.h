#pragma once

#include<fbxsdk\fbxsdk.h>

#include<vector>

#include"K_Math\MyMathFanctions.h"
#include"BulletPhysics.h"

namespace K_Physics {

	//!@brief ����p���f���̃|���S���������N���X�A�`��͈�؂ł��Ȃ��B�s��ɂ���]�ɂ��Ή����Ă��Ȃ�
	class MapPolygon {
	public:
		//struct PolygonType{
		//	Eigen::Vector3f point[3];
		//};
		//struct PolygonData {
		//	int numPolygon;
		//	PolygonType* polygon;
		//};

		//bullet�p
		struct PolygonType {
			K_Math::Vector3 point[3];
		};
		struct PolygonData {
			int numPolygon;
			PolygonType* polygon;
		};


	public:
		MapPolygon();
		~MapPolygon();
		bool Initialize();
		void Finalize();
		//!@brief FBX�ǂݍ���
		//!@param[in] filename �t�@�C���̃p�X
		//!@return ���������true
		bool LoadModel(const char *filename);
		//!@brief �ǂݍ��񂾃f�[�^�����Ƃɒn�`�R���W�������쐬
		//!@param[in] physics �R���W�������Ǘ����镨���N���X�ւ̃|�C���^
		void SetCollisionWorld(BulletPhysics *physics, int myselfMask, int giveMask);
		//!@brief �R���W�������g��
		//!@param[in] scale �g�k�̔{��
		void SetScaling(const K_Math::Vector3& scale);
		//!@return �ǂݍ��񂾃��f���̃|���S����
		int GetNumFace();
		//!@return ���̃I�u�W�F�N�g���擾
		K_Physics::RigidBodyData* GetRigidBody();

	private:
		bool InitializeFBX(FbxManager** manager, FbxScene** scene, const char* filename);
		void FinalizeFBX(FbxManager** manager);
		bool LoadFBXNodeRecursive(FbxNode *node);
		bool LoadFBX(FbxMesh *mesh);


	private:
		//����p�̒��_�z��
		std::vector<PolygonData> polygonStack;

		int numFace;

		btTriangleMesh* collisionMesh;
		K_Physics::RigidBodyData* rigid;
		K_Physics::CollisionShape* shape;
	};
}
