#pragma once

#define GLEW_STATIC

#include<Windows.h>
#include<iostream>
#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"GameParameters.h"

//�A�v���P�[�V�����̓�����Ǘ�
class SystemClass {
public:
	SystemClass(int windowWidth, int windowHeight, bool isFullScreen);
	~SystemClass();
	bool Initialize(int windowWidth, int windowHeight, bool isFullScreen);
	void Finalize();
	//�E�B���h�E�C�x���g�A�V�X�e���֘A�̍X�V�������s��
	void ProcessSystem();
	//�o�b�t�@����ւ��ɂ���ĕ`��𔽉f
	void SwapBuffer();

	bool WindowForcus();
	bool WindowClosed();
	int GetWindowWidth();
	int GetWindowHeight();
	GameParameters* GetParameters();

private:
	bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
	bool InitializeOpenGL();
	
private:
	int screenWidth, screenHeight;
	bool isFullScreen;
	bool isFocus;
	bool windowClosed;
	GLFWwindow* windowHandle;
	GameParameters* parameters;

	//FPS�v��
	float fps;
	std::chrono::system_clock::time_point cullentTime;
	std::chrono::system_clock::time_point startTime;
	int framecount;
};