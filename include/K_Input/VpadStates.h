#pragma once

#include<GLFW\glfw3.h>
#include<iostream>
#include"K_Math\MyMathFanctions.h"


namespace K_Input {
	//!@brief �X�e�B�b�N�����ʒu�̗V�ђ萔
#define BUTTON_EPSILON 0.2f

	enum class VpadIndex {
		Pad0 = GLFW_JOYSTICK_1,
		Pad1 = GLFW_JOYSTICK_2,
		Pad2 = GLFW_JOYSTICK_3,
		Pad3 = GLFW_JOYSTICK_4,
		Pad4 = GLFW_JOYSTICK_5,
		Pad5 = GLFW_JOYSTICK_6,
		Pad6 = GLFW_JOYSTICK_7,
	};

	enum class MouseButton {
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
	};

//!@brief �ڑ������Q�[���p�b�h�̃{�^���Ǝ�
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

		//��������͈ꉞ�u���Ă���A�W���C�p�b�h�ɂ���Ă͎g�p���邩������Ȃ�
		Axis6 = 6,
		Axis7 = 7,
		Axis8 = 8,
		Axis9 = 9,
		Axis10 = 10,
	};

	//!@brief ���z�Q�[���p�b�h�Ɋ��蓖�Ă�L�[�{�[�h�̒萔
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

	//!@brief ���z�}�E�X�̔z��p�萔
	//!���N���b�N�ƉE�N���b�N�ƒ����N���b�N�̂R��
	enum class VMouse {
		Right = 0,
		Left,
		Middle,
	};
	//���ڐ��萔�́A�񋓂̈�ԉ����w��i�A�Ԃł��邱�Ƃ��O��j
	const int VMouseCount = (int)VMouse::Middle + 1;

	//!@brief ���z�Q�[���p�b�h�����{�^���̔z��p�萔
	//!ABXY��LR3����
	//!�\���L�[
	//!start �� select
	enum class VpadButton {
		A = 0,
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
	};
	//���ڐ��萔�́A�񋓂̈�ԉ����w��i�A�Ԃł��邱�Ƃ��O��j
	const int VButtonCount = (int)VpadButton::Select + 1;

	//!@brief ���z�Q�[���p�b�h�����U���̔z��p�萔
	enum class VpadAxis {
		Axis0 = 0,
		Axis1,
		Axis2,
		Axis3,
		Axis4,
		Axis5,
	};
	//���ڐ��萔�́A�񋓂̈�ԉ����w��i�A�Ԃł��邱�Ƃ��O��j
	const int VAxisCount = (int)VpadAxis::Axis5 + 1;


	//!@brief ���z�Q�[���p�b�h�����U���̂����̂Q����g�ݍ��킹���u�X�e�B�b�N�v�̔z��p�萔
	enum class VpadStick {
		L = 0,
		R,
	};
	//���ڐ��萔�́A�񋓂̈�ԉ����w��i�A�Ԃł��邱�Ƃ��O��j
	const int VStickCount = (int)VpadStick::R + 1;

	//!@brief �}�E�X�̏��
	struct MouseState {
		MouseButton button;	//���z�p�b�h�̂��̃{�^���ɑΉ�����Q�[���p�b�h�̃{�^���̃r�b�g
		int press;			//���A���̃{�^����������Ă��邩
		int prevPress;		//�O�t���[���A���̃{�^����������Ă�����
	};

	//!@brief �W���C�p�b�h�̏��
	struct ButtonState {
		JoyButton button;	//���z�p�b�h�̂��̃{�^���ɑΉ�����Q�[���p�b�h�̃{�^���̃r�b�g
		Key keyboard;		//���z�p�b�h�̂��̃{�^���ɂɑΉ�����L�[
		int press;			//���A���̃{�^����������Ă��邩
		int prevPress;		//�O�t���[���A���̃{�^����������Ă�����
	};

	//!@brief �A�i���O�X�e�B�b�N�̎�
	struct AxisState {
		JoyAxis axis;		//���z�p�b�h�̂��̎��ɑΉ�����Q�[���p�b�h�̎�
		Key plusButton;		//���̎��ɂ����鐶�̕����ɑΉ�����L�[�{�[�h�̃L�[
		Key minusButton;	//���̎��ɂ����镉�̕����ɑΉ�����L�[�{�[�h�̃L�[
		float pos;			//�A�i���O�X�e�B�b�N�̈ʒu
	};

	//!@brief ���Əc�̓񎲂����X�e�B�b�N
	struct StickState {
	public:
		StickState();

		//!@brief �X�e�B�b�N�������Z�b�g
		//!@param[in] xAxisPos �����Ƃ��Ďg��JoyStickAxis�̎��ʒu�ւ̃|�C���^
		//!@param[in] yAxisPos �c���Ƃ��Ďg��JoyStickAxis�̎��ʒu�ւ̃|�C���^
		void SetAxis(float* xAxisPos, float* yAxisPos);

		//!@return ���̈ʒu�i�e�v�f 0.0f - 1.0f �͈̔́j
		K_Math::Vector2 GetPosition();

		//!@return �X�e�B�b�N���̊p�x�iX����������n�܂郉�W�A���p�x�j
		float GetRotation();

		//!@return �X�e�B�b�N�̌X���̑傫��(0.0f - 1.0f �͈̔�)
		float GetPower();

	private:
		//���̓�͎Q�Ɨp�Ȃ̂ŏ��������Ȃ�����
		float* x;
		float* y;
	};
}
