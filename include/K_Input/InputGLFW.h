#pragma once
#include"VpadStates.h"

namespace K_Input {

	///@brief GLFWを利用したキー入力とジョイパッドの入力を扱うクラス
	class InputGLFW {
	public:
		///@brief Initialize()を呼ぶ
		InputGLFW(unsigned int joyID, GLFWwindow* handle);
		///@brief Finalize()を呼ぶ
		~InputGLFW();
		///@brief 入力設定を初期化
		///@param[in] joyID ジョイパッドの番号（0,1,2...とコントローラーの数に応じて一つずつ設定する）
		///@param[in] handle SystemClassの持つウィンドウのハンドル
		///@return 成功するとtrue
		bool Initialize(unsigned int joyID, GLFWwindow* handle);
		///@brief 入力情報を更新
		void Run();

		///@param[in] buttonID 調べる仮想キーの値
		///@return そのボタンが押された瞬間true
		bool isPressButton(VpadButton buttonID);
		///@param[in] buttonID 調べる仮想キーの値
		///@return そのボタンを押している間true
		bool isStayButton(VpadButton buttonID);
		///@param[in] buttonID 調べる仮想キーの値
		///@return そのボタンが離された瞬間true
		bool isReaveButton(VpadButton buttonID);

		///@return スティック１つの軸の位置
		float GetStickState(VpadStick axisID);
		///@return ２軸スティックの角度を取得（X軸方向から始まるラジアン角度）
		float GetStickRotation(VpadStick stickID);
		///@return ２軸スティックの傾きを取得
		float GetStickPower(VpadStick stickID);

		///@brief 指定の仮想パッドボタン定数と物理入力との対応を設定
		void SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key board);
		///@brief 指定の仮想パッド軸定数と物理入力との対応を設定
		void SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus);
		///@brief 指定の仮想パッドスティック定数と物理入力との対応を設定
		void SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis);

	private:
		void GetButtonState();
		void GetAxisState();

	private:
		GLFWwindow * window;
		//コントローラーIDとJOYINFOEX構造体
		unsigned int joyID;

		//仮想コントローラーのボタン対応と入力情報の配列
		ButtonState vpadButton[static_cast<int>(VpadButton::EnumSize)] = {};

		//スティックの状態
		StickState vpadStick[static_cast<int>(VpadStick::EnumSize)] = {};
		AxisState vpadAxis[static_cast<int>(VpadAxis::EnumSize)] = {};	//0 ～ 1 の間に値が収まっている
		float stickState[static_cast<int>(VpadAxis::EnumSize)] = {};		//値が０から１に収まっていない
	};

}
