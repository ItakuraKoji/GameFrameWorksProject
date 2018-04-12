#pragma once
#include"VpadStates.h"

namespace K_Input {

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
		bool isPressButton(VpadButton buttonID);
		///@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		///@return ���̃{�^���������Ă����true
		bool isStayButton(VpadButton buttonID);
		///@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		///@return ���̃{�^���������ꂽ�u��true
		bool isReaveButton(VpadButton buttonID);

		///@return �X�e�B�b�N�P�̎��̈ʒu
		float GetStickState(VpadStick axisID);
		///@return �Q���X�e�B�b�N�̊p�x���擾�iX����������n�܂郉�W�A���p�x�j
		float GetStickRotation(VpadStick stickID);
		///@return �Q���X�e�B�b�N�̌X�����擾
		float GetStickPower(VpadStick stickID);

		///@brief �w��̉��z�p�b�h�{�^���萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key board);
		///@brief �w��̉��z�p�b�h���萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus);
		///@brief �w��̉��z�p�b�h�X�e�B�b�N�萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis);

	private:
		void GetButtonState();
		void GetAxisState();

	private:
		GLFWwindow * window;
		//�R���g���[���[ID��JOYINFOEX�\����
		unsigned int joyID;

		//���z�R���g���[���[�̃{�^���Ή��Ɠ��͏��̔z��
		ButtonState vpadButton[static_cast<int>(VpadButton::EnumSize)] = {};

		//�X�e�B�b�N�̏��
		StickState vpadStick[static_cast<int>(VpadStick::EnumSize)] = {};
		AxisState vpadAxis[static_cast<int>(VpadAxis::EnumSize)] = {};	//0 �` 1 �̊Ԃɒl�����܂��Ă���
		float stickState[static_cast<int>(VpadAxis::EnumSize)] = {};		//�l���O����P�Ɏ��܂��Ă��Ȃ�
	};

}
