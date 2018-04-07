#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
#include<iostream>
#include"MyMathFanctions.h"

namespace K_Graphics {

	enum CameraType {
		Perspective,
		Ortho,
	};

	class CameraClass {
	public:
		CameraClass(CameraType type, int width, int height, float near, float far, float fov);
		~CameraClass();

		//カメラを一気に設定する
		void Initialize(CameraType type, int width, int height, float near, float far, float fov);

		void Draw();
		const K_Math::Matrix4x4& GetViewMatrix();
		const K_Math::Matrix4x4& GetCameraMatrix();
		const K_Math::Matrix4x4& GetProjectionMatrix();

		//座標指定
		void SetPosition(float x, float y, float z);
		//ターゲットからの方向と距離
		void SetPosition(float distance, const K_Math::Vector3& vector);
		void SetTarget(float x, float y, float z);

		const K_Math::Vector3& GetAxisX();
		const K_Math::Vector3& GetAxisY();
		const K_Math::Vector3& GetAxisZ();
		const K_Math::Vector3& GetTerget();
		const K_Math::Vector3& GetPosition();

		int   GetScreenWidth();
		int   GetScreenHeight();
		float GetScreenNear();
		float GetScreenFar();
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
		K_Math::Matrix4x4       projectionMatrix;
		int            screenWidth;
		int            screenHeight;
		float          screenNear;
		float          screenFar;
		float          fieldOfView;


	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}