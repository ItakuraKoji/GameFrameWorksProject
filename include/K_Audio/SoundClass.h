#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	//OpenALをラッピングしたクラス
	class DLL_DECLSPEC SoundClass {
	public:
		SoundClass();
		~SoundClass();

		//!ピッチを上げるとストリーミングが追い付かないことがある、そのときはnumBufferの値を大きくするといい
		void CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, int numBuffer = 32);
		void DeleteSource(const char* sourceName);
		void SetListnerPosition(float x, float y, float z);
		void SetListnerVelocity(float x, float y, float z);

		SoundSource* GetSource(const char* sourceName);

	private:
		ALCdevice * device;
		ALCcontext* context;

		std::unordered_map<std::string, SoundSource*> source;
	};
}
