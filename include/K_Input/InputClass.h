#pragma once
#include<unordered_map>
#include"VirtualGamePad.h"

namespace K_Input {
	//!@brief 入力関連を取得できるクラス
	class InputClass {
	public:
		InputClass(GLFWwindow* handle);
		~InputClass();

		//!@brief 入力情報を全て更新
		void Run();

		//!@brief ゲームパッドを追加
		//!@input ゲームパッドのID
		//!@return 追加に失敗（すでに登録されていた）場合はfalse
		bool AddGamePad(VpadIndex padID);
		//!@brief ゲームパッドの取得
		//!@input ゲームパッドのID
		//!@return ゲームパッドが登録されていなかったらnullptrが返る
		VirtualGamePad* GetPad(VpadIndex padID);

		//!@param[in] buttonID 調べるマウスのボタン
		//!@return そのボタンが押された瞬間true
		bool IsPressMouse(VMouse buttonID);
		//!@param[in] buttonID 調べるマウスのボタン
		//!@return そのボタンを押している間true
		bool IsStayMouse(VMouse buttonID);
		//!@param[in] buttonID 調べるマウスのボタン
		//!@return そのボタンが離された瞬間true
		bool IsReaveMouse(VMouse buttonID);
		//!@return ウィンドウ左上を原点としたマウス位置
		K_Math::Vector2& GetMousePosition();

	private:
		void GetMouseState();

	private:
		//GLFWのウィンドウ情報
		GLFWwindow * windowHandle;

		//マウス状態
		K_Math::Vector2 mousePosition;
		MouseState vMouseButton[VMouseCount] = {};

		std::unordered_map<VpadIndex, VirtualGamePad*> gamePad;
	};
}