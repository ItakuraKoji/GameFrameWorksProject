#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	//OpenAL�����b�s���O�����N���X
	class DLL_DECLSPEC SoundClass {
	public:
		SoundClass();
		~SoundClass();

		//!�s�b�`���グ��ƃX�g���[�~���O���ǂ��t���Ȃ����Ƃ�����A���̂Ƃ���numBuffer�̒l��傫������Ƃ���
		void CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, float baseVolume = 1.0f, int numBuffer = 32);
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
