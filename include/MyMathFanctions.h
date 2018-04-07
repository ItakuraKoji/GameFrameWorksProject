#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
#include<Eigen\Geometry>

//Eigenはずいぶん複雑なヘッダのようで、こいつは最初のほうにインクルードするほうがいいっぽい

//数学系はみんなこの名前空間がいいなあって
namespace K_Math {
	typedef Eigen::Matrix<float, 2, 1, Eigen::DontAlign> Vector2;
	typedef Eigen::Matrix<float, 3, 1, Eigen::DontAlign> Vector3;
	typedef Eigen::Matrix<float, 4, 1, Eigen::DontAlign> Vector4;
	typedef Eigen::Matrix<float, 3, 3, Eigen::DontAlign> Matrix3x3;
	typedef Eigen::Matrix<float, 4, 4, Eigen::DontAlign> Matrix4x4;
	typedef Eigen::Quaternion<float, Eigen::DontAlign> Quaternion;
	typedef Eigen::Transform<float, 3, 2, Eigen::DontAlign> Affine3;
	typedef Eigen::AngleAxis<float> AngleAxis;
	typedef Eigen::Translation<float, 3> Translation;
	typedef Eigen::DiagonalMatrix<float, 3> DiagonalMatrix;

	struct Box2D {
	public:
		Box2D();
		Box2D(int x, int y, int w, int h);
		Box2D& operator =(Box2D& box);
	public:
		int x, y, w, h;
	};

	float DegToRad(float deg);
	float RadToDeg(float rad);
	void  MatrixPerspectiveLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixPerspectiveRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	void  MatrixOrthoLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixOrthoRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	void  MatrixLookAt(K_Math::Matrix4x4& result, K_Math::Vector3& position, K_Math::Vector3& lookAt, K_Math::Vector3& up);
}
