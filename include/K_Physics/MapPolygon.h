#pragma once

#include"K_Math\MyMathFanctions.h"
#include<vector>
#include"BulletPhysics.h"
#include"K_Graphics/K3MDLoader.h"

namespace K_Physics {

	//!@brief ����p���f���̃|���S���������N���X�A�`��͈�؂ł��Ȃ��B�s��ɂ���]�ɂ��Ή����Ă��Ȃ�
	class MapPolygon {
	public:
		//bullet�p
		struct PolygonType {
			K_Math::Vector3 point[3];
		};
		struct PolygonData {
			std::vector<PolygonType> polygon;
		};


	public:
		//!@brief FBX��ǂݍ��݃f�[�^�����Ƃɒn�`�R���W�������쐬
		//!@param[in] filename �t�@�C���̃p�X
		//!@param[in] physics �R���W�������Ǘ����镨���N���X�ւ̃|�C���^
		//!@param[in] myselfMask �������g���Փ˃}�X�N�A�ڂ����̓T���v���R�[�h�R�Q��
		//!@param[in] giveMask ����Ɏg�킹��Փ˃}�X�N�A�ڂ����̓T���v���R�[�h�R�Q��
		MapPolygon(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask);
		~MapPolygon();
		bool Initialize(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask);
		bool Initialize(const K_Loader::K3MDHierarchy* modelData, BulletPhysics *physics, int myselfMask, int giveMask);
		void Finalize();

		//!@brief �R���W�������g��
		//!@param[in] scale �g�k�̔{��
		void SetScaling(const K_Math::Vector3& scale);
		//!@return ���̃I�u�W�F�N�g���擾
		K_Physics::RigidBodyData* GetRigidBody();

	private:
		//!@brief FBX�ǂݍ���
		//!@param[in] filename �t�@�C���̃p�X
		//!@return ���������true
		bool LoadModel(const char *filename);
		bool LoadModel(const K_Loader::K3MDHierarchy* modelData);

		//!@brief �ǂݍ��񂾃f�[�^�����Ƃɒn�`�R���W�������쐬
		//!@param[in] physics �R���W�������Ǘ����镨���N���X�ւ̃|�C���^
		void SetCollisionWorld(BulletPhysics *physics, const PolygonData& polygonData, int myselfMask, int giveMask);


	private:
		//�A�N�Z�X�p�̕����N���X
		K_Physics::BulletPhysics* physics;

		//����p�̒��_�z��
		PolygonData polygon;

		btTriangleMesh* collisionMesh;
		K_Physics::RigidBodyData* rigid;
		K_Physics::CollisionShape* shape;
	};
}
