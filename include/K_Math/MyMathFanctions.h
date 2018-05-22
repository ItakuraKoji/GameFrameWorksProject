#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include<math.h>

//Eigenはずいぶん複雑なヘッダのようで、こいつは最初のほうにインクルードするほうがいいっぽい

namespace K_Math {
	//!float型２要素ベクトル\n要素のアクセスに "vector.x" と関数の形をとるのに注意
	typedef glm::vec2 Vector2;
	//!float型３要素ベクトル\n要素のアクセスに "vector.x" と関数の形をとるのに注意
	typedef glm::vec3 Vector3;
	//!float型４要素ベクトル\n要素のアクセスに "vector.x" と関数の形をとるのに注意
	typedef glm::vec4 Vector4;
	//!float型３行３列行列
	typedef glm::mat3x3 Matrix3x3;
	//!float型４行４列行列
	typedef glm::mat4x4 Matrix4x4;
	//!float型のクォータニオン\n内部では回転行列はクォータニオンを利用している
	typedef glm::quat Quaternion;

	//アフィン変換用
	//typedef Eigen::Transform<float, 3, 2, Eigen::DontAlign> Affine3;
	//typedef Eigen::AngleAxis<float> AngleAxis;
	//typedef Eigen::Translation<float, 3> Translation;
	//typedef Eigen::DiagonalMatrix<float, 3> DiagonalMatrix;

	//!@brief 座標の"x""y"、幅高さの"w""h"を持つ\n全てint型で、2D描画用
	struct Box2D {
	public:
		Box2D();
		Box2D(int x, int y, int w, int h);
		Box2D& operator =(Box2D& box);
	public:
		int x, y, w, h;
	};

	//!@brief 「度数法　→　ラジアン」の変換
	//!@param[in] deg 度数法での角度
	//!@return ラジアンでの角度
	float DegToRad(float deg);
	//!@brief 「ラジアン　→　度数法」の変換
	//!@param[in] rad ラジアンでの角度
	//!@return 度数法での角度
	float RadToDeg(float rad);
	//!@brief 左手座標系の射影変換行列を作る
	//!@param[out] result 結果の4x4行列を格納する変数
	//!@param[in] screenWidth 描画スクリーンの幅\nウィンドウではなく、ビューポートの幅を入れる
	//!@param[in] screenWidth 描画スクリーンの高さ\nウィンドウではなく、ビューポートの高さを入れる
	//!@param[in] screenNear 近いほうの、錐視台のクリッピング領域
	//!@param[in] screenFar 遠いほうの、錐視台のクリッピング領域
	//!@param[in] fieldOfView 画面の縦に対する視野角
	void  MatrixPerspectiveLH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	//!@brief 右手座標系の射影変換行列を作る\n詳細はMatrixPerspectiveLH()を参照のこと
	void  MatrixPerspectiveRH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	//!@brief 左手座標系の平行投影行列を作る
	//!@param[out] result 結果の4x4行列を格納する変数
	//!@param[in] screenWidth 描画スクリーンの幅\nウィンドウではなく、ビューポートの幅を入れる
	//!@param[in] screenWidth 描画スクリーンの高さ\nウィンドウではなく、ビューポートの高さを入れる
	//!@param[in] screenNear 近いほうの、錐視台のクリッピング領域
	//!@param[in] screenFar 遠いほうの、錐視台のクリッピング領域
	void  MatrixOrthoLH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	//!@brief 右手座標系の平行投影行列を作る\n詳細はMatrixOrthoLH()を参照のこと
	void  MatrixOrthoRH(Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	//!@brief ビュー行列を作る
	//!@param[out] result 結果の4x4行列を格納する変数
	//!@param[in] position カメラの座標
	//!@param[in] lookAt 注視点の座標
	//!@param[in] up カメラの上を指す単位ベクトル
	void  MatrixLookAt(Matrix4x4& result, Vector3& position, Vector3& lookAt, Vector3& up);

	//!@brief 回転成分からクォータニオンを作成する(Y軸→X軸→Z軸)
	Quaternion RotationToQuaternion(const Vector3& rotation);
	//!@brief クォータニオンから回転成分を作成する(Y軸→X軸→Z軸)
	Vector3 QuaternionToRotation(const Quaternion& quaternion);

	//!@brief
	float Norm(const Vector2& vector);
	float Norm(const Vector3& vector);
	float Norm(const Vector4& vector);

	//!@brief
	float Cross(const Vector2& vector1, const Vector2& vector2);
	Vector3 Cross(const Vector3& vector1, const Vector3& vector2);

	//!@brief
	float Dot(const Vector2& vector1, const Vector2& vector2);
	float Dot(const Vector3& vector1, const Vector3& vector2);
}
