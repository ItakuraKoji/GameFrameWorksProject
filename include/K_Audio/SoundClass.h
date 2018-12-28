#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	//OpenALをラッピングしたクラス
	class DLL_DECLSPEC SoundClass {
	private:

		struct SoundGroup {
		public:
			SoundGroup();

		public:
			std::vector<SoundSource*> source;//このグループに属するサウンドへのポインタ
			float groupVolume;//グループ全体のマスターボリューム
		};

	public:
		SoundClass();
		~SoundClass();

		void CreateSoundGroup(int size);
		//!ピッチを上げるとストリーミングが追い付かないことがある、そのときはnumBufferの値を大きくするといい
		void CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, int group = 0, float baseVolume = 1.0f, int numBuffer = 32);
		void DeleteSource(const char* sourceName);
		void SetListnerPosition(float x, float y, float z);
		void SetListnerVelocity(float x, float y, float z);

		void SetMasterVolume(float volume);
		void SetGroupVolume(int group, float volume);

		SoundSource* GetSource(const char* sourceName);
		void StopSoundGroup(int group);
		void StopSoundAll();

		float GetMasterVolume();
		float GetGroupVolume(int group);

	private:
		ALCdevice * device;
		ALCcontext* context;

		std::unordered_map<std::string, SoundSource*> source;
		std::vector<SoundGroup> soundGroup;

		//全ての音源のボリューム
		float masterVolume;
	};
}
