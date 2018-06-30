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


	//音源クラス。ループフラグが下りているときはファイル終端に到達した時点でストリーミングを終える
	class DLL_DECLSPEC SoundSource {
	public:
		enum LoadMode {
			Streaming,
			AllRead,
		};

	public:
		SoundSource(const char* sourceName, const char* filePass, LoadMode mode, int numBuffer);
		~SoundSource();
		void Play(bool loop);
		void PlayCopy();
		void Pause();
		void Stop();

		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetPosition(float x, float y, float z);
		void SetVelocity(float x, float y, float z);
		bool IsPlay();

	private:
		void Finalize();
		void StreamingThread();
		void AllReadThread();
		void EndThread();
		void FillBuffer();
		int ReadBuffer(char* buffer, int maxReadSize);
		int OggCommentValue(vorbis_comment* comment, const char* key);

	private:
		const std::string name;
		AudioData* audio;
		LoadMode mode;
		std::vector<char> allReadData;
		//CopyPlay用の管理リスト
		std::list<ALuint> copySources;

		std::thread* thread;
		std::recursive_mutex _mutex;

		ALuint sourceID;
		ALuint *bufferIDs;
		int numBuffer;

		ALuint format;

		float volume;
		float pitch;
		float posX, posY, posZ;
		float velocityX, velocityY, velocityZ;

		bool isPlayed;
		bool isLoop;
		bool isEnd;
	};

}
