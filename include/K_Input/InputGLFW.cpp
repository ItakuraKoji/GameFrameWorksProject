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

	//������
	bool InputGLFW::Initialize(unsigned int joyID, GLFWwindow* handle) {
		this->window = handle;

		//�Q�[���p�b�h�{�^���ݒ�
		SetButtonConfig(VpadButton::A, JoyButton::Button0, Key::Space);
		SetButtonConfig(VpadButton::B, JoyButton::Button1, Key::Z);
		SetButtonConfig(VpadButton::X, JoyButton::Button2, Key::Empty);
		SetButtonConfig(VpadButton::Y, JoyButton::Button3, Key::Empty);
		SetButtonConfig(VpadButton::L1, JoyButton::Button4, Key::Empty);
		SetButtonConfig(VpadButton::R1, JoyButton::Button5, Key::Empty);

		//�Q�[���p�b�h���ݒ�
		SetAxisConfig(VpadAxis::Axis0, JoyAxis::Axis0, Key::ArrowRight, Key::ArrowLeft);
		SetAxisConfig(VpadAxis::Axis1, JoyAxis::Axis1, Key::ArrowUp, Key::ArrowDown);
		SetAxisConfig(VpadAxis::Axis2, JoyAxis::Axis4, Key::D, Key::A);
		SetAxisConfig(VpadAxis::Axis3, JoyAxis::Axis3, Key::W, Key::S);
		SetAxisConfig(VpadAxis::Axis4, JoyAxis::Axis2, Key::Empty, Key::Empty);

		//�Q�[���p�b�h2���X�e�B�b�N�ݒ�
		SetStickConfig(VpadStick::L, VpadAxis::Axis0, VpadAxis::Axis1);
		SetStickConfig(VpadStick::R, VpadAxis::Axis2, VpadAxis::Axis3);

		this->joyID = joyID;

		////���z�L�[�ƃQ�[���p�b�h�̑Ή�
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_A].button = ButtonBits::PAD_BUTTON_0;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_B].button = ButtonBits::PAD_BUTTON_1;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_X].button = ButtonBits::PAD_BUTTON_2;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_Y].button = ButtonBits::PAD_BUTTON_3;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_L].button = ButtonBits::PAD_BUTTON_4;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_R].button = ButtonBits::PAD_BUTTON_5;

		////���z�L�[�ƃL�[�{�[�h�̑Ή�
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_A].keyboard = Key::SPACE;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_B].keyboard = Key::Z;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_RIGHT].keyboard = Key::ARROW_RIGHT;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_LEFT].keyboard = Key::ARROW_LEFT;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_UP].keyboard = Key::ARROW_UP;
		//this->vpadButton[(int)VpadButton::VPAD_BUTTON_DOWN].keyboard = Key::ARROW_DOWN;

		////���z�X�e�B�b�N�ƃQ�[���p�b�h�̑Ή�
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_0].axis = ButtonBits::PAD_AXIS_0;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_1].axis = ButtonBits::PAD_AXIS_1;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_2].axis = ButtonBits::PAD_AXIS_4;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_3].axis = ButtonBits::PAD_AXIS_3;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_4].axis = ButtonBits::PAD_AXIS_2;

		////���z�X�e�B�b�N�ƃL�[�{�[�h�̑Ή�
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_0].plusButton = Key::ARROW_RIGHT;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_0].minusButton = Key::ARROW_LEFT;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_1].plusButton = Key::ARROW_UP;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_1].minusButton = Key::ARROW_DOWN;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_2].plusButton = Key::D;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_2].minusButton = Key::A;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_3].plusButton = Key::W;
		//this->vpadAxis[(int)VpadAxis::VPAD_AXIS_3].minusButton = Key::S;

		////�Q���X�e�B�b�N��o�^
		//this->vpadStick[(int)VpadStick::VPAD_STICK_L].SetAxis(&this->vpadAxis[(int)VpadAxis::VPAD_AXIS_0].pos, &this->vpadAxis[(int)VpadAxis::VPAD_AXIS_1].pos);
		//this->vpadStick[(int)VpadStick::VPAD_STICK_R].SetAxis(&this->vpadAxis[(int)VpadAxis::VPAD_AXIS_2].pos, &this->vpadAxis[(int)VpadAxis::VPAD_AXIS_3].pos);

		return true;
	}

	//���݂̉��z�Q�[���p�b�h�̏�Ԃ��擾
	void InputGLFW::Run() {
		GetAxisState();
		GetButtonState();
	}


	//�������u��
	bool InputGLFW::isPressButton(VpadButton buttonID) {
		return this->vpadButton[(int)buttonID].press && !this->vpadButton[(int)buttonID].prevPress;
	}
	//�����Ă����
	bool InputGLFW::isStayButton(VpadButton buttonID) {
		return (this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress) || isPressButton(buttonID);
	}
	//�����ꂽ�u��
	bool InputGLFW::isReaveButton(VpadButton buttonID) {
		return !this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress;
	}

	//�X�e�B�b�N���
	float InputGLFW::GetStickState(VpadStick axisID) {
		return this->vpadAxis[(int)axisID].pos;
	}
	//�p�x�ƌX��
	float InputGLFW::GetStickRotation(VpadStick stickID) {
		return this->vpadStick[(int)stickID].GetRotation();
	}
	float InputGLFW::GetStickPower(VpadStick stickID) {
		float power = this->vpadStick[(int)stickID].GetPower();
		if (power > 1.0f) {
			power = 1.0f;
		}
		return power;
	}

	void InputGLFW::SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key keyboard) {
		this->vpadButton[(int)vpadButton].button = joypadButton;
		this->vpadButton[(int)vpadButton].keyboard = keyboard;
		this->vpadButton[(int)vpadButton].press = 0;
		this->vpadButton[(int)vpadButton].prevPress = 0;
	}

	void InputGLFW::SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus) {
		this->vpadAxis[(int)vpadAxis].axis = joypadAxis;
		this->vpadAxis[(int)vpadAxis].plusButton = axisPlus;
		this->vpadAxis[(int)vpadAxis].minusButton = axisMinus;
		this->vpadAxis[(int)vpadAxis].pos = 0.0f;
	}

	void InputGLFW::SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis) {
		this->vpadStick[(int)vpadStick].SetAxis(&this->vpadAxis[(int)xAxis].pos, &this->vpadAxis[(int)yAxis].pos);
	}


	////////
	//private
	////

	void InputGLFW::GetAxisState() {

		//���������Ԃɓǂݍ���
		int count;
		const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		if (count > static_cast<int>(VpadAxis::EnumSize)) {
			count = static_cast<int>(VpadAxis::EnumSize);
		}
		for (int i = 0; i < static_cast<int>(VpadAxis::EnumSize); ++i) {
			this->stickState[i] = 0;
		}
		for (int i = 0; i < count; ++i) {
			this->stickState[i] = axis[i];
			if (this->stickState[i] < BUTTON_EPSILON && this->stickState[i] > -BUTTON_EPSILON) {
				this->stickState[i] = 0.0f;
			}
		}
		//���������z�p�b�h�Ɋ��蓖��
		for (int i = 0; i < static_cast<int>(VpadAxis::EnumSize); ++i) {
			if (this->vpadAxis[i].axis == JoyAxis::Empty) {
				//���蓖�ĂȂ��Ȃ烋�[�v�͔�΂�
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
			// -1 ���� 1 �͈̔͂̃X�e�B�b�N�ʒu���i�[�����
			this->vpadAxis[i].pos = stateData;
		}
	}
	void InputGLFW::GetButtonState() {

		int count;
		//�����ꂽ�{�^�����珇�ԂɃr�b�g�𗧂Ă�
		const unsigned char* buttonState = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		if (count > static_cast<int>(VpadButton::EnumSize)) {
			count = static_cast<int>(VpadButton::EnumSize);
		}
		int buttonBit = 0;
		for (int i = 0; i < count; ++i) {
			if (buttonState[i] == GLFW_PRESS) {
				buttonBit += (1 << i);
			}
		}
		//�{�^���ɑΉ�����r�b�g��AND���Z�����Č��ʂ�z��ɂ����߂�
		for (int i = 0; i < static_cast<int>(VpadButton::EnumSize); ++i) {
			this->vpadButton[i].prevPress = this->vpadButton[i].press;
			this->vpadButton[i].press = (glfwGetKey(this->window, (int)this->vpadButton[i].keyboard) == GLFW_PRESS) || ((int)this->vpadButton[i].button & buttonBit);
		}
	}

}
