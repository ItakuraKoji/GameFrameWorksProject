#include"VirtualGamePad.h"

namespace K_Input {

	////////
	//public
	////
	VirtualGamePad::VirtualGamePad(VpadIndex padID, GLFWwindow* handle) {
		this->window = handle;
		this->vpadID = padID;

		//�Q�[���p�b�h�{�^���ݒ�
		SetButtonConfig(VpadButton::A, JoyButton::Empty, Key::Space);
		SetButtonConfig(VpadButton::B, JoyButton::Empty, Key::Z);
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

		//�Q�[���p�b�h���ݒ�
		SetAxisConfig(VpadAxis::Axis0, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis1, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis2, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis3, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis4, JoyAxis::Empty, Key::Empty, Key::Empty);
		SetAxisConfig(VpadAxis::Axis5, JoyAxis::Empty, Key::Empty, Key::Empty);

		//�Q�[���p�b�h2���X�e�B�b�N�ݒ�
		SetStickConfig(VpadStick::L, VpadAxis::Axis0, VpadAxis::Axis1);
		SetStickConfig(VpadStick::R, VpadAxis::Axis2, VpadAxis::Axis3);
	}

	VirtualGamePad::~VirtualGamePad() {

	}

	//���݂̉��z�Q�[���p�b�h�̏�Ԃ��擾
	void VirtualGamePad::Run() {
		GetAxisState();
		GetButtonState();
	}

	//�Q�[���p�b�h�����擾
	const char* VirtualGamePad::GetJoyPadName() {
		return glfwGetJoystickName((int)this->vpadID);
	}

	//���z�Q�[���p�b�h
	//�������u��
	bool VirtualGamePad::IsPressButton(VpadButton buttonID) {
		return this->vpadButton[(int)buttonID].press && !this->vpadButton[(int)buttonID].prevPress;
	}
	//�����Ă����
	bool VirtualGamePad::IsStayButton(VpadButton buttonID) {
		return (this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress) || IsPressButton(buttonID);
	}
	//�����ꂽ�u��
	bool VirtualGamePad::IsReaveButton(VpadButton buttonID) {
		return !this->vpadButton[(int)buttonID].press && this->vpadButton[(int)buttonID].prevPress;
	}


	//�X�e�B�b�N���
	float VirtualGamePad::GetAxisPosition(VpadAxis axisID) {
		return this->vpadAxis[(int)axisID].pos;
	}
	//�p�x�ƌX��
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
		this->vpadButton[(int)vpadButton].button = joypadButton;
		this->vpadButton[(int)vpadButton].keyboard = keyboard;
		this->vpadButton[(int)vpadButton].press = 0;
		this->vpadButton[(int)vpadButton].prevPress = 0;
	}

	void VirtualGamePad::SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus) {
		this->vpadAxis[(int)vpadAxis].axis = joypadAxis;
		this->vpadAxis[(int)vpadAxis].plusButton = axisPlus;
		this->vpadAxis[(int)vpadAxis].minusButton = axisMinus;
		this->vpadAxis[(int)vpadAxis].pos = 0.0f;
	}

	void VirtualGamePad::SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis) {
		this->vpadStick[(int)vpadStick].SetAxis(&this->vpadAxis[(int)xAxis].pos, &this->vpadAxis[(int)yAxis].pos);
	}


	////////
	//private
	////
	void VirtualGamePad::GetAxisState() {

		//���������Ԃɓǂݍ���
		int count;
		const float* axis = glfwGetJoystickAxes((int)this->vpadID, &count);
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
		//���������z�p�b�h�Ɋ��蓖��
		for (int i = 0; i < VAxisCount; ++i) {
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
	void VirtualGamePad::GetButtonState() {

		int count;
		//�����ꂽ�{�^�����珇�ԂɃr�b�g�𗧂Ă�
		const unsigned char* buttonState = glfwGetJoystickButtons((int)this->vpadID, &count);
		if (count > VButtonCount) {
			count = VButtonCount;
		}
		int buttonBit = 0;
		for (int i = 0; i < count; ++i) {
			if (buttonState[i] == GLFW_PRESS) {
				buttonBit += (1 << i);
			}
		}
		//�{�^���ɑΉ�����r�b�g��AND���Z�����Č��ʂ�z��ɂ����߂�
		for (int i = 0; i < VButtonCount; ++i) {
			this->vpadButton[i].prevPress = this->vpadButton[i].press;
			this->vpadButton[i].press = (glfwGetKey(this->window, (int)this->vpadButton[i].keyboard) == GLFW_PRESS) || ((int)this->vpadButton[i].button & buttonBit);
		}
	}
}
