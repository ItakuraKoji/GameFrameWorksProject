#pragma once

#define __EFFEKSEER_RENDERER_GL_GLEW__

#include<Effekseer\Effekseer.h>
#include<Effekseer\EffekseerRendererGL.h>
#include<Effekseer\EffekseerSoundAL.h>
#include<unordered_map>
#include<string>
#include<gl\GLU.h>

#include"CameraClass.h"

namespace K_Graphics {

	typedef Effekseer::Handle EffectHandle;

	//!@brief エフェクト管理クラス\nほぼEffekseerの機能をラッピングしただけにとどまっている
	class EffectClass {
	public:
		//!@brief Effekseerを初期化
		EffectClass();
		//!@brief Effekseerの終了処理
		~EffectClass();

		//!@brief エフェクト描画に使うカメラを設定
		//!@param[in] カメラのポインタ
		void SetMatrix(CameraClass* viewCamera);
		//!@brief エフェクト全体の更新
		void Run();
		//!@brief エフェクトはこの関数でまとめて描画される
		void Draw();

		//!@brief エフェクトの読み込み
		//!@param[in] effectName エフェクトのユーザー定義名（同じ名前は登録できない）
		//!@param[in] filePass Effekseerで製作したエフェクトへのファイルパス
		bool AddEffectSource(const std::string& effectName, const char* filePass);
		//!@brief エフェクトの消去
		//!@param[in] effectName エフェクト名
		void DeleteEffectSource(const std::string& effectName);

		//!@brief エフェクトを指定位置で再生
		//!@param[in] effectName エフェクト名
		//!@param[in] posX 位置X座標
		//!@param[in] posY 位置Y座標
		//!@param[in] posZ 位置Z座標
		//!@return エフェクトを走査するハンドル番号、移動などの操作をしないなら変数で取っておかなくてもいい
		EffectHandle Play(const std::string& effectName, float posX, float posY, float posZ);
		//!@brief エフェクトを明示的に停止、こちらは呼んだ瞬間にすぐ消える
		//!@param[in] handle Play()で帰ってくるエフェクト番号
		void Stop(EffectHandle handle);
		//!@brief エフェクトを明示的に停止、ただしエフェクトの生成をやめるだけなのでしばらく残る
		//!@param[in] handle Play()で帰ってくるエフェクト番号
		void StopRoot(EffectHandle handle);


	private:
		Effekseer::Manager* manager = nullptr;
		EffekseerRenderer::Renderer* renderer = nullptr;
		EffekseerSound::Sound* sound = nullptr;
		std::unordered_map<std::string, Effekseer::Effect*> effect;
		CameraClass* viewCamera;
	};
}
