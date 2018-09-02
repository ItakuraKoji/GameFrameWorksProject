#pragma once
#include"VpadStates.h"
#include<vector>

namespace K_Input {

	//!@brief GLFWを利用したキー入力とジョイパッドの入力を扱うクラス
	class VirtualGamePad {
	public:
		//!@brief 入力設定を初期化
		//!@param[in] joyID ジョイパッドの番号
		//!@param[in] handle SystemClassの持つウィンドウのハンドル
		VirtualGamePad(VpadIndex padID, GLFWwindow* handle);
		//!@brief 終了処理
		~VirtualGamePad();

		//!@brief 入力情報を更新
		void Run();

		//!@return 接続されたゲームパッドの名前
		const char* GetJoyPadName();

		//!@param[in] buttonID 調べる仮想キーの値
		//!@return そのボタンが押された瞬間true
		bool IsPressButton(VpadButton buttonID);
		//!@param[in] buttonID 調べる仮想キーの値
		//!@return そのボタンを押している間true
		bool IsStayButton(VpadButton buttonID);
		//!@param[in] buttonID 調べる仮想キーの値
		//!@return そのボタンが離された瞬間true
		bool IsReaveButton(VpadButton buttonID);

		//!@return そのボタンが押された瞬間true
		bool IsPressButtonAny();
		//!@return そのボタンを押している間true
		bool IsStayButtonAny();
		//!@return そのボタンが離された瞬間true
		bool IsReaveButtonAny();

#if 0
		//TODO:未実装なので必要になったあたりで作っておく

		//!現在押されているゲームパッドのボタン
		std::vector<VpadButton> GetPressButtonID();

		//!現在倒されているゲームパッドの軸
		//!@param[in] judgePower この値より倒されている軸が返ってくる
		std::vector<VpadAxis> GetActiveStickID(float judgePower);
#endif


		//!@return ゲームパッド軸の位置
		float GetAxisPosition(VpadAxis axisID);
		//!@return ２軸スティックの角度を取得（X軸方向から始まるラジアン角度）
		float GetStickRotation(VpadStick stickID);
		//!@return ２軸スティックの傾きを取得
		float GetStickPower(VpadStick stickID);

		//!@brief 指定の仮想パッドボタン定数と物理入力との対応を設定
		void SetButtonConfig(VpadButton vpadButton, JoyButton joypadButton, Key board);
		//!@brief 指定の仮想パッド軸定数と物理入力との対応を設定
		void SetAxisConfig(VpadAxis vpadAxis, JoyAxis joypadAxis, Key axisPlus, Key axisMinus);
		//!@brief 指定の仮想パッドスティック定数と物理入力との対応を設定
		void SetStickConfig(VpadStick vpadStick, VpadAxis xAxis, VpadAxis yAxis);


		//!現在押されているゲームパッドのボタン
		std::vector<JoyButton> GetPressRawButtonID();

		//!現在倒されているゲームパッドの軸
		//!@param[in] judgePower この値より倒されている軸が返ってくる
		std::vector<JoyAxis> GetActiveRawAxisID(float judgePower);

	private:
		void GetButtonState();
		void GetAxisState();

	private:
		GLFWwindow* window;
		//コントローラーIDとJOYINFOEX構造体
		VpadIndex vpadID;

		//仮想コントローラーのボタン対応と入力情報の配列
		ButtonState vpadButton[VButtonCount] = {};
		//いずれかのボタンが押されているか、というフラグ
		//2フレーム分のデータがある
		bool anyKeyPressPrev;
		bool anyKeyPress;

		//スティックの状態
		StickState  vpadStick[VStickCount] = {};
		AxisState   vpadAxis[VAxisCount] = {};	//0 ～ 1 の間に値が収まっている
		float       stickState[VAxisCount] = {};		//値が０から１に収まっていない
	};

}
