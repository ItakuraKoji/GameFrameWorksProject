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

		bool CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode);
		void DeleteSource(const char* sourceName);

		SoundSource* GetSource(const char* sourceName);

	private:
		ALCdevice * device;
		ALCcontext* context;

		std::unordered_map<std::string, SoundSource*> source;
	};
}
