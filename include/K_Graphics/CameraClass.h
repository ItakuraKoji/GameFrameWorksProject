#pragma once
#include<iostream>
#include"K_Math\MyMathFanctions.h"

namespace K_Graphics {

	///@brief カメラの種類を判別する列挙型
	enum CameraType {
		Perspective,
		Ortho,
	};

	///@brief カメラの情報を管理し、ビュー行列と投影行列を提供するクラス
	class CameraClass {
	public:
		///@brief Initialize()を呼ぶ
		CameraClass(CameraType type, int width, int height, float near, float far, float fov);
		///@brief 特に何もしない
		~CameraClass();

		///@brief カメラを一気に初期化
		///@param[in] type カメラのタイプ
		///@param[in] screenWidth ビューポートの幅（アスペクト比を出すのに使用する）
		///@param[in] screenHeight ビューポートの高さ（アスペクト比を出すのに使用する）
		///@param[in] cameraNeer クリッピング平面の最近面
		///@param[in] cameraFar クリッピング平面の最遠面
		///@param[in] fieldOfView 縦の視野角
		void Initialize(CameraType type, int screenWidth, int screenHeight, float cameraNeer, float cameraFar, float fieldOfView);

		///@brief 行列を作成する、この関数を呼ばない限りカメラは更新されない
		void Draw();
		///@brief ビュー行列を取得する
		///@return 4x4のビュー変換行列
		const K_Math::Matrix4x4& GetViewMatrix();
		///@brief ビュー行列の逆行列を取得する
		///@return 4x4のビュー変換行列の逆行列
		const K_Math::Matrix4x4& GetCameraMatrix();
		///@brief 投影行列を取得する
		///@return 4x4の投影変換行列
		const K_Math::Matrix4x4& GetProjectionMatrix();

		///@brief カメラの座標を指定
		///@param[in] x x座標
		///@param[in] y y座標
		///@param[in] z z座標
		void SetPosition(float x, float y, float z);
		///@brief ターゲットからの方向と距離を指定してカメラ位置を設定
		///@param[in] distance ターゲットからの距離
		///@param[in] vector ターゲットから離れる向き
		void SetPosition(float distance, const K_Math::Vector3& vector);
		///@brief ターゲット（注視点）を指定
		///@param[in] x x座標
		///@param[in] y y座標
		///@param[in] z z座標
		void SetTarget(float x, float y, float z);

		///@return カメラ空間X軸の単位ベクトル
		const K_Math::Vector3& GetAxisX();
		///@return カメラ空間Y軸の単位ベクトル
		const K_Math::Vector3& GetAxisY();
		///@return カメラ空間Z軸の単位ベクトル
		const K_Math::Vector3& GetAxisZ();
		///@return 注視点ベクトル
		const K_Math::Vector3& GetTerget();
		///@return カメラ位置ベクトル
		const K_Math::Vector3& GetPosition();

		///@return カメラに設定されたビューポート幅
		int   GetScreenWidth();
		///@return カメラに設定されたビューポート高さ
		int   GetScreenHeight();
		///@return クリッピング平面の最近面
		float GetScreenNear();
		///@return クリッピング平面の最遠面
		float GetScreenFar();
		///@brief 平行投影カメラの場合は値が不定なので注意
		///@return カメラの視野角
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