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


	///@brief 音源クラス\nループフラグが下りているときはファイル終端に到達した時点でストリーミングを終える
	class DLL_DECLSPEC SoundSource {
	public:
		///@brief 「全て読み込む」「ストリーミング再生」の二つを持つ
		enum LoadMode {
			Streaming,
			AllRead,
		};

	public:
		///@brief 情報を初期化
		///@param[in] sourceName このソースのユーザー定義名
		///@param[in] filePass 音声ファイルのパス
		///@param[in] mode 読み込みモード（PlayCopy()はAllReadモードの時にのみ使用可能）
		///@param[in] numBuffer ストリーミング再生時のバッファの数（省略時32個）
		SoundSource(const char* sourceName, const char* filePass, LoadMode mode, int numBuffer = 32);
		///@brief 音声ファイルのポインタをdeleteする
		~SoundSource();

		///@brief 音声を再生
		///@param[in] loop trueの時に音声を繰り返し再生
		void Play(bool loop);
		///@brief 音声をコピーして再生（LoadModeがAllRead時のみ使え、連続再生時音が重なる）
		void PlayCopy();
		///@brief 音声を一時停止
		void Pause();
		///@brief 音声を停止
		void Stop();

		///@brief 音声のボリュームを設定（0.0fが最低で、1.0fが最大）
		///@param[in] volume 音量の値
		void SetVolume(float volume);
		///@brief 音声の位置を設定
		///@param[in] x 位置座標X軸
		///@param[in] y 位置座標Y軸
		///@param[in] z 位置座標Z軸
		void SetPosition(float x, float y, float z);
		///@brief 音声の移動速度を設定（ドップラー効果を得られる）
		///@param[in] x 速度X成分
		///@param[in] y 速度Y成分
		///@param[in] z 速度Z成分
		void SetVelocity(float x, float y, float z);
		///@return 音声が再生中か
		bool IsPlay();

	private:
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
		float posX, posY, posZ;
		float velocityX, velocityY, velocityZ;

		bool isPlayed;
		bool isLoop;
		bool isEnd;
	};

}
