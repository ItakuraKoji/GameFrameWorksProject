#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<string>
#include<thread>
#include<mutex>
#include<iostream>
#include<vector>
#include<list>


#include"AudioDataFactory.h"


namespace K_Audio {
	//�O���錾
	class SoundClass;

	//�����N���X�B���[�v�t���O������Ă���Ƃ��̓t�@�C���I�[�ɓ��B�������_�ŃX�g���[�~���O���I����
	class DLL_DECLSPEC SoundSource {
	public:
		enum LoadMode {
			Streaming,
			AllRead,
		};

	public:
		SoundSource(SoundClass* soundManager, int group, const char* sourceName, const char* filePass, LoadMode mode, float baseVolume, int numBuffer);
		~SoundSource();
		void Play(bool loop);
		ALuint PlayCopy();
		ALuint PlayCopy(float volume, float pitch);
		ALuint PlayCopy(float posX, float posY, float posZ, float volume, float pitch);
		void Pause();
		void Stop();

		//PlayCopy���܂߂đS���~�߂�
		void StopAll();


		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetPosition(float x, float y, float z);
		void SetVelocity(float x, float y, float z);
		bool IsPlay();

		int GetSoundGroup();

	private:
		void SetSourceVolume(ALuint sourceID, float volume);

		void Finalize();
		void StreamingThread();
		void AllReadThread();
		void EndThread();
		void FillBuffer();
		int ReadBuffer(char* buffer, int maxReadSize);
		int OggCommentValue(vorbis_comment* comment, const char* key);

	private:
		//�}�X�^�[�{�����[���ւ̃A�N�Z�X�p
		SoundClass* soundManager;
		//���̃T�E���h��������O���[�v
		int group;

		const std::string name;
		AudioData* audio;
		LoadMode mode;
		std::vector<char> allReadData;
		//CopyPlay�p�̊Ǘ����X�g
		std::list<ALuint> copySources;

		//�T�E���h�𗬂��X���b�h
		std::thread* thread;
		std::recursive_mutex _mutex;

		ALuint sourceID;
		ALuint *bufferIDs;
		int numBuffer;

		ALuint format;

		//���̃T�E���h�̃j���[�g�����̉���
		float baseVolume;

		//���̃T�E���h�̉σ{�����[��
		float volume;
		float pitch;
		float posX, posY, posZ;
		float velocityX, velocityY, velocityZ;

		bool isPlayed;
		bool isLoop;
		bool isEnd;
	};

}
