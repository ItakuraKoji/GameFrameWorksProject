#pragma once
#include<unordered_map>
#include"VirtualGamePad.h"

namespace K_Input {
	//!@brief ���͊֘A���擾�ł���N���X
	class InputClass {
	public:
		InputClass(GLFWwindow* handle);
		~InputClass();

		//!@brief ���͏���S�čX�V
		void Run();

		//!@brief �Q�[���p�b�h��ǉ�
		//!@input �Q�[���p�b�h��ID
		//!@return �ǉ��Ɏ��s�i���łɓo�^����Ă����j�ꍇ��false
		bool AddGamePad(VpadIndex padID);
		//!@brief �Q�[���p�b�h�̎擾
		//!@input �Q�[���p�b�h��ID
		//!@return �Q�[���p�b�h���o�^����Ă��Ȃ�������nullptr���Ԃ�
		VirtualGamePad* GetPad(VpadIndex padID);

		//!@param[in] buttonID ���ׂ�}�E�X�̃{�^��
		//!@return ���̃{�^���������ꂽ�u��true
		bool IsPressMouse(VMouse buttonID);
		//!@param[in] buttonID ���ׂ�}�E�X�̃{�^��
		//!@return ���̃{�^���������Ă����true
		bool IsStayMouse(VMouse buttonID);
		//!@param[in] buttonID ���ׂ�}�E�X�̃{�^��
		//!@return ���̃{�^���������ꂽ�u��true
		bool IsReaveMouse(VMouse buttonID);
		//!@return �E�B���h�E��������_�Ƃ����}�E�X�ʒu
		K_Math::Vector2& GetMousePosition();

	private:
		void GetMouseState();

	private:
		//GLFW�̃E�B���h�E���
		GLFWwindow * windowHandle;

		//�}�E�X���
		K_Math::Vector2 mousePosition;
		MouseState vMouseButton[VMouseCount] = {};

		std::unordered_map<VpadIndex, VirtualGamePad*> gamePad;
	};
}