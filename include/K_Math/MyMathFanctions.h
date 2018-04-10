#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<Eigen\Core>
#include<Eigen\Geometry>

//Eigenはずいぶん複雑なヘッダのようで、こいつは最初のほうにインクルードするほうがいいっぽい

//数学系はみんなこの名前空間がいいなあって
namespace K_Math {
	///float型２要素ベクトル\n要素のアクセスに "vector.x()" と関数の形をとるのに注意
	typedef Eigen::Matrix<float, 2, 1, Eigen::DontAlign> Vector2;
	///float型３要素ベクトル\n要素のアクセスに "vector.x()" と関数の形をとるのに注意
	typedef Eigen::Matrix<float, 3, 1, Eigen::DontAlign> Vector3;
	///float型４要素ベクトル\n要素のアクセスに "vector.x()" と関数の形をとるのに注意
	typedef Eigen::Matrix<float, 4, 1, Eigen::DontAlign> Vector4;
	///float型３行３列行列
	typedef Eigen::Matrix<float, 3, 3, Eigen::DontAlign> Matrix3x3;
	///float型４行４列行列
	typedef Eigen::Matrix<float, 4, 4, Eigen::DontAlign> Matrix4x4;
	///float型のクォータニオン\n内部では回転行列はクォータニオンを利用している
	typedef Eigen::Quaternion<float, Eigen::DontAlign> Quaternion;

	//アフィン変換用、基本的には内部で使っているだけでユーザーが扱うことはほぼない
	typedef Eigen::Transform<float, 3, 2, Eigen::DontAlign> Affine3;
	typedef Eigen::AngleAxis<float> AngleAxis;
	typedef Eigen::Translation<float, 3> Translation;
	typedef Eigen::DiagonalMatrix<float, 3> DiagonalMatrix;

	///@brief 座標の"x""y"、幅高さの"w""h"を持つ\n全てint型で、2D描画用
	struct Box2D {
	public:
		Box2D();
		Box2D(int x, int y, int w, int h);
		Box2D& operator =(Box2D& box);
	public:
		int x, y, w, h;
	};

	///@brief 「度数法　→　ラジアン」の変換
	///@param[in] deg 度数法での角度
	///@return ラジアンでの角度
	float DegToRad(float deg);
	///@brief 「ラジアン　→　度数法」の変換
	///@param[in] rad ラジアンでの角度
	///@return 度数法での角度
	float RadToDeg(float rad);
	///@brief 左手座標系の射影変換行列を作る
	///@param[out] result 結果の4x4行列を格納する変数
	///@param[in] screenWidth 描画スクリーンの幅\nウィンドウではなく、ビューポートの幅を入れる
	///@param[in] screenWidth 描画スクリーンの高さ\nウィンドウではなく、ビューポートの高さを入れる
	///@param[in] screenNear 近いほうの、錐視台のクリッピング領域
	///@param[in] screenFar 遠いほうの、錐視台のクリッピング領域
	///@param[in] fieldOfView 画面の縦に対する視野角
	void  MatrixPerspectiveLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	///@brief 右手座標系の射影変換行列を作る\n詳細はMatrixPerspectiveLH()を参照のこと
	void  MatrixPerspectiveRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar, float fieldOfView);
	///@brief 左手座標系の平行投影行列を作る
	///@param[out] result 結果の4x4行列を格納する変数
	///@param[in] screenWidth 描画スクリーンの幅\nウィンドウではなく、ビューポートの幅を入れる
	///@param[in] screenWidth 描画スクリーンの高さ\nウィンドウではなく、ビューポートの高さを入れる
	///@param[in] screenNear 近いほうの、錐視台のクリッピング領域
	///@param[in] screenFar 遠いほうの、錐視台のクリッピング領域
	void  MatrixOrthoLH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	///@brief 右手座標系の平行投影行列を作る\n詳細はMatrixOrthoLH()を参照のこと
	void  MatrixOrthoRH(K_Math::Matrix4x4& result, float screenWidth, float screenHeight, float screenNear, float screenFar);
	///@brief ビュー行列を作る
	///@param[out] result 結果の4x4行列を格納する変数
	///@param[in] position カメラの座標
	///@param[in] lookAt 注視点の座標
	///@param[in] up カメラの上を指す単位ベクトル
	void  MatrixLookAt(K_Math::Matrix4x4& result, K_Math::Vector3& position, K_Math::Vector3& lookAt, K_Math::Vector3& up);
}
