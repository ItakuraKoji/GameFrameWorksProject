#pragma once
#define GLEW_STATIC

#include<chrono>

#include<GLEW/glew.h>
#include<GLFW/glfw3.h>

#include"K_Math\MyMathFanctions.h"
#include"K_Input\InputClass.h"

//リファレンス用
//!@brief グラフィック関連の名前空間\n
//!@details・主要クラス\n
//!3Dモデルの表示　:「MeshObject」「SpriteObject」\n
//!カメラ　　　　　:「CameraList」が管理する「CameraClass」\n
//!光源　　　　　　:「LightList」\n
//!画像　　　　　　:「TextureList」が管理する「Texture」\n
//!シェーダー　　　:「ShaderList」が管理する「ShaderClass」\n
//!描画バッファ　　:「FrameBufferList」\n
namespace K_Graphics {}
//!@brief 物理衝突判定関連の名前空間
//!@details・主要クラス\n
//!コリジョン作成・衝突判定　：「BulletPhysics」
//!地形データ作成　　　　　　：「MapPolygon」
namespace K_Physics {}
//!@brief 音源関連の名前空間
//!@details・主要クラス\n
//!音源の作成・リスナーの移動　：「SoundClass」
//!音源の再生・移動　　　　　　：「SoundSource」
namespace K_Audio {}
//!@brief 入力関連の名前空間
//!@details・主要クラス
//!入力の取得「InputGLFW」
namespace K_Input {}
//!@brief 行列系の関数とベクトルや行列型を提供する
//!@details主要クラスはなく、関数と型だけが定義されている
namespace K_Math {}

//!@brief 主にSystemClassを使い、アプリケーションの初期化を行う
namespace K_System {
	struct IconImage{
		unsigned char* image;
		int width;
		int height;
	};

	enum class CullingInfo {
		BackFace,
		FrontFace
	};
	enum class FrontFaceInfo
	{
		Crockwise,
		Counterclockwise
	};
	enum class BlendInfo {
		Multiply,
		Add
	};
	enum class IntervalType {
		FPS60,
		FPS30,
	};

	//!
	//!アプリケーションの動作を管理
	//!@brief 一番初めに初期化し、IsSystemEnd()がtrueになるまでの間アプリケーションを動かす\nなお、「キー入力・ゲームパッド入力」の情報はこのクラスにある
	//!
	class SystemClass {
	public:
		//!Initialize()を呼ぶ
		SystemClass(const std::string& windowName, int windowWidth, int windowHeight, bool isFullScreen);
		//!Finalize()を呼ぶ
		~SystemClass();
		//!@brief 描画周りの初期化
		//!@param[in] windowWidth 作成するウィンドウの名前
		//!@param[in] windowWidth 作成するウィンドウの幅
		//!@param[in] windowHeight 作成するウィンドウの高さ
		//!@param[in] isFullScreen フルスクリーンにするか（ウィンドウのサイズが解像度になります）
		//!@return 初期化に成功するとtrue
		bool Initialize(const std::string& windowName, int windowWidth, int windowHeight, bool isFullScreen);
		//!@brief 終了処理、Initialize()の初めにも呼ばれている
		void Finalize();
		//!@brief ウィンドウイベントと入力の更新処理を行う
		void ProcessSystem();
		//!@brief 描画を反映、これを呼ぶとフレームレートが60FPSになる
		void SwapBuffer();
		//!@brief ウィンドウがフォーカスされているか
		//!@return ウィンドウがフォーカスされているときtrue
		bool WindowForcus();
		//!@brief アプリの終了通知
		//!@return ウィンドウが閉じられたり、Escキーが押下されたときにtrue
		bool IsSystemEnd();
		//!@return ウィンドウの幅
		int GetWindowWidth();
		//!@return ウィンドウの高さ
		int GetWindowHeight();
		//!@return FramePerSecondの値
		float GetFPS();

		//!@brief ウィンドウを隠す
		//!@param[in] flag trueで隠し、falseで出す
		void HideWindow(bool flag);
		//!@brief ウィンドウのアイコンを変える
		//!@param[in] imageArray イメージ配列
		//!@param[in] count 配列数
		void SetIcon(IconImage* imageArray, int count);
		//!@brief 描画時の待ち時間を変更
		void SetDrawInterval(IntervalType type);

		//!@brief アクセサー
		//!@return 入力クラスへのポインタ
		K_Input::InputClass* GetInput();

		//!@brief ウィンドウを閉じてアプリケーションを終了する
		void EndSystem();

		//!@brief 描画のカリング設定
		//!@param[in] active trueでカリングを有効にする
		//!@param[in] cullingInfo カリングするのは表か裏かを設定する
		//!@param[in] frontFace ポリゴンの表面が反時計回りか時計回りかを設定する
		static void SetCulling(bool active, CullingInfo cullingInfo = CullingInfo::BackFace, FrontFaceInfo frontFaceInfo = FrontFaceInfo::Counterclockwise);
		//!@brief アルファブレンドの計算方法を設定
		//!@param[in] info 計算式を表す定数
		static void SetAlphaBlendInfo(bool active, BlendInfo info = BlendInfo::Multiply);
		//!@brief 深度テストを 有効 / 無効 にする
		//!@param[in] active trueで有効に、falseで無効にする
		static void SetDepthTest(bool active);


	private:
		bool CreateAppricationWindow(const char* windowName, int width, int height, bool fullScreen);
		bool InitializeOpenGL();

	private:
		int windowWidth, windowHeight;
		bool isFullScreen;
		bool isFocus;
		bool windowClosed;
		GLFWwindow* windowHandle;
		K_Input::InputClass* input;

		//FPS計測
		float fps;
		std::chrono::system_clock::time_point cullentTime;
		std::chrono::system_clock::time_point startTime;
		int framecount;
	};
}
