#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

namespace K_Graphics {

	///@brief カメラに名前を付けて管理するクラス
	class CameraList {
	public:
		///@brief Initialize()を呼ぶ
		CameraList();
		///@brief Initialize()を呼ぶ
		~CameraList();
		///@brief リスト内のカメラクラスを全開放している
		void Initialize();

		///@brief 射影投影のカメラを追加する、射影投影か平行投影かは後で変更できる\n同じ名前は登録できない
		///@param[in] name 登録するカメラのユーザー定義名
		///@param[in] position カメラ位置
		///@param[in] target 注視点位置
		///@param[in] screenWidth ビューポートの幅（アスペクト比を出すのに使用する）
		///@param[in] screenHeight ビューポートの高さ（アスペクト比を出すのに使用する）
		///@param[in] cameraNeer クリッピング平面の最近面
		///@param[in] cameraFar クリッピング平面の最遠面
		///@param[in] fieldOfView 縦の視野角
		void AddPerspectiveCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int screenWidth, int screenHeight, float cameraNeer, float cameraFar, float fieldOfView);

		///@brief 平行投影のカメラを追加する、射影投影か平行投影かは後で変更できる\n同じ名前は登録できない
		///@param[in] name 登録するカメラのユーザー定義名
		///@param[in] position カメラ位置
		///@param[in] target 注視点位置
		///@param[in] screenWidth ビューポートの幅（アスペクト比を出すのに使用する）
		///@param[in] screenHeight ビューポートの高さ（アスペクト比を出すのに使用する）
		///@param[in] cameraNeer クリッピング平面の最近面
		///@param[in] cameraFar クリッピング平面の最遠面
		void AddOrthoCamera(const std::string& name, const K_Math::Vector3& position, const K_Math::Vector3& target, int screenWidth, int screenHeight, float cameraNeer, float cameraFar);
		///@brief 名前からカメラを取得する
		///@param[in] name カメラのユーザー定義名
		///@return その名前のカメラへのポインタ、失敗した場合はnullptr
		CameraClass* GetCamera(const std::string& name);

	private:
		std::unordered_map<std::string, CameraClass*> cameraList;
	};
}
