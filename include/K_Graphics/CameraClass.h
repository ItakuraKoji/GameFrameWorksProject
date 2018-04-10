#pragma once
#include<iostream>
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	///@brief �J�����̎�ނ𔻕ʂ���񋓌^
	enum CameraType {
		Perspective,
		Ortho,
	};

	///@brief �J�����̏����Ǘ����A�r���[�s��Ɠ��e�s���񋟂���N���X
	class CameraClass {
	public:
		///@brief Initialize()���Ă�
		CameraClass(CameraType type, int width, int height, float near, float far, float fov);
		///@brief ���ɉ������Ȃ�
		~CameraClass();

		///@brief �J��������C�ɏ�����
		///@param[in] type �J�����̃^�C�v
		///@param[in] screenWidth �r���[�|�[�g�̕��i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] screenHeight �r���[�|�[�g�̍����i�A�X�y�N�g����o���̂Ɏg�p����j
		///@param[in] cameraNeer �N���b�s���O���ʂ̍ŋߖ�
		///@param[in] cameraFar �N���b�s���O���ʂ̍ŉ���
		///@param[in] fieldOfView �c�̎���p
		void Initialize(CameraType type, int screenWidth, int screenHeight, float cameraNeer, float cameraFar, float fieldOfView);

		///@brief �s����쐬����A���̊֐����Ă΂Ȃ�����J�����͍X�V����Ȃ�
		void Draw();
		///@brief �r���[�s����擾����
		///@return 4x4�̃r���[�ϊ��s��
		const K_Math::Matrix4x4& GetViewMatrix();
		///@brief �r���[�s��̋t�s����擾����
		///@return 4x4�̃r���[�ϊ��s��̋t�s��
		const K_Math::Matrix4x4& GetCameraMatrix();
		///@brief ���e�s����擾����
		///@return 4x4�̓��e�ϊ��s��
		const K_Math::Matrix4x4& GetProjectionMatrix();

		///@brief �J�����̍��W���w��
		///@param[in] x x���W
		///@param[in] y y���W
		///@param[in] z z���W
		void SetPosition(float x, float y, float z);
		///@brief �^�[�Q�b�g����̕����Ƌ������w�肵�ăJ�����ʒu��ݒ�
		///@param[in] distance �^�[�Q�b�g����̋���
		///@param[in] vector �^�[�Q�b�g���痣������
		void SetPosition(float distance, const K_Math::Vector3& vector);
		///@brief �^�[�Q�b�g�i�����_�j���w��
		///@param[in] x x���W
		///@param[in] y y���W
		///@param[in] z z���W
		void SetTarget(float x, float y, float z);

		///@return �J�������X���̒P�ʃx�N�g��
		const K_Math::Vector3& GetAxisX();
		///@return �J�������Y���̒P�ʃx�N�g��
		const K_Math::Vector3& GetAxisY();
		///@return �J�������Z���̒P�ʃx�N�g��
		const K_Math::Vector3& GetAxisZ();
		///@return �����_�x�N�g��
		const K_Math::Vector3& GetTerget();
		///@return �J�����ʒu�x�N�g��
		const K_Math::Vector3& GetPosition();

		///@return �J�����ɐݒ肳�ꂽ�r���[�|�[�g��
		int   GetScreenWidth();
		///@return �J�����ɐݒ肳�ꂽ�r���[�|�[�g����
		int   GetScreenHeight();
		///@return �N���b�s���O���ʂ̍ŋߖ�
		float GetScreenNear();
		///@return �N���b�s���O���ʂ̍ŉ���
		float GetScreenFar();
		///@brief ���s���e�J�����̏ꍇ�͒l���s��Ȃ̂Œ���
		///@return �J�����̎���p
		float GetFieldOfView();

	private:
		K_Math::Vector3 target;
		K_Math::Vector3 position;

		K_Math::Matrix4x4 viewMatrix;
		K_Math::Matrix4x4 cameraMatrix;
		K_Math::Vector3 xAxis;
		K_Math::Vector3 yAxis;
		K_Math::Vector3 zAxis;

		CameraType projectionType;
		K_Math::Matrix4x4 projectionMatrix;
		int            screenWidth;
		int            screenHeight;
		float          screenNear;
		float          screenFar;
		float          fieldOfView;


	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}