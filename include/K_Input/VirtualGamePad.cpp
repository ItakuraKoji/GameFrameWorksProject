#include"VirtualGamePad.h"

namespace K_Input {

	////////
	//public
	////
	VirtualGamePad::VirtualGamePad(VpadIndex padID, GLFWwindow* handle) {
		this->window = handle;
		this->vpadID = padID;

		//ゲームパッドボタン設定(無を設定)
		SetButtonConfig(VpadButton::A, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::B, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::X, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Y, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::L1, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::L2, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::L3, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::R1, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::R2, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::R3, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Up, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Down, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Right, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Left, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Start, JoyButton::Empty, Key::Empty);
		SetButtonConfig(VpadButton::Select, JoyButton::Empty, Key::Empty);

		//ゲームパッド軸設定(無を設定)
		SetAxisConfig(VpadAxis::Axis0, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis1, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis2, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis3, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis4, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis5, JoyAxis::Empty, Key::Empty, Key::Empty);

		//ゲームパッド2軸スティック設定
		SetStickConfig(VpadStick::L, VpadAxis::Axis0, VpadAxis::Axis1);
		SetStickConfig(VpadStick::R, VpadAxis::Axis2, VpadAxis::Axis3);
	}

	VirtualGamePad::~VirtualGamePad() {

	}

	//現在の仮想ゲームパッドの状態を取得
	void VirtualGamePad::Run() {
		GetAxisState();
		GetButtonState();
	}

	//ゲームパッド名を取得
	const char* VirtualGamePad::GetJoyPadName() {
		return glfwGetJoystickName((int)this->vpadID);
	}

	//仮想ゲームパッド
	//押した瞬間
	bool VirtualGamePad::IsPressButton(VpadButton buttonID) {
		return this->vpadButton[(int)buttonID].press && !this->vpadButton[(int)buttonID].prevPress;
	}
	//押している間
	bool VirtualGamePad::IsStayButton(VpadButton buttonID) {
		return (this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress) || IsPressButton(buttonID);
	}
	//離された瞬間
	bool VirtualGamePad::IsReaveButton(VpadButton buttonID) {
		return !this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress;
	}

	//!@return そのボタンが押された瞬間true
	bool VirtualGamePad::IsPressButtonAny() {
		return this->anyKeyPress && !this->anyKeyPressPrev;
	}
	//!@return そのボタンを押している間true
	bool VirtualGamePad::IsStayButtonAny() {
		return (this->anyKeyPress && this->anyKeyPressPrev) || IsPressButtonAny();
	}
	//!@return そのボタンが離された瞬間true
	bool VirtualGamePad::IsReaveButtonAny() {
		return !this->anyKeyPress && !this->anyKeyPressPrev;
	}


	//スティック情報
	float VirtualGamePad::GetAxisPosition(VpadAxis axisID) {
		return this->vpadAxis[(int)axisID].pos;
	}
	//角度と傾き
	float VirtualGamePad::GetStickRotation(VpadStick stickID) {
		return this->vpadStick[(int)stickID].GetRotation();
	}
	float VirtualGamePad::GetStickPower(VpadStick stickID) {
		float power = this->vpadStick[(int)stickID].GetPower();
		if (power > 1.0f) {
			power = 1.0f;
		}
		return power;
	}

	void VirtualGamePad::SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key keyboard) {
		//ジョイスティックと仮想パッドのボタンを対応
		this->vpadButton[(int)vpadButton].button = joypadButton;
		this->vpadButton[(int)vpadButton].keyboard = keyboard;
		this->vpadButton[(int)vpadButton].press = 0;
		this->vpadButton[(int)vpadButton].prevPress = 0;
	}

	void VirtualGamePad::SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus) {
		//ジョイスティックと仮想パッドの軸を対応
		this->vpadAxis[(int)vpadAxis].axis = joypadAxis;
		this->vpadAxis[(int)vpadAxis].plusButton = axisPlus;
		this->vpadAxis[(int)vpadAxis].minusButton = axisMinus;
		this->vpadAxis[(int)vpadAxis].pos = 0.0f;
	}

	void VirtualGamePad::SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis) {
		//仮想ゲームパッドの軸２つからスティックを設定
		this->vpadStick[(int)vpadStick].SetAxis(&this->vpadAxis[(int)xAxis].pos, &this->vpadAxis[(int)yAxis].pos);
	}

	//!現在押されているゲームパッドのボタン
	std::vector<JoyButton> VirtualGamePad::GetPressRawButtonID() {
		//ボタン情報読み込み
		int count;
		const unsigned char* buttonState = glfwGetJoystickButtons((int)this->vpadID, &count);
		
		std::vector<JoyButton> result;
		result.reserve(VButtonCount);

		//全て調べる
		for (int i = 0; i < count; ++i) {
			//押されているかをチェック
			if (buttonState[i] == GLFW_PRESS) {
				result.push_back(static_cast<JoyButton>(1 << i));
			}
		}
		return result;
	}

	//!現在倒されているゲームパッドの軸
	//!@param[in] judgePower この値より倒されている軸が返ってくる
	std::vector<JoyAxis> VirtualGamePad::GetActiveRawAxisID(float judgePower) {
		//軸情報を順番に読み込み
		int count;
		const float* axis = glfwGetJoystickAxes((int)this->vpadID, &count);

		std::vector<JoyAxis> result;
		result.reserve(VAxisCount);

		//全て調べる
		for (int i = 0; i < count; ++i) {
			//一定以上の傾きかをチェック
			if (axis[i] > judgePower) {
				result.push_back(static_cast<JoyAxis>(i));
			}
		}
		return result;
	}


	////////
	//private
	////
	void VirtualGamePad::GetAxisState() {

		//軸情報を順番に読み込み
		int count;
		const float* axis = glfwGetJoystickAxes((int)this->vpadID, &count);

		//得られた軸の数とゲームパッドの軸の数をそろえる
		if (count > VAxisCount) {
			count = VAxisCount;
		}

		for (int i = 0; i < VAxisCount; ++i) {
			this->stickState[i] = 0;
		}
		for (int i = 0; i < count; ++i) {
			this->stickState[i] = axis[i];
			if (this->stickState[i] < BUTTON_EPSILON && this->stickState[i] > -BUTTON_EPSILON) {
				this->stickState[i] = 0.0f;
			}
		}
		//軸情報を仮想パッドに割り当て
		for (int i = 0; i < VAxisCount; ++i) {
			if (this->vpadAxis[i].axis == JoyAxis::Empty) {
				//割り当てなしならループは飛ばす
				continue;
			}
			float stateData = 0.0f;
			if (glfwGetKey(this->window, (int)this->vpadAxis[i].plusButton) == GLFW_PRESS) {
				stateData += 1.0f;
			}
			if (glfwGetKey(this->window, (int)this->vpadAxis[i].minusButton) == GLFW_PRESS) {
				stateData += -1.0f;
			}
			if (stateData == 0.0f) {
				stateData = this->stickState[(int)this->vpadAxis[i].axis];
			}
			// -1 から 1 の範囲のスティック位置が格納される
			this->vpadAxis[i].pos = stateData;
		}
	}
	void VirtualGamePad::GetButtonState() {
		//いずれかのボタンを押したかのフラグをリセット
		this->anyKeyPressPrev = this->anyKeyPress;
		this->anyKeyPress = false;

		int count;
		//押されたボタンから順番にビットを立てる
		const unsigned char* buttonState = glfwGetJoystickButtons((int)this->vpadID, &count);

		int buttonBit = 0;
		for (int i = 0; i < count; ++i) {
			if (buttonState[i] == GLFW_PRESS) {
				//ビットシフトでビット立て
				buttonBit += (1 << i);
			}
		}
		//ボタンに対応するビットとAND演算をして結果を配列におさめる
		for (int i = 0; i < VButtonCount; ++i) {
			this->vpadButton[i].prevPress = this->vpadButton[i].press;
			this->vpadButton[i].press = (glfwGetKey(this->window, (int)this->vpadButton[i].keyboard) == GLFW_PRESS) || ((int)this->vpadButton[i].button & buttonBit);

			//ボタンが押されていた時にフラグ立て
			if (this->vpadButton[i].press) {
				this->anyKeyPress = true;
			}
		}
	}
}
