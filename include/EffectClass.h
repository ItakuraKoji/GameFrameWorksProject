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

	class EffectClass {
	public:
		EffectClass();
		~EffectClass();

		void SetMatrix(CameraClass* viewCamera);
		//エフェクト全体の更新
		void Run();
		//エフェクトはこの関数でまとめて描画される
		void Draw();

		//エフェクト読み込み
		void AddEffectSource(const std::string& effectName, const char* filePass);
		void DeleteEffectSource(const std::string& effectName);

		//再生
		//エフェクト名、位置を渡す
		//エフェクトを走査するハンドル番号が返る、移動などの操作をしないなら変数で取っておかなくてもいい
		EffectHandle Play(const std::string& effectName, float posX, float posY, float posZ);
		//明示的に停止、こちらは呼んだ瞬間にすぐ消える
		void Stop(EffectHandle handle);
		//明示的に停止、ただしエフェクトの生成をやめるだけなのでしばらく残る
		void StopRoot(EffectHandle handle);



	private:
		Effekseer::Manager* manager = nullptr;
		EffekseerRenderer::Renderer* renderer = nullptr;
		EffekseerSound::Sound* sound = nullptr;
		std::unordered_map<std::string, Effekseer::Effect*> effect;
		CameraClass* viewCamera;
	};
}
