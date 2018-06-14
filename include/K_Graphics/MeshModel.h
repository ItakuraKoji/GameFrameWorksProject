#pragma once
#include"ModelData.h"
#include"ModelDataFactory.h"
#include"CameraClass.h"
#include"ShaderClass.h"

namespace K_Graphics {

	//!@brief 3D���f���N���X\n�t�@�N�g���[���琶�Y���ꂽ���f���f�[�^���󂯎���ď���������\n
	//!�u�`��v�Ƃ��������ŁA�`�掞�ɂ́uK_Graphics::ShaderClass�v�̃|�C���^���K�{
	class MeshModel {
	public:
		//!@brief Initialize()���Ă�
		MeshModel(ModelDatas* data);
		//!@brief Finalize()���Ă�
		~MeshModel();

		//!@brief ���f���f�[�^�ɂ����3D���f����������
		//!@param[in] ModelDataFactory�N���X�ɂ���Đ������ꂽ���f���f�[�^�ւ̃|�C���^
		//!@return ���������true
		bool Initialize(ModelDatas* data);
		//!@brief �I�������A���f���f�[�^�̃|�C���^��delete���Ă���
		void Finalize();
		//!@brief �X�L�����b�V���A�j���[�V������I���A�Đ�����
		//!@param[in] animationName �A�j���[�V�����̖��O�i���f�����O�\�t�g�ō쐬�����Ƃ��̃A�j���[�V�������j
		//!@param[in] playOnce �A�����ē����A�j���[�V�������Đ������Ƃ��ɏ��߂���Đ����Ȃ�����
		//!@param[in] isLoop �A�j���[�V���������[�v�Đ����邩
		//!@param[in] interpolationFrames �t���[���P�ʂ̕�Ԏ��ԁi�O�ŕ⊮���Ȃ��j
		void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames);
		//!@brief �e�N�X�`����ύX����A��{�͎g��Ȃ��Ă�����
		//!@param[in] texture �g�p����e�N�X�`���ւ̃|�C���^
		//!@param[in] arrayIndex �ΏۂƂȂ郁�b�V���K�w�̃C���f�b�N�X
		//!@param[in] materialIndex �ΏۂƂȂ郁�b�V���K�w�̂Ȃ��̃}�e���A���C���f�b�N�X
		void SetTexture(Texture* texture, int hierarchyIndex, int materialIndex);
		//!@brief �A�j���[�V�����X�s�[�h��ύX����
		//!@param[in] 1��ʏ�̑��x�Ƃ����A�j���[�V�����̑��x
		void SetSpeed(float speed);

		//!@brief �A�j���[�V�������X�V
		void UpdateAnimation();
		//!@brief �`����s��\n�V�F�[�_�[�ɂ͕K�v�ȍs�񂪂��łɓ����Ă���Ƃ���
		//!@param[in] shader �g�p����V�F�[�_�[
		void Draw(ShaderClass* shader);
		//!@brief (������)�C���X�^���X�`����s��\n�V�F�[�_�[�Ƃ̘A�g���܂��ł��Ă��Ȃ��̂Ŗ�����
		void InstanceDraw(int numInstance, ShaderClass* shader);

	private:
		void SetBone(int hierarchyIndex, ShaderClass* shader);
		void DrawBuffers(int hierarchyIndex, ShaderClass* shader);

	private:
		ModelDatas * data;
		Texture*  boneTexture;
		bool isBoneProcessed;
	};

	//!@brief K_Graphics::MeshModel�̕`���"position""rotation""scale"�̂R�ōs����悤�ɂ�������\n
	//!�`��ɂ́uK_Graphics::CameraClass�v�uK_Graphics::ShaderClass�v�̓���K�v
	class MeshObject {
	public:
		//!@brief Initialize()���Ă�
		MeshObject(MeshModel* model);
		//!@brief Finalize()���Ă�
		~MeshObject();

		//!@brief MeshModel�ւ̃|�C���^��n���ď�����
		//!@param[in] model MeshModel�ւ̃|�C���^
		//!@return ���������true
		bool Initialize(MeshModel* model);
		//!@brief Initialize()�œn����MeshModel�ւ̃|�C���^���J�����Ă���
		void Finalize();

		//!@brief �X�L�����b�V���A�j���[�V������I���A�Đ�����
		//!@param[in] animationName �A�j���[�V�����̖��O�i���f�����O�\�t�g�ō쐬�����Ƃ��̃A�j���[�V�������j
		//!@param[in] playOnce �A�����ē����A�j���[�V�������Đ������Ƃ��ɏ��߂���Đ����Ȃ�����
		//!@param[in] isLoop �A�j���[�V���������[�v�Đ����邩
		//!@param[in] inInterpolation �O�̃A�j���[�V�����Ɛ��`��Ԃ����邩(�ȗ����͂O�ɂȂ��ĕ⊮���Ȃ�)
		void SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames = 0);
		//!@brief �A�j���[�V�����X�s�[�h��ύX����
		//!@param[in] 1��ʏ�̑��x�Ƃ����A�j���[�V�����̑��x
		void SetSpeed(float speed);

		//!@brief �A�j���[�V�������X�V
		void UpdateAnimation();

		//!@brief �e��s������A�V�F�[�_�[�ɓn���ĕ`����s��
		//!@param[in] camera �g�p����J�����N���X�ւ̃|�C���^
		//!@param[in] shader �g�p����V�F�[�_�[�ւ̃|�C���^
		//!@param[in] position 3D��ԏ�̈ʒu���W
		//!@param[in] rotation XYZ���ꂼ��̎��Ɋւ����]�p�x�iY��X��Z�̏��ŉ�]����j
		//!@param[in] scale �X�P�[�����O
		void Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		void Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);
		//!@brief �C���X�^���X�`��͖�����
		void InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

		//!@brief SRT�p�����烏�[���h�ϊ��s����쐬����
		//!@param[in] position 3D��ԏ�̈ʒu���W
		//!@param[in] rotation XYZ���ꂼ��̎��Ɋւ����]�p�x�iY��X��Z�̏��ŉ�]����j
		//!@param[in] scale �X�P�[�����O
		//!@return ���[���h�ϊ��s��
		K_Math::Matrix4x4 CreateWorldMatrix(const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		K_Math::Matrix4x4 CreateWorldMatrix(const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);

	protected:
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);
		MeshModel* drawModel;
	};

	//!@brief ����1.0f�̔|���S����ModelClass������Ő������A2D�摜�̕`��ɗ��p�ł���@�\��t���������N���X\n
	//!�`��ɂ́uK_Graphics::CameraClass�v�uK_Graphics::ShaderClass�v�̓���K�v
	class SpriteObject {
	public:
		
		//!@brief Initialize()���Ă�
		SpriteObject(Texture* texture, float controlPointX = 0.0f, float controlPointY = 0.0f);
		//!@brief Finalize()���Ă�
		~SpriteObject();

		//!@brief �������A�e�N�X�`����ݒ肵�A�ό`�̃R���g���[���|�C���g���W���w�肷�邱�Ƃ��ł���
		//!@param[in] texture �g�p����e�N�X�`���Anullptr���w�肷�邱�Ƃ��ł���
		//!@param[in] controlPointX �R���g���[���|�C���gX���W\n�|���S�������㌴�_�ŕό`�̉e�����󂯂Ȃ�
		//!@param[in] controlPointY �R���g���[���|�C���gY���W\n�|���S�������㌴�_�ŕό`�̉e�����󂯂Ȃ�
		bool Initialize(Texture* texture, float controlPointX, float controlPointY);
		//!@brief �I������
		void Finalize();

		//!@brief �\������e�N�X�`����ύX����
		//!@param[in] texture �g�p����e�N�X�`���̃|�C���^
		bool SetTexture(Texture* texture);
		//!@brief 2D�`����s��
		//!�u�X�N���[���̃T�C�Y���J�����ɐݒ肵�Ă���v�u���s���e�̃J�����ł���v�uZ��������������Z���ƕ��s�v
		//!�Ƃ����R�̏����𖞂������J������ݒ肷��ƃX�N���[�����W�ł̕`�悪�ł���
		//!@param[in] camera �g�p����J�����i��L�̏����ɒ��Ӂj
		//!@param[in] shader �g�p����V�F�[�_�[
		//!@param[in] src �e�N�X�`���̐؂�����\n�u�e�N�X�`���̃s�N�Z�����WXY�v�u��`�̕��ƍ����v�����ꂼ��"x""y""w""h"�ɐݒ肷��
		//!@param[in] draw ���ۂɕ`�悷��ꏊ�Ƒ傫�����w�肷��\n�u�ʒu���WXY�v�u��`�̕��ƍ����v�����ꂼ��"x""y""w""h"�ɐݒ肷��
		//!@param[in] rotation �E��]�̊p�x�i�x���@�j
		void Draw2D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Box2D& draw, float rotation);
		//!@brief 3D��ԏ�ɕ`�悷��A�������K���J�����̕��������r���{�[�h�ɂȂ�
		//!@param[in] camera �g�p����J�����N���X�ւ̃|�C���^
		//!@param[in] shader �g�p����V�F�[�_�[�ւ̃|�C���^
		//!@param[in] src �e�N�X�`���̐؂�����\n�u�e�N�X�`���̃s�N�Z�����WXY�v�u��`�̕��ƍ����v�����ꂼ��"x""y""w""h"�ɐݒ肷��
		//!@param[in] position 3D��ԏ�̈ʒu���W
		//!@param[in] rotation XYZ���ꂼ��̎��Ɋւ����]�p�x�iY��X��Z�̏��ŉ�]����j
		//!@param[in] scale �X�P�[�����O
		void Draw3D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

		//!@brief SRT�p�����烏�[���h�ϊ��s����쐬����
		//!@param[in] camera �g�p����J�����N���X�ւ̃|�C���^�A�r���{�[�h��]�ɕK�v
		//!@param[in] position 3D��ԏ�̈ʒu���W
		//!@param[in] rotation XYZ���ꂼ��̎��Ɋւ����]�p�x�iY��X��Z�̏��ŉ�]����j
		//!@param[in] scale �X�P�[�����O
		//!@param[in] billBoard �r���{�[�h�Ƃ��ăX�v���C�g���������̃t���O
		//!@return ���[���h�ϊ��s��
		K_Math::Matrix4x4 CreateWorldMatrix(CameraClass* camera, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale, bool billBoard);

	public:
		//!@brief �R���g���[���|�C���g�̍��W
		K_Math::Vector2 controlPoint;
	protected:
		MeshModel* drawModel;
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale, bool billBoard);
		Texture* cullentTexture;
	};

}
