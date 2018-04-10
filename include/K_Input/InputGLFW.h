#pragma once

#include<Windows.h>
#include<GLFW\glfw3.h>
#include<math.h>
#include<iostream>
#include<Eigen\Core>
#include"K_Math\MyMathFanctions.h"

namespace K_Input {

///@brief �X�e�B�b�N�����ʒu�̗V�ђ萔
#define BUTTON_EPSILON 0.2f

///@brief �ڑ������Q�[���p�b�h�̃{�^���Ǝ�
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

	///@brief ���z�Q�[���p�b�h�z��p�萔
	///ABXY��LR
	///�\���L�[
	///6���̃A�i���O���i0�`1��float�j
	///��L�̃A�i���O������A�������Əc�����Ƃ��Ď��X�e�B�b�N�Q�{�i�E�ƍ���z��j
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

	///@brief �W���C�p�b�h�̏��
	struct JoyButtonState {
		int button;			//���z�p�b�h�̂��̃{�^���ɑΉ�����Q�[���p�b�h�̃{�^���̃r�b�g
		int keyboard;		//���z�p�b�h�̂��̃{�^���ɂɑΉ�����L�[
		int press;			//���A���̃{�^����������Ă��邩
		int prevPress;		//�O�t���[���A���̃{�^����������Ă�����
	};

	///@brief �A�i���O�X�e�B�b�N�̎�
	struct JoyStickAxis {
		int axis;			//���z�p�b�h�̂��̎��ɑΉ�����Q�[���p�b�h�̎�
		int plusButton;		//���̎��ɂ����鐶�̕����ɑΉ�����L�[�{�[�h�̃L�[
		int minusButton;	//���̎��ɂ����镉�̕����ɑΉ�����L�[�{�[�h�̃L�[
		float pos;			//�A�i���O�X�e�B�b�N�̈ʒu
	};

	///@brief ���Əc�̓񎲂����X�e�B�b�N
	struct JoyStickState {
	public:
		///@brief �X�e�B�b�N�������Z�b�g
		///@param[in] xAxis �����Ƃ��Ďg��JoyStickAxis�ւ̃|�C���^
		void SetAxisX(JoyStickAxis* xAxis) {
			this->x = xAxis;
		}
		///@brief �X�e�B�b�N�c�����Z�b�g
		///@param[in] yAxis �c���Ƃ��Ďg��JoyStickAxis�ւ̃|�C���^
		void SetAxisY(JoyStickAxis* yAxis) {
			this->y = yAxis;
		}

		///@return ���̈ʒu�i�e�v�f 0.0f - 1.0f �͈̔́j
		K_Math::Vector2 GetPosition() {
			return K_Math::Vector2(x->pos, y->pos);
		}
		///@return �X�e�B�b�N���̊p�x�iX����������n�܂郉�W�A���p�x�j
		float GetRotation() {
			return atan2(y->pos, x->pos);
		}
		///@return �X�e�B�b�N�̌X���̑傫��(0.0f - 1.0f �͈̔�)
		float GetPower() {
			return sqrtf(powf(x->pos, 2) + powf(y->pos, 2));
		}
	private:
		JoyStickAxis * x;
		JoyStickAxis* y;
	};


	///@brief GLFW�𗘗p�����L�[���͂ƃW���C�p�b�h�̓��͂������N���X
	class InputGLFW {
	public:
		///@brief Initialize()���Ă�
		InputGLFW(unsigned int joyID, GLFWwindow* handle);
		///@brief Finalize()���Ă�
		~InputGLFW();
		///@brief ���͐ݒ��������
		///@param[in] joyID �W���C�p�b�h�̔ԍ��i0,1,2...�ƃR���g���[���[�̐��ɉ����Ĉ���ݒ肷��j
		///@param[in] handle SystemClass�̎��E�B���h�E�̃n���h��
		///@return ���������true
		bool Initialize(unsigned int joyID, GLFWwindow* handle);
		///@brief ���͏����X�V
		void Run();

		///@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		///@return ���̃{�^���������ꂽ�u��true
		bool isPressButton(ButtonID buttonID);
		///@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		///@return ���̃{�^���������Ă����true
		bool isStayButton(ButtonID buttonID);
		///@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		///@return ���̃{�^���������ꂽ�u��true
		bool isReaveButton(ButtonID buttonID);

		///@return �X�e�B�b�N�P�̎��̈ʒu
		float GetStickState(ButtonID axisID);
		///@return �Q���X�e�B�b�N�̊p�x���擾�iX����������n�܂郉�W�A���p�x�j
		float GetStickRotation(ButtonID stickID);
		///@return �Q���X�e�B�b�N�̌X�����擾
		float GetStickPower(ButtonID stickID);

	private:
		void GetInputState();

	private:
		GLFWwindow * window;
		//�R���g���[���[ID��JOYINFOEX�\����
		unsigned int joyID;

		//���z�R���g���[���[�̃{�^���Ή��Ɠ��͏��̔z��
		JoyButtonState vpadState[10] = {};

		//�X�e�B�b�N�̏��
		JoyStickState vpadStickState[2] = {};
		JoyStickAxis vpadStickAxis[6] = {};	//0 �` 1 �̊Ԃɒl�����܂��Ă���
		float stickState[6] = {};		//�l���O����P�Ɏ��܂��Ă��Ȃ�
	};

}
