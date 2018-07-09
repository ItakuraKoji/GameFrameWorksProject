#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<bullet\BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include<bullet\BulletCollision\CollisionDispatch\btGhostObject.h>
#include"K_Graphics\ShaderClass.h"
#include"K_Graphics\CameraClass.h"
#include"bulletDebugDraw.h"
#include"K_Math\MyMathFanctions.h"

#include<vector>

namespace K_Physics {

	typedef btCollisionShape CollisionShape;

	//!@brief �Փˎ��ɕԂ��^�O���\n
	//!tagName : �^�O�̖��O �f�t�H���g�� "default"\n
	//!tagIndex : �^�O�ɕt�������鐔�� �f�t�H���g�� 0\n
	//!userData : �^�O�Ƌ��ɑ��肽�����ւ̃|�C���^ �f�t�H���g�� nullptr\n
	struct CollisionTag {
		std::string tagName;
		int tagIndex;
		void* userData;
	};

	//!@brief �R���W�����̈ړ��@�\�����A�v��bullet�̃R���W�������������₷�������N���X
	class CollisionData {
	public:
		CollisionData(btCollisionObject* obj, int myselfMask, int giveMask, CollisionTag tag);
		void SetCollisionPosition(const K_Math::Vector3& position);
		void SetCollisionRotation(const K_Math::Vector3& rotation);
		K_Math::Vector3 GetCollisionPosition();
		K_Math::Vector3 GetCollisionRotation();
		btCollisionObject* GetCollision();

		void SetMyselfMask(int mask);
		void SetGiveMask(int mask);

		int GetMyselfMask();
		int GetGiveMask();

	public:
		CollisionTag tag;
		
	protected:
		btCollisionObject* const collision;
		//!@brief �Փ˔��莞�Ɂu�������v�g���r�b�g�}�X�N
		int myselfMask;
		//!@brief �Փ˔��莞�Ɂu���肪�v�g���r�b�g�}�X�N
		int giveMask;
	};

	//!@brief CollisionData�ɉ����āA�����n�ɕK�v�Ȋ֐���ǉ������h���N���X
	class RigidBodyData : public CollisionData {
	public:
		RigidBodyData(btRigidBody* obj, int myselfMask, int giveMask, CollisionTag tag);
		void AddForce(const K_Math::Vector3& vector);
		void Activate(bool frag = true);
	};


	//!@brief bullet�̃��[���h���Ǘ�����N���X\n�R���W�����̐����E����֐���񋟂��Ă���
	class BulletPhysics {
	public:

	public:
		//!@brief Initialize()���Ă�
		BulletPhysics();
		//!@brief Finalize()���Ă�
		~BulletPhysics();
		//!@brief bullet�̏��������s��
		bool Initialize();
		//!@brief bullet�̏I���������s��
		void Finalize();

		//!@brief bullet�̕������E���X�V���č��W��ύX����\n�v���C���[�̒n�`����֐��͕ʋ쓮�Ȃ̂Ńt���[���̏��߂ɌĂԂƂ悢
		void Run();
		//!@brief bullet�̃R���W��������������
		//!@param[in] shader �g�p����V�F�[�_�[
		//!@param[in] camera �g�p����J����
		//!@param[in] trans �ό`�s��i�ȗ����P�ʍs��j
		void DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans = K_Math::Matrix4x4());
		void DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, float nearClip, float farClip, const K_Math::Matrix4x4& trans = K_Math::Matrix4x4());

		//!@brief �O�p�`�̌`����쐬
		//!@param[in] point1 �p�̍��W
		//!@param[in] point2 �p�̍��W
		//!@param[in] point3 �p�̍��W
		CollisionShape* CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3);
		//!@brief btTriangleMesh���烁�b�V���`����쐬
		//!@param[in] mesh CreateTriangleMesh()���Ԃ��`��̃|�C���^
		CollisionShape* CreateTriangleMeshShape(btTriangleMesh* mesh);
		//!@brief ���̌`����쐬
		//!@param[in] radius ���̔��a
		CollisionShape* CreateSphereShape(float radius);
		//!@brief �J�v�Z���̌`����쐬
		//!@param[in] radius ���̔��a
		//!@param[in] height �J�v�Z���̍���
		CollisionShape* CreateCapsuleShape(float radius, float height);
		//!@brief �����̂̌`����쐬
		//!@param[in] halfWidth ���S����̒����̂̕�
		//!@param[in] halfHeight ���S����̒����̂̍���
		//!@param[in] halfDepth ���S����̒����̂̉��s��
		CollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);
		//!@brief �|���S���̏W���`������쐬
		//!@param[in] vectice �O�p�`�̊p���W�̔z��
		//!@param[in] numFace �O�p�`�̐�
		btTriangleMesh* CreateTriangleMesh(K_Math::Vector3* vectice, int numFace);


		//!@brief ���̃I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
		//!@param[in] shape ���̂̌`��ւ̃|�C���^
		//!@param[in] mass ���̂̎���
		//!@param[in] ghost �R���W���������̂ƏՓ˂��邩�̃t���O�itrue�ō��̂Ƃ͏Փ˂��Ȃ��j
		//!@param[in] mask �Փ˃t�B���^�Ɏg���r�b�g�}�X�N
		//!@param[in] pos ���̂̏����ʒu�i�ȗ����͂��ׂĂO�j
		//!@param[in] rot ���̂̉�]�i�ȗ����͂��ׂĂO�j
		RigidBodyData* CreateRigidBody(btCollisionShape* shape, btScalar mass, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));
		//!@brief �R���W�����I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
		//!@param[in] shape �R���W�����̌`��ւ̃|�C���^
		//!@param[in] ghost �R���W���������̂ƏՓ˂��邩�̃t���O�itrue�ō��̂Ƃ͏Փ˂��Ȃ��j
		//!@param[in] mask �Փ˃t�B���^�Ɏg���r�b�g�}�X�N
		//!@param[in] pos �R���W�����̏����ʒu�i�ȗ����͂��ׂĂO�j
		//!@param[in] rot �R���W�����̉�]�i�ȗ����͂��ׂĂO�j
		CollisionData* CreateCollisionObject(btCollisionShape* shape, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));

		//!@brief �����I�ɐ��E�ɓo�^���Ă��鍄�̂𐢊E����O���Ă���|�C���^��delete����\n���̃N���X�̃f�X�g���N�^�ɂĂ��̊֐��ɂ���đS�ĊJ�����Ă���
		void RemoveCollision(CollisionData** collision);
		void RemoveCollision(RigidBodyData** rigidbody);

		//!@brief �����I�Ƀ��X�g�ɑ��݂���`��������X�g����O����delete����
		//!���̊֐����Ă΂Ȃ��Ă��A���̃N���X�̃f�X�g���N�^�őS�ĊJ�����Ă���
		//!���̊֐��̌�́A�`��ւ̃|�C���^�͎g���Ȃ��̂Œ���
		void RemoveCollisionShape(CollisionShape** shape);

		//!@brief ���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s��(�d�������m)
		//!@param[in] obj �ړ�����R���W�����I�u�W�F�N�g
		//!@param[in] move �ړ��x�N�g��
		//!@param[in] vLimitAngle �c�����̕ǂ�����N�����Ȃ��p�x(deg)�A���̊p�x�ȏ�̓��ˊp�x�ɑ΂��ĕǂ�����s��
		//!@param[in] hLimitAngle �������̕ǂ�����N�����Ȃ��p�x(deg)�A���̊p�x�ȏ�̓��ˊp�x�ɑ΂��ĕǂ�����s��
		void MoveCharacter(CollisionData* obj, const K_Math::Vector3& move, float vLimitAngle = 40.0f, float hLimitAngle = 0.0f);

		//!@brief ���U�I�ȃR���W�����̈ړ��A���肪 MoveCharacter ������G�c(�������y��)
		//!@param[in] obj �ړ�����R���W�����I�u�W�F�N�g
		//!@param[in] move �ړ��x�N�g��
		//!@param[in] vLimitDirection �c�����̉����Ԃ����Amove�̒�����Ɍ��肷�邩�̃t���O
		//!@param[in] hLimitDirection �������̉����Ԃ����Amove�̒�����Ɍ��肷�邩�̃t���O
		void MoveCharacterDiscrete(CollisionData* obj, const K_Math::Vector3& move, bool vLimitDirection = true, bool hLimitDirection = false);

		//!@brief from�̈ʒu����to�̈ʒu�փ��C���΂��āA���̏Փˈʒu��Ԃ�
		//!@param[in] from ���C�̔������ʒu
		//!@param[in] to ���C�̏I�_
		//!@return ���[���h��ł̃��C�̏Փˈʒu��\����
		float Raycast(const K_Math::Vector3& from, const K_Math::Vector3& to, int myselfMask);

		//!@brief ���݂̕������E�ł̓���̃I�u�W�F�N�g�ɑ΂���Փ˂̃`�F�b�N
		//!@param[in] �Փ˂��`�F�b�N�������I�u�W�F�N�g
		//!@return �Փ˂��N�������I�u�W�F�N�g�̃^�O���
		std::vector<CollisionTag*>& FindConfrictionObjects(CollisionData* myself);

		//!@brief �d�͂Ƃ͔��Ε������w���P�ʃx�N�g����ݒ�A�L�����N�^�[�̈ړ��ɗ��p����
		//!@param[in] vector �d�͂Ɣ��Ε����̒P�ʃx�N�g��
		void SetSkyVector(const K_Math::Vector3& vector);
	private:
		//�R���W�������폜
		void RemoveCollisionObject(btCollisionObject* rigidbody);
		//�R���W�������ړ�
		void MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector);
		//�w������Ɉړ��i���U����j
		void MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, const btVector3& limitDirection);
		//�w������Ɉړ�
		btVector3 MoveSmooth(btCollisionObject* obj, const btVector3& moveVector, float limitAngle, const btVector3& limitDirection);
		//�ړ��������܂Ƃ߁AallowDistance�͂߂荞�݋��e�l�AisCalcurate��true�̎��͖@����Ԃ�
		btVector3 MoveBySweep(btCollisionObject* obj, const btVector3& moveVector, const btVector3& limitDirection, float limitAngle, float allowDistance);
	private:
		//�Փˌ��ʊi�[�p
		std::vector<CollisionTag*> confrictResult;
		//��ւ̒P�ʃx�N�g���A�����蔻��Ŏg�p
		btVector3 toSkyVector;

		btDiscreteDynamicsWorld*                bulletWorld;
		btDefaultCollisionConfiguration*        config;
		btCollisionDispatcher*                  dispatcher;
		btConstraintSolver*                     solver;
		btBroadphaseInterface*                  broadphase;
		bulletDebugDraw                         debugDrawer;
		btAlignedObjectArray<CollisionShape*>   shapeArray;
	};



	////���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
	struct SweepTestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
	public:
		SweepTestCallBack(btCollisionObject *myself);

		//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
		//�߂�l�ɈӖ��͂Ȃ��݂���
		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)override;

		//�R���W�����}�X�N����
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;

	public:
		btCollisionObject* myself;

	};

	//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
	struct DetectMaxDistance : public btCollisionWorld::ContactResultCallback {
	public:
		DetectMaxDistance(btCollisionObject* obj);
		//���������I�u�W�F�N�g���L�^
		//�߂�l�ɈӖ��͂Ȃ��݂���
		//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
		virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)override;

		//�R���W�����}�X�N����
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;
	public:
		//�����o���I�u�W�F�N�g
		btCollisionObject * obj;
		//�����̍ő�l�Ƃ��̕����Ƃ��̃I�u�W�F�N�g
		float maxDistance;
		btVector3 fixVec;
		btCollisionObject* maxObject;

		int count;
		bool isLoop;
	};

	//���ʂɉ����o���R�[���o�b�N
	struct FixContactCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		FixContactCallBack(btCollisionObject* obj, const btVector3& limitDirection);
		//���������I�u�W�F�N�g���L�^
		//�߂�l�ɈӖ��͂Ȃ��݂���
		//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
		virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)override;

		//�R���W�����}�X�N����
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;
	public:
		//�����o���I�u�W�F�N�g
		btCollisionObject * obj;
		//�����o������
		btVector3 limitDirection;
		bool isHit;
	};

	//���ׂĂ̏Փ˂��L�^����
	struct CollectCollisionCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		CollectCollisionCallBack(btCollisionObject* obj, std::vector<CollisionTag*>& tagList);
		virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)override;

		//�R���W�����}�X�N����
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;
	public:
		std::vector<CollisionTag*>& result;
		bool isHit;
	};

	//���C�L���X�g�p�̃R�[���o�b�N
	struct MyRaycastCallBack : public btCollisionWorld::ClosestRayResultCallback {
	public:
		MyRaycastCallBack(const btVector3& from, const btVector3& to, int myselfMask);
		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace)override;

		//�R���W�����}�X�N����
		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override;
	};

}