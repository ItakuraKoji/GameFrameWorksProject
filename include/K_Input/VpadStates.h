#pragma once

#include<GLFW\glfw3.h>
#include<iostream>
#include"K_Math\MyMathFanctions.h"


namespace K_Input {
	///@brief スティック初期位置の遊び定数
#define BUTTON_EPSILON 0.2f

///@brief 接続したゲームパッドのボタンと軸
	enum class JoyButton {
		Empty = 0,
		Button0 = 1,
		Button1 = 2,
		Button2 = 4,
		Button3 = 8,
		Button4 = 16,
		Button5 = 32,
		Button6 = 64,
		Button7 = 128,
		Button8 = 256,
		Button9 = 512,
		Button10 = 1024,
		Button11 = 2048,
		Button12 = 4096,
	};

	enum class JoyAxis {
		Empty = -1,
		Axis0 = 0,
		Axis1 = 1,
		Axis2 = 2,
		Axis3 = 3,
		Axis4 = 4,
		Axis5 = 5,
	};

	///@brief 仮想ゲームパッドに割り当てるキーボードの定数
	enum class Key {
		Empty = GLFW_KEY_UNKNOWN,
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		N = 'N',
		M = 'M',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		Space = GLFW_KEY_SPACE,
		Enter = GLFW_KEY_ENTER,
		ArrowUp = GLFW_KEY_UP,
		ArrowDown = GLFW_KEY_DOWN,
		ArrowRight = GLFW_KEY_RIGHT,
		ArrowLeft = GLFW_KEY_LEFT,
		ShiftRight = GLFW_KEY_RIGHT_SHIFT,
		ShiftLeft = GLFW_KEY_LEFT_SHIFT,
		ControlRight = GLFW_KEY_RIGHT_CONTROL,
		ControlLeft = GLFW_KEY_LEFT_CONTROL,
	};

	///@brief 仮想ゲームパッドが持つボタンの配列用定数
	///ABXYとLR3つずつ
	///十字キー
	///start と select
	enum class VpadButton {
		A,
		B,
		X,
		Y,
		R1,
		R2,
		R3,
		L1,
		L2,
		L3,
		Right,
		Left,
		Up,
		Down,
		Start,
		Select,
		//こいつだけは一番下にする
		EnumSize,
	};

	///@brief 仮想ゲームパッドが持つ６軸の配列用定数
	enum class VpadAxis {
		Axis0,
		Axis1,
		Axis2,
		Axis3,
		Axis4,
		Axis5,
		//こいつだけは一番下にする
		EnumSize,
	};

	///@brief 仮想ゲームパッドが持つ６軸のうちの２軸を組み合わせた「スティック」の配列用定数
	enum class VpadStick {
		L,
		R,
		//こいつだけは一番下にする
		EnumSize,
	};

	///@brief ジョイパッドの状態
	struct ButtonState {
		JoyButton button;	//仮想パッドのこのボタンに対応するゲームパッドのボタンのビット
		Key keyboard;		//仮想パッドのこのボタンにに対応するキー
		int press;			//今、このボタンが押されているか
		int prevPress;		//前フレーム、このボタンが押されていたか
	};

	///@brief アナログスティックの軸
	struct AxisState {
		JoyAxis axis;		//仮想パッドのこの軸に対応するゲームパッドの軸
		Key plusButton;		//この軸における生の方向に対応するキーボードのキー
		Key minusButton;	//この軸における負の方向に対応するキーボードのキー
		float pos;			//アナログスティックの位置
	};

	///@brief 横と縦の二軸を持つスティック
	struct StickState {
	public:
		StickState();

		///@brief スティック横軸をセット
		///@param[in] xAxisPos 横軸として使うJoyStickAxisの軸位置へのポインタ
		///@param[in] yAxisPos 縦軸として使うJoyStickAxisの軸位置へのポインタ
		void SetAxis(float* xAxisPos, float* yAxisPos);

		///@return 軸の位置（各要素 0.0f - 1.0f の範囲）
		K_Math::Vector2 GetPosition();

		///@return スティック軸の角度（X軸方向から始まるラジアン角度）
		float GetRotation();

		///@return スティックの傾きの大きさ(0.0f - 1.0f の範囲)
		float GetPower();

	private:
		//この二つは参照用なので書き換えないこと
		float* x;
		float* y;
	};
}
