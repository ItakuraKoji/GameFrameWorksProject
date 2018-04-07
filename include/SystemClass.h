#pragma once

#define GLEW_STATIC

#include<Windows.h>
#include<iostream>
#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"GameParameters.h"

//アプリケーションの動作を管理
class SystemClass {
public:
	SystemClass(int windowWidth, int windowHeight, bool isFullScreen);
	~SystemClass();
	bool Initialize(int windowWidth, int windowHeight, bool isFullScreen);
	void Finalize();
	//ウィンドウイベント、システム関連の更新処理を行う
	void ProcessSystem();
	//バッファ入れ替えによって描画を反映
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

	//FPS計測
	float fps;
	std::chrono::system_clock::time_point cullentTime;
	std::chrono::system_clock::time_point startTime;
	int framecount;
};