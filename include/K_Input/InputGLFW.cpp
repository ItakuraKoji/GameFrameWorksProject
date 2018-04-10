#include"InputGLFW.h"

namespace K_Input {

	////////
	//public
	////
	InputGLFW::InputGLFW(unsigned int joyID, GLFWwindow* handle) {
		Initialize(joyID, handle);
	}

	InputGLFW::~InputGLFW() {

	}

	//初期化
	bool InputGLFW::Initialize(unsigned int joyID, GLFWwindow* handle) {
		this->window = handle;
		//仮想キーとゲームパッドの対応
		this->vpadState[VPAD_BUTTON_A].button = PAD_BUTTON_0;
		this->vpadState[VPAD_BUTTON_B].button = PAD_BUTTON_1;
		this->vpadState[VPAD_BUTTON_X].button = PAD_BUTTON_2;
		this->vpadState[VPAD_BUTTON_Y].button = PAD_BUTTON_3;
		this->vpadState[VPAD_BUTTON_L].button = PAD_BUTTON_4;
		this->vpadState[VPAD_BUTTON_R].button = PAD_BUTTON_5;

		//仮想キーとキーボードの対応
		this->vpadState[VPAD_BUTTON_A].keyboard = GLFW_KEY_SPACE;
		this->vpadState[VPAD_BUTTON_B].keyboard = 'Z';
		this->vpadState[VPAD_BUTTON_RIGHT].keyboard = GLFW_KEY_RIGHT;
		this->vpadState[VPAD_BUTTON_LEFT].keyboard = GLFW_KEY_LEFT;
		this->vpadState[VPAD_BUTTON_UP].keyboard = GLFW_KEY_UP;
		this->vpadState[VPAD_BUTTON_DOWN].keyboard = GLFW_KEY_DOWN;

		//仮想スティックとスティックの対応
		this->vpadStickAxis[VPAD_STICK_LX].axis = PAD_STICK_X;
		this->vpadStickAxis[VPAD_STICK_LY].axis = PAD_STICK_Y;
		this->vpadStickAxis[VPAD_STICK_RX].axis = PAD_STICK_Z;
		this->vpadStickAxis[VPAD_STICK_RY].axis = PAD_STICK_R;
		this->vpadStickAxis[VPAD_STICK_Z1].axis = PAD_STICK_Z;

		//仮想スティックとキーボードの対応
		this->vpadStickAxis[VPAD_STICK_LX].plusButton = GLFW_KEY_RIGHT;
		this->vpadStickAxis[VPAD_STICK_LX].minusButton = GLFW_KEY_LEFT;
		this->vpadStickAxis[VPAD_STICK_LY].plusButton = GLFW_KEY_UP;
		this->vpadStickAxis[VPAD_STICK_LY].minusButton = GLFW_KEY_DOWN;
		this->vpadStickAxis[VPAD_STICK_RX].plusButton = 'D';
		this->vpadStickAxis[VPAD_STICK_RX].minusButton = 'A';
		this->vpadStickAxis[VPAD_STICK_RY].plusButton = 'W';
		this->vpadStickAxis[VPAD_STICK_RY].minusButton = 'S';

		//２軸スティックを登録
		this->vpadStickState[VPAD_STICK_L].SetAxisX(&this->vpadStickAxis[VPAD_STICK_LX]);
		this->vpadStickState[VPAD_STICK_L].SetAxisY(&this->vpadStickAxis[VPAD_STICK_LY]);
		this->vpadStickState[VPAD_STICK_R].SetAxisX(&this->vpadStickAxis[VPAD_STICK_RX]);
		this->vpadStickState[VPAD_STICK_R].SetAxisY(&this->vpadStickAxis[VPAD_STICK_RY]);

		this->joyID = joyID;

		return true;
	}

	//現在の仮想ゲームパッドの状態を取得
	void InputGLFW::Run() {
		GetInputState();

		int count;
		int buttonBit = 0;
		const unsigned char* buttonState = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		for (int i = 0; i < count; ++i) {
			if (buttonState[i] == GLFW_PRESS) {
				buttonBit += (1 << i);
			}
		}


		//ボタンに対応するビットとAND演算をして結果を配列におさめる
		for (int i = 0; i < 10; ++i) {
			this->vpadState[i].prevPress = this->vpadState[i].press;
			this->vpadState[i].press = (glfwGetKey(this->window, this->vpadState[i].keyboard) == GLFW_PRESS) || (this->vpadState[i].button & buttonBit);
		}
		//スティック情報を仮想パッドに割り当て
		for (int i = 0; i < 6; ++i) {
			int index = this->vpadStickAxis[i].axis;
			float stateData = this->stickState[index];
			if (glfwGetKey(this->window, this->vpadStickAxis[i].plusButton) == GLFW_PRESS) {
				stateData = 1.0f;
			}
			if (glfwGetKey(this->window, this->vpadStickAxis[i].minusButton) == GLFW_PRESS) {
				stateData = -1.0f;
			}

			// -1 から 1 の範囲のスティック位置が格納される
			this->vpadStickAxis[i].pos = stateData;
		}
	}


	//押した瞬間
	bool InputGLFW::isPressButton(ButtonID buttonID) {
		return this->vpadState[buttonID].press && !this->vpadState[buttonID].prevPress;
	}
	//押している間
	bool InputGLFW::isStayButton(ButtonID buttonID) {
		return (this->vpadState[buttonID].press && this->vpadState[buttonID].prevPress) || isPressButton(buttonID);
	}
	//離された瞬間
	bool InputGLFW::isReaveButton(ButtonID buttonID) {
		return !this->vpadState[buttonID].press && this->vpadState[buttonID].prevPress;
	}

	//スティック情報
	float InputGLFW::GetStickState(ButtonID axisID) {
		return this->vpadStickAxis[axisID].pos;
	}
	//角度と傾き
	float InputGLFW::GetStickRotation(ButtonID stickID) {
		return this->vpadStickState[stickID].GetRotation();
	}
	float InputGLFW::GetStickPower(ButtonID stickID) {
		float power = this->vpadStickState[stickID].GetPower();
		if (power > 1.0f) {
			power = 1.0f;
		}
		return power;
	}

	////////
	//private
	////

	//キーの状態を取得
	void InputGLFW::GetInputState() {
		int count;
		const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		if (count > 6) {
			count = 6;
		}
		for (int i = 0; i < 6; ++i) {
			this->stickState[i] = 0;
		}
		for (int i = 0; i < count; ++i) {
			this->stickState[i] = axis[i];
			if (this->stickState[i] < BUTTON_EPSILON && this->stickState[i] > -BUTTON_EPSILON) {
				this->stickState[i] = 0.0f;
			}
		}
	}

}
