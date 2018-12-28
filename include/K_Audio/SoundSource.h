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
	//前方宣言
	class SoundClass;

	//音源クラス。ループフラグが下りているときはファイル終端に到達した時点でストリーミングを終える
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

		//PlayCopyも含めて全部止める
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
		//マスターボリュームへのアクセス用
		SoundClass* soundManager;
		//このサウンドが属するグループ
		int group;

		const std::string name;
		AudioData* audio;
		LoadMode mode;
		std::vector<char> allReadData;
		//CopyPlay用の管理リスト
		std::list<ALuint> copySources;

		//サウンドを流すスレッド
		std::thread* thread;
		std::recursive_mutex _mutex;

		ALuint sourceID;
		ALuint *bufferIDs;
		int numBuffer;

		ALuint format;

		//このサウンドのニュートラルの音量
		float baseVolume;

		//このサウンドの可変ボリューム
		float volume;
		float pitch;
		float posX, posY, posZ;
		float velocityX, velocityY, velocityZ;

		bool isPlayed;
		bool isLoop;
		bool isEnd;
	};

}
