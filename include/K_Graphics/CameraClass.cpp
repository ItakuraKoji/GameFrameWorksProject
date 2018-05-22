#include"CameraClass.h"


namespace K_Graphics {
	////////
	//public
	////
	CameraClass::CameraClass(CameraType type, int width, int height, float near, float far, float fov) {
		Initialize(type, width, height, near, far, fov);
	}
	CameraClass::~CameraClass() {

	}

	void CameraClass::Initialize(CameraType type, int width, int height, float near, float far, float fov) {
		SetPosition(0, 0, 0);
		SetTarget(0, 0, 0);
		this->screenWidth = width;
		this->screenHeight = height;
		this->screenNear = near;
		this->screenFar = far;
		this->fieldOfView = fov;
		this->projectionType = type;
		Draw();
	}


	const K_Math::Matrix4x4& CameraClass::GetViewMatrix() {
		return this->viewMatrix;

	}
	const K_Math::Matrix4x4& CameraClass::GetCameraMatrix() {
		return this->cameraMatrix;
	}
	const K_Math::Matrix4x4& CameraClass::GetProjectionMatrix() {
		return this->projectionMatrix;
	}

	void CameraClass::SetPosition(float x, float y, float z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}

	void CameraClass::SetPosition(float distance, const K_Math::Vector3& vector) {
		if (vector == K_Math::Vector3(0.0f, 0.0f, 0.0f)) {
			//normalize�ł��Ȃ��[���x�N�g���̏ꍇ�͈ʒu���^�[�Q�b�g�Ɠ�����
			this->position = this->target;
			return;
		}

		//���S����vector�̕�����distance������
		this->position = this->target + glm::normalize(vector) * distance;
	}


	void CameraClass::SetTarget(float x, float y, float z) {
		this->target.x = x;
		this->target.y = y;
		this->target.z = z;
	}

	const K_Math::Vector3& CameraClass::GetAxisX() {
		return this->xAxis;
	}
	const K_Math::Vector3& CameraClass::GetAxisY() {
		return this->yAxis;
	}
	const K_Math::Vector3& CameraClass::GetAxisZ() {
		return this->zAxis;
	}
	const K_Math::Vector3& CameraClass::GetTerget() {
		return this->target;
	}
	const K_Math::Vector3& CameraClass::GetPosition() {
		return this->position;
	}

	int CameraClass::GetScreenWidth() {
		return this->screenWidth;
	}
	int CameraClass::GetScreenHeight() {
		return this->screenHeight;
	}
	float CameraClass::GetScreenNear() {
		return this->screenNear;
	}
	float CameraClass::GetScreenFar() {
		return screenFar;
	}
	float CameraClass::GetFieldOfView() {
		return this->fieldOfView;
	}

	//�����_��^���ăr���[�s����쐬
	void CameraClass::Draw() {
		K_Math::Vector3 up;
		//�J�����̏�������`
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
		K_Math::MatrixLookAt(this->viewMatrix, this->position, this->target, up);

		//�t�s��i�J�����s��j���ێ�
		for (int r = 0; r < 3; ++r) {
			for (int l = 0; l < 3; ++l) {
				this->cameraMatrix[r][l] = this->viewMatrix[l][r];
			}
		}
		(K_Math::Vector3)this->cameraMatrix[3] = -this->viewMatrix[3];

		//�J������
		this->xAxis = (K_Math::Vector3)this->viewMatrix[0];
		this->yAxis = (K_Math::Vector3)this->viewMatrix[1];
		this->zAxis = (K_Math::Vector3)this->viewMatrix[2];

		//�v���W�F�N�V�����s����쐬
		if (this->projectionType == CameraType::Perspective) {
			K_Math::MatrixPerspectiveLH(this->projectionMatrix, (float)this->screenWidth, (float)this->screenHeight, this->screenNear, this->screenFar, this->fieldOfView);
		}
		else {
			K_Math::MatrixOrthoLH(this->projectionMatrix, (float)this->screenWidth, (float)this->screenHeight, this->screenNear, this->screenFar);
		}
	}
}
