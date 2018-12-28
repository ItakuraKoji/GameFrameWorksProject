#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	//OpenAL�����b�s���O�����N���X
	class DLL_DECLSPEC SoundClass {
	private:

		struct SoundGroup {
		public:
			SoundGroup();

		public:
			std::vector<SoundSource*> source;//���̃O���[�v�ɑ�����T�E���h�ւ̃|�C���^
			float groupVolume;//�O���[�v�S�̂̃}�X�^�[�{�����[��
		};

	public:
		SoundClass();
		~SoundClass();

		void CreateSoundGroup(int size);
		//!�s�b�`���グ��ƃX�g���[�~���O���ǂ��t���Ȃ����Ƃ�����A���̂Ƃ���numBuffer�̒l��傫������Ƃ���
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

		//�S�Ẳ����̃{�����[��
		float masterVolume;
	};
}
