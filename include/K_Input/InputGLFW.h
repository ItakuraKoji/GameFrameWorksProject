#pragma once

#include<Windows.h>
#include<GLFW\glfw3.h>
#include<math.h>
#include<iostream>
#include<Eigen\Core>
#include"K_Math\MyMathFanctions.h"

namespace K_Input {

///@brief スティック初期位置の遊び定数
#define BUTTON_EPSILON 0.2f

///@brief 接続したゲームパッドのボタンと軸
	enum BUTTON_BITS {
		PAD_BUTTON_0 = 1,
		PAD_BUTTON_1 = 2,
		PAD_BUTTON_2 = 4,
		PAD_BUTTON_3 = 8,
		PAD_BUTTON_4 = 16,
		PAD_BUTTON_5 = 32,
		PAD_BUTTON_6 = 64,
		PAD_BUTTON_7 = 128,
		PAD_BUTTON_8 = 256,
		PAD_BUTTON_9 = 512,
		PAD_BUTTON_10 = 1024,
		PAD_BUTTON_11 = 2048,
		PAD_BUTTON_12 = 4096,
		PAD_STICK_X = 0,
		PAD_STICK_Y = 1,
		PAD_STICK_Z = 2,
		PAD_STICK_R = 3,
		PAD_STICK_U = 4,
		PAD_STICK_V = 5,
	};

	///@brief 仮想ゲームパッド配列用定数
	///ABXYとLR
	///十字キー
	///6軸のアナログ情報（0～1のfloat）
	///上記のアナログ情報二つを、横方向と縦方向として持つスティック２本（右と左を想定）
	enum ButtonID {
		VPAD_BUTTON_A,
		VPAD_BUTTON_B,
		VPAD_BUTTON_X,
		VPAD_BUTTON_Y,
		VPAD_BUTTON_R,
		VPAD_BUTTON_L,
		VPAD_BUTTON_RIGHT,
		VPAD_BUTTON_LEFT,
		VPAD_BUTTON_UP,
		VPAD_BUTTON_DOWN,

		VPAD_STICK_LX = 0,
		VPAD_STICK_LY,
		VPAD_STICK_RX,
		VPAD_STICK_RY,
		VPAD_STICK_Z1,
		VPAD_STICK_Z2,

		VPAD_STICK_L = 0,
		VPAD_STICK_R,
	};

	///@brief ジョイパッドの状態
	struct JoyButtonState {
		int button;			//仮想パッドのこのボタンに対応するゲームパッドのボタンのビット
		int keyboard;		//仮想パッドのこのボタンにに対応するキー
		int press;			//今、このボタンが押されているか
		int prevPress;		//前フレーム、このボタンが押されていたか
	};

	///@brief アナログスティックの軸
	struct JoyStickAxis {
		int axis;			//仮想パッドのこの軸に対応するゲームパッドの軸
		int plusButton;		//この軸における生の方向に対応するキーボードのキー
		int minusButton;	//この軸における負の方向に対応するキーボードのキー
		float pos;			//アナログスティックの位置
	};

	///@brief 横と縦の二軸を持つスティック
	struct JoyStickState {
	public:
		///@brief スティック横軸をセット
		///@param[in] xAxis 横軸として使うJoyStickAxisへのポインタ
		void SetAxisX(JoyStickAxis* xAxis) {
			this->x = xAxis;
		}
		///@brief スティック縦軸をセット
		///@param[in] yAxis 縦軸として使うJoyStickAxisへのポインタ
		void SetAxisY(JoyStickAxis* yAxis) {
			this->y = yAxis;
		}

		///@return 軸の位置（各要素 0.0f - 1.0f の範囲）
		K_Math::Vector2 GetPosition() {
			return K_Math::Vector2(x->pos, y->pos);
		}
		///@return スティック軸の角度（X軸方向から始まるラジアン角度）
		float GetRotation() {
			return atan2(y->pos, x->pos);
		}
		///@return スティックの傾きの大きさ(0.0f - 1.0f の範囲)
		float GetPower() {
			return sqrtf(powf(x->pos, 2) + powf(y->pos, 2));
		}
	private:
		JoyStickAxis * x;
		JoyStickAxis* y;
	};


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
		bool isPressButton(ButtonID buttonID);
		///@param[in] buttonID 調べる仮想キーの値
		///@return そのボタンを押している間true
		bool isStayButton(ButtonID buttonID);
		///@param[in] buttonID 調べる仮想キーの値
		///@return そのボタンが離された瞬間true
		bool isReaveButton(ButtonID buttonID);

		///@return スティック１つの軸の位置
		float GetStickState(ButtonID axisID);
		///@return ２軸スティックの角度を取得（X軸方向から始まるラジアン角度）
		float GetStickRotation(ButtonID stickID);
		///@return ２軸スティックの傾きを取得
		float GetStickPower(ButtonID stickID);

	private:
		void GetInputState();

	private:
		GLFWwindow * window;
		//コントローラーIDとJOYINFOEX構造体
		unsigned int joyID;

		//仮想コントローラーのボタン対応と入力情報の配列
		JoyButtonState vpadState[10] = {};

		//スティックの状態
		JoyStickState vpadStickState[2] = {};
		JoyStickAxis vpadStickAxis[6] = {};	//0 ～ 1 の間に値が収まっている
		float stickState[6] = {};		//値が０から１に収まっていない
	};

}
