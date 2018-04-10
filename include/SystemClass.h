#pragma once

#define GLEW_STATIC

#include<Windows.h>
#include<iostream>
#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"K_Math\MyMathFanctions.h"
#include"K_Input\InputGLFW.h"


namespace K_System {

	///
	///アプリケーションの動作を管理
	///@details 一番初めに初期化し、IsSystemEnd()がtrueになるまでの間アプリケーションを動かす\nなお、「キー入力・ゲームパッド入力」の情報はこのクラスにある
	///
	class SystemClass {
	public:
		///Initialize()を呼ぶ
		SystemClass(int windowWidth, int windowHeight, bool isFullScreen);
		///Finalize()を呼ぶ
		~SystemClass();
		///@brief 描画周りの初期化
		///@param[in] windowWidth 作成するウィンドウの幅
		///@param[in] windowHeight 作成するウィンドウの高さ
		///@param[in] isFullScreen フルスクリーンにするか（ウィンドウのサイズが解像度になります）
		///@return 初期化に成功するとtrue
		bool Initialize(int windowWidth, int windowHeight, bool isFullScreen);
		///@brief 終了処理、Initialize()の初めにも呼ばれている
		void Finalize();
		///@brief ウィンドウイベントと入力の更新処理を行う
		void ProcessSystem();
		///@brief 描画を反映、これを呼ぶとフレームレートが60FPSになる
		void SwapBuffer();
		///@brief ウィンドウがフォーカスされているか
		///@return ウィンドウがフォーカスされているときtrue
		bool WindowForcus();
		///@brief アプリの終了通知
		///@return ウィンドウが閉じられたり、Escキーが押下されたときにtrue
		bool IsSystemEnd();
		///@brief アクセサー
		///@return ウィンドウの幅
		int GetWindowWidth();
		///@brief アクセサー
		///@return ウィンドウの高さ
		int GetWindowHeight();
		///@brief アクセサー
		///@return 入力クラスへのポインタ
		K_Input::InputGLFW* GetInput();

	private:
		bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
		bool InitializeOpenGL();

	private:
		int windowWidth, windowHeight;
		bool isFullScreen;
		bool isFocus;
		bool windowClosed;
		GLFWwindow* windowHandle;
		K_Input::InputGLFW* input;

		//FPS計測
		float fps;
		std::chrono::system_clock::time_point cullentTime;
		std::chrono::system_clock::time_point startTime;
		int framecount;
	};
}
