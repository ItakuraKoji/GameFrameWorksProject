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
		this->position << x, y, z;
	}

	void CameraClass::SetPosition(float distance, const K_Math::Vector3& vector) {
		if (vector == K_Math::Vector3::Zero()) {
			//normalizeできないゼロベクトルの場合は位置をターゲットと同じに
			this->position = this->target;
			return;
		}

		//中心からvectorの方向にdistance分放す
		this->position = this->target + vector.normalized() * distance;
	}


	void CameraClass::SetTarget(float x, float y, float z) {
		this->target << x, y, z;
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

	//注視点を与えてビュー行列を作成
	void CameraClass::Draw() {
		K_Math::Vector3 up;
		//カメラの上方向を定義
		up.x() = 0.0f;
		up.y() = 1.0f;
		up.z() = 0.0f;
		K_Math::MatrixLookAt(this->viewMatrix, this->position, this->target, up);

		//逆行列（カメラ行列）も保持
		this->cameraMatrix = this->viewMatrix;
		this->cameraMatrix.block(0, 0, 3, 3) = this->viewMatrix.block(0, 0, 3, 3).transpose();
		this->cameraMatrix.block(3, 0, 1, 3) = -this->viewMatrix.block(3, 0, 1, 3);

		//カメラ軸
		this->xAxis = this->viewMatrix.block(0, 0, 1, 3).transpose();
		this->yAxis = this->viewMatrix.block(1, 0, 1, 3).transpose();
		this->zAxis = this->viewMatrix.block(2, 0, 1, 3).transpose();

		//プロジェクション行列を作成
		if (this->projectionType == CameraType::Perspective) {
			K_Math::MatrixPerspectiveLH(this->projectionMatrix, (float)this->screenWidth, (float)this->screenHeight, this->screenNear, this->screenFar, this->fieldOfView);
		}
		else {
			K_Math::MatrixOrthoLH(this->projectionMatrix, (float)this->screenWidth, (float)this->screenHeight, this->screenNear, this->screenFar);
		}
	}
}
