#include"InputClass.h"

namespace K_Input {
	////////
	//public
	////
	InputClass::InputClass(GLFWwindow* window) {
		this->windowHandle = window;

		//マウスにはコンフィグ機能はない（必要ない）
		this->vMouseButton[(int)VMouse::Left].button = MouseButton::Left;
		this->vMouseButton[(int)VMouse::Right].button = MouseButton::Right;
		this->vMouseButton[(int)VMouse::Middle].button = MouseButton::Middle;
	}
	InputClass::~InputClass() {
		for (auto i : this->gamePad) {
			delete i.second;
		}
	}

	void InputClass::Run() {
		for (auto i : this->gamePad) {
			i.second->Run();
		}
		GetMouseState();
	}

	bool InputClass::AddGamePad(VpadIndex padID) {
		if (this->gamePad.find(padID) != this->gamePad.end()) {
			return false;
		}
		VirtualGamePad* pad = new VirtualGamePad(padID, this->windowHandle);
		this->gamePad[padID] = pad;
		return true;
	}

	VirtualGamePad* InputClass::GetPad(VpadIndex padID) {
		if (this->gamePad.find(padID) == this->gamePad.end()) {
			return nullptr;
		}
		return this->gamePad[padID];
	}

	//仮想マウス
	//押した瞬間
	bool InputClass::IsPressMouse(VMouse buttonID) {
		return this->vMouseButton[(int)buttonID].press && !this->vMouseButton[(int)buttonID].prevPress;
	}
	//押している間
	bool InputClass::IsStayMouse(VMouse buttonID) {
		return (this->vMouseButton[(int)buttonID].press && this->vMouseButton[(int)buttonID].prevPress) || IsPressMouse(buttonID);
	}
	//離された瞬間
	bool InputClass::IsReaveMouse(VMouse buttonID) {
		return !this->vMouseButton[(int)buttonID].press && this->vMouseButton[(int)buttonID].prevPress;
	}
	K_Math::Vector2& InputClass::GetMousePosition() {
		return this->mousePosition;
	}

	////////
	//private
	////
	void InputClass::GetMouseState() {
		//ボタン押下状態
		for (int i = 0; i < VMouseCount; ++i) {
			this->vMouseButton[i].prevPress = this->vMouseButton[i].press;

			int state = glfwGetMouseButton(this->windowHandle, (int)this->vMouseButton[i].button);
			if (state == GLFW_PRESS) {
				this->vMouseButton[i].press = 1;
			}
			else {
				this->vMouseButton[i].press = 0;
			}
		}
		//位置
		double x, y;
		glfwGetCursorPos(this->windowHandle, &x, &y);
		this->mousePosition.x = (float)x;
		this->mousePosition.y = (float)y;
	}
}