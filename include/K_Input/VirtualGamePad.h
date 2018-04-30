#pragma once
#include"VpadStates.h"

namespace K_Input {

	//!@brief GLFW�𗘗p�����L�[���͂ƃW���C�p�b�h�̓��͂������N���X
	class VirtualGamePad {
	public:
		//!@brief ���͐ݒ��������
		//!@param[in] joyID �W���C�p�b�h�̔ԍ�
		//!@param[in] handle SystemClass�̎��E�B���h�E�̃n���h��
		VirtualGamePad(VpadIndex padID, GLFWwindow* handle);
		//!@brief �I������
		~VirtualGamePad();

		//!@brief ���͏����X�V
		void Run();

		//!@return �ڑ����ꂽ�Q�[���p�b�h�̖��O
		const char* GetJoyPadName();

		//!@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		//!@return ���̃{�^���������ꂽ�u��true
		bool IsPressButton(VpadButton buttonID);
		//!@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		//!@return ���̃{�^���������Ă����true
		bool IsStayButton(VpadButton buttonID);
		//!@param[in] buttonID ���ׂ鉼�z�L�[�̒l
		//!@return ���̃{�^���������ꂽ�u��true
		bool IsReaveButton(VpadButton buttonID);

		//!@return �Q�[���p�b�h���̈ʒu
		float GetAxisPosition(VpadAxis axisID);
		//!@return �Q���X�e�B�b�N�̊p�x���擾�iX����������n�܂郉�W�A���p�x�j
		float GetStickRotation(VpadStick stickID);
		//!@return �Q���X�e�B�b�N�̌X�����擾
		float GetStickPower(VpadStick stickID);

		//!@brief �w��̉��z�p�b�h�{�^���萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key board);
		//!@brief �w��̉��z�p�b�h���萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus);
		//!@brief �w��̉��z�p�b�h�X�e�B�b�N�萔�ƕ������͂Ƃ̑Ή���ݒ�
		void SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis);

	private:
		void GetButtonState();
		void GetAxisState();

	private:
		GLFWwindow* window;
		//�R���g���[���[ID��JOYINFOEX�\����
		VpadIndex vpadID;

		//���z�R���g���[���[�̃{�^���Ή��Ɠ��͏��̔z��
		ButtonState vpadButton[VButtonCount] = {};

		//�X�e�B�b�N�̏��
		StickState  vpadStick[VStickCount] = {};
		AxisState   vpadAxis[VAxisCount] = {};	//0 �` 1 �̊Ԃɒl�����܂��Ă���
		float       stickState[VAxisCount] = {};		//�l���O����P�Ɏ��܂��Ă��Ȃ�
	};

}
