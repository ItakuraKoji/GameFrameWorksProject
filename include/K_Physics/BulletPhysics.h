#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<bullet\BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include<bullet\BulletCollision\CollisionDispatch\btGhostObject.h>
#include"K_Graphics\ShaderClass.h"
#include"K_Graphics\CameraClass.h"
#include"bulletDebugDraw.h"
#include"K_Math\MyMathFanctions.h"

#include<iostream>
#include<vector>

namespace K_Physics {

	///@brief �Փˎ��ɕԂ��^�O���
	struct CollisionTag {
		std::string tagName;
		int tagIndex;
		void* userData;
	};

	///@brief �R���W�����̈ړ��@�\�����A�v��bullet�̃R���W�������������₷�������N���X
	class CollisionData {
	public:
		CollisionData(btCollisionObject* obj, int mask, CollisionTag tag);
		void SetCollisionPosition(const K_Math::Vector3& position);
		K_Math::Vector3 GetCollisionPosition();

	public:
		btCollisionObject* const collision;
		CollisionTag tag;
		const int mask;
	};

	///@brief bullet�̃��[���h���Ǘ�����N���X\n�R���W�����̐����E����֐���񋟂��Ă���
	class BulletPhysics {
	public:

	public:
		///@brief Initialize()���Ă�
		BulletPhysics();
		///@brief Finalize()���Ă�
		~BulletPhysics();
		///@brief bullet�̏��������s��
		bool Initialize();
		///@brief bullet�̏I���������s��
		void Finalize();

		///@brief bullet�̕������E���X�V���č��W��ύX����\n�v���C���[�̒n�`����֐��͕ʋ쓮�Ȃ̂Ńt���[���̏��߂ɌĂԂƂ悢
		void Run();
		///@brief bullet�̃R���W��������������
		///@param[in] shader �g�p����V�F�[�_�[
		///@param[in] camera �g�p����J����
		///@param[in] trans �ό`�s��i�ȗ����P�ʍs��j
		void DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans = K_Math::Matrix4x4::Identity());

		///@brief �O�p�`�̌`����쐬
		///@param[in] point1 �p�̍��W
		///@param[in] point2 �p�̍��W
		///@param[in] point3 �p�̍��W
		btCollisionShape* CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3);
		///@brief �|���S���̏W���`����쐬
		///@param[in] vectice �O�p�`�̊p���W�̔z��
		///@param[in] numFace �O�p�`�̐�
		btTriangleMesh* CreateTriangleMesh(K_Math::Vector3* vectice, int numFace);
		///@brief btTriangleMesh���烁�b�V���`����쐬
		///@param[in] mesh CreateTriangleMesh()���Ԃ��`��̃|�C���^
		btCollisionShape* CreateTriangleMeshShape(btTriangleMesh* mesh);
		///@brief ���̌`����쐬
		///@param[in] radius ���̔��a
		btCollisionShape* CreateSphereShape(float radius);
		///@brief �J�v�Z���̌`����쐬
		///@param[in] radius ���̔��a
		///@param[in] height �J�v�Z���̍���
		btCollisionShape* CreateCapsuleShape(float radius, float height);
		///@brief �����̂̌`����쐬
		///@param[in] halfWidth ���S����̒����̂̕�
		///@param[in] halfHeight ���S����̒����̂̍���
		///@param[in] halfDepth ���S����̒����̂̉��s��
		btCollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);


		///@brief ���̃I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
		///@param[in] shape ���̂̌`��ւ̃|�C���^
		///@param[in] mass ���̂̎���
		///@param[in] ghost �R���W���������̂ƏՓ˂��邩�̃t���O�itrue�ō��̂Ƃ͏Փ˂��Ȃ��j
		///@param[in] mask �Փ˃t�B���^�Ɏg���r�b�g�}�X�N
		///@param[in] pos ���̂̏����ʒu�i�ȗ����͂��ׂĂO�j
		///@param[in] rot ���̂̉�]�i�ȗ����͂��ׂĂO�j
		CollisionData* CreateRigidBody(btCollisionShape* shape, btScalar mass, bool ghost, int mask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));
		///@brief �R���W�����I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
		///@param[in] shape �R���W�����̌`��ւ̃|�C���^
		///@param[in] ghost �R���W���������̂ƏՓ˂��邩�̃t���O�itrue�ō��̂Ƃ͏Փ˂��Ȃ��j
		///@param[in] mask �Փ˃t�B���^�Ɏg���r�b�g�}�X�N
		///@param[in] pos �R���W�����̏����ʒu�i�ȗ����͂��ׂĂO�j
		///@param[in] rot �R���W�����̉�]�i�ȗ����͂��ׂĂO�j
		CollisionData* CreateCollisionObject(btCollisionShape* shape, bool ghost, int mask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));

		///@brief �����I�ɐ��E�ɓo�^���Ă��鍄�̂𐢊E����O���Ă���|�C���^��delete����\n���̃N���X�̃f�X�g���N�^�ɂĂ��̊֐��ɂ���đS�ĊJ�����Ă���
		void RemoveCollisionObject(btCollisionObject* rigidbody);

		///@brief �����I�Ƀ��X�g�ɑ��݂���`��������X�g����O����delete����
		///���̊֐����Ă΂Ȃ��Ă��A���̃N���X�̃f�X�g���N�^�őS�ĊJ�����Ă���
		///���̊֐��̌�́A�`��ւ̃|�C���^�͎g���Ȃ��̂Œ���
		void RemoveCollisionShape(btCollisionShape* shape);

		///@brief ���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s��(�d�������m)
		///@param[in] obj �ړ�����R���W�����I�u�W�F�N�g
		///@param[in] move �ړ��x�N�g��
		void MoveCharacter(btCollisionObject* obj, const K_Math::Vector3& move);

		///@brief ���U�I�ȃR���W�����̈ړ��A���肪 MoveCharacter ������G�c(�������y��)
		///@param[in] obj �ړ�����R���W�����I�u�W�F�N�g
		///@param[in] hMove ���ړ��x�N�g��
		///@param[in] vMove �c�ړ��x�N�g��
		void MoveCharacterDiscrete(btCollisionObject* obj, const K_Math::Vector3& hMove, const K_Math::Vector3& vMove);

		///@brief ���݂̕������E�ł̓���̃I�u�W�F�N�g�ɑ΂���Փ˂̃`�F�b�N
		///@param[in] �Փ˂��`�F�b�N�������I�u�W�F�N�g
		///@return �Փ˂��N�������I�u�W�F�N�g�̃^�O���
		std::vector<CollisionTag>& FindConfrictionObjects(btCollisionObject* myself);

		///@brief �d�͂Ƃ͔��Ε������w���P�ʃx�N�g����ݒ�A�L�����N�^�[�̈ړ��ɗ��p����
		///@param[in] vector �d�͂Ɣ��Ε����̒P�ʃx�N�g��
		void SetSkyVector(const K_Math::Vector3& vector);
	private:
		//�R���W�������ړ�
		void MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector);
		//�w������Ɉړ��i���U����j
		void MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection);
		//�w������Ɉړ��iSweep�g�p�j
		btVector3 MoveSmooth(btCollisionObject* obj, const btVector3& moveVector, float limitAngle, bool limitDirection);
		//�ړ��������܂Ƃ�
		btVector3 MoveBySweep(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection, float allowDistance = 0.2f);
	private:
		std::vector<CollisionTag> confrictResult;
		//��ւ̒P�ʃx�N�g���A�����蔻��Ŏg�p
		btVector3 toSkyVector;

		btDiscreteDynamicsWorld*                bulletWorld;
		btDefaultCollisionConfiguration*        config;
		btCollisionDispatcher*                  dispatcher;
		btConstraintSolver*                     solver;
		btBroadphaseInterface*                  broadphase;
		bulletDebugDraw                         debugDrawer;
		btAlignedObjectArray<btCollisionShape*> shapeArray;
	};


	////���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
	struct SweepTestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
	public:
		SweepTestCallBack(btCollisionObject *myself);

		//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
		//�߂�l�ɈӖ��͂Ȃ��݂���
		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);
	public:
		btCollisionObject * myself;

	};

	//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
	struct FixContactCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		FixContactCallBack(btCollisionObject* obj);
		//���������I�u�W�F�N�g���L�^
		//�߂�l�ɈӖ��͂Ȃ��݂���
		//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
		virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	public:
		//�����o���I�u�W�F�N�g
		btCollisionObject * obj;
		//�����̍ő�l�Ƃ��̕���
		float maxDistance;
		btVector3 fixVec;

		int count;
		bool isLoop;
	};

	//���ׂĂ̏Փ˂��L�^����
	struct CollectCollisionCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		CollectCollisionCallBack(std::vector<CollisionTag>& tagList);
		virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	public:
		std::vector<CollisionTag>& result;
		bool isHit;
	};

}