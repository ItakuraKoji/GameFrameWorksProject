#include"SoundSource.h"

namespace K_Audio {

	////////
	//public
	////
	SoundSource::SoundSource(const char* sourceName, const char* filePass, LoadMode mode, int numBuffer) : name(sourceName) {
		this->bufferIDs = new ALuint[numBuffer];
		this->isPlayed = false;
		this->isLoop = false;
		this->isEnd = false;
		this->numBuffer = numBuffer;
		this->mode = mode;


		alGenSources(1, &this->sourceID);
		alDistanceModel(AL_EXPONENT_DISTANCE);

		//読み込み
		AudioDataFactory factory;
		this->audio = factory.Create(filePass);

		if (this->audio->GetFormat() == AudioData::SoundFormat::Stereo16) {
			this->format = AL_FORMAT_STEREO16;
		}
		else {
			this->format = AL_FORMAT_MONO16;
		}
		//バッファ生成＆事前読み込み
		switch (this->mode) {
		case LoadMode::Streaming:
			//ストリーミングモードはバッファを作ってキューする
			char buffer[4096];
			for (int i = 0; i < this->numBuffer; ++i) {
				alGenBuffers(1, &this->bufferIDs[i]);
				int readSize = ReadBuffer(buffer, 4096);
				alBufferData(this->bufferIDs[i], this->format, buffer, readSize, this->audio->GetSamplingRate());
				alSourceQueueBuffers(this->sourceID, 1, &this->bufferIDs[i]);
			}
			//スレッド開始 (スレッドにメンバー関数を指定する際は第二引数にthisポインターを指定する)
			this->thread = new std::thread(&SoundSource::StreamingThread, this);
			break;

		case LoadMode::AllRead:
			//オールリードモードはすべて読んでバッファに突っ込む
			int size = this->audio->GetLoopLength() * this->audio->GetBlockSize();
			this->allReadData.resize(size);
			int readSize = ReadBuffer(this->allReadData.data(), size);
			alGenBuffers(1, &this->bufferIDs[0]);
			alBufferData(this->bufferIDs[0], this->format, this->allReadData.data(), readSize, this->audio->GetSamplingRate());
			alSourcei(this->sourceID, AL_BUFFER, this->bufferIDs[0]);
			this->numBuffer = 1;
			this->thread = new std::thread(&SoundSource::AllReadThread, this);
			break;
		}

	}

	SoundSource::~SoundSource() {
		EndThread();

		if (this->thread != nullptr) {
			this->thread->join();
			delete this->thread;
			this->thread = nullptr;
		}
		delete this->audio;

		alDeleteBuffers(this->numBuffer, this->bufferIDs);
		alDeleteSources(1, &this->sourceID);

		delete[] this->bufferIDs;
	}


	void SoundSource::Play(bool loop) {
		ALint state;
		alGetSourcei(this->sourceID, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			return;
		}
		alSourcePlay(this->sourceID);
		std::lock_guard<std::recursive_mutex> lock(this->_mutex);
		this->isPlayed = true;
		this->isLoop = loop;
	}

	void SoundSource::PlayCopy() {
		//ストリーミングモードでは利用できない
		if (this->mode == LoadMode::Streaming) {
			return;
		}
		ALuint source;
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->isPlayed = true;
			this->isLoop = false;
			alGenSources(1, &source);
			alSourcei(source, AL_BUFFER, this->bufferIDs[0]);
			this->copySources.push_back(source);
		}
		alSourcePlay(source);
	}


	void SoundSource::Pause() {
		alSourceStop(this->sourceID);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->isPlayed = false;
		}
	}

	void SoundSource::Stop() {
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->isPlayed = false;
		}
		//バッファを初期化
		int num;
		ALuint soundBuffer;
		while (alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &num), num > 0) {
			alSourceUnqueueBuffers(this->sourceID, 1, &soundBuffer);
		}

		alSourceStop(this->sourceID);

		this->audio->Seek(0);
		char buffer[4096];
		for (int i = 0; i < this->numBuffer; ++i) {
			int readSize = ReadBuffer(buffer, 4096);
			alBufferData(this->bufferIDs[i], this->format, buffer, readSize, this->audio->GetSamplingRate());
			alSourceQueueBuffers(this->sourceID, 1, &this->bufferIDs[i]);
		}
	}

	void SoundSource::SetVolume(float volume) {
		alSourcef(this->sourceID, AL_MAX_GAIN, volume);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->volume = volume;
		}
	}
	void SoundSource::SetPosition(float x, float y, float z) {
		alSource3f(this->sourceID, AL_POSITION, x, y, z);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->posX = x;
			this->posY = y;
			this->posZ = z;
		}
	}
	void SoundSource::SetVelocity(float x, float y, float z) {
		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->velocityX = x;
			this->velocityY = y;
			this->velocityZ = z;
		}
	}

	bool SoundSource::IsPlay() {
		std::lock_guard<std::recursive_mutex> lock(this->_mutex);
		return this->isPlayed;
	}

	////////
	//private
	////

	//ストリーミングして再生するスレッド
	void SoundSource::StreamingThread() {
		while (!this->isEnd) {
			if (!this->isPlayed) {
				//スリープはだいたい1フレーム分のウェイト
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
				continue;
			}

			ALint state;
			alGetSourcei(this->sourceID, AL_SOURCE_STATE, &state);

			if (state != AL_PLAYING) {
				alSourcePlay(this->sourceID);
			}
			else {
				FillBuffer();
			}
		}
	}

	//AllReadの場合は、生成したソースのコピーの管理を行う
	void SoundSource::AllReadThread() {

		//再生終了したら削除
		while (!this->isEnd) {
			//大体1秒に一回でも行けそうではあるが、とりあえず60FPS相当のウェイト
			std::this_thread::sleep_for(std::chrono::milliseconds(16));

			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			for (auto it = this->copySources.begin(); it != this->copySources.end();) {
				ALint state;
				alGetSourcei((*it), AL_SOURCE_STATE, &state);

				if (state != AL_PLAYING) {
					alDeleteSources(1, &(*it));
					it = this->copySources.erase(it);
					continue;
				}
				++it;
			}
		}

		for (auto i : this->copySources) {
			alDeleteSources(1, &i);
		}
	}


	//フラグを立ててスレッドを終わらせる
	void SoundSource::EndThread() {
		alSourceStop(this->sourceID);
		alSourcei(this->sourceID, AL_BUFFER, AL_NONE);

		std::lock_guard<std::recursive_mutex> lock(this->_mutex);
		this->isEnd = true;
		this->isPlayed = false;
	}

	//LOOPSTARTとLOOPLENGTHを走査
	int SoundSource::OggCommentValue(vorbis_comment* comment, const char* key) {
		int loopStart = 0;
		for (int i = 0; i < comment->comments; ++i) {
			std::string str(comment->user_comments[i]);
			std::string keyword(key);

			//keywordとstrとの部分一致を判定
			if (str.find(keyword) != std::string::npos) {
				int start = (int)str.find("=") + 1;
				int end = (int)str.length();
				//コメントに付属しているPCMの位置を取得
				return std::stoi(str.substr(start, end - start));
			}
		}
		//見つからないときは 0 を返す
		return 0;
	}

	//基本4096バイト、実際にバッファを読む関数
	void SoundSource::FillBuffer() {
		int size = 0;

		char buffer[4096];
		int fillSize = 4096;
		int offset = this->audio->GetPcmOffset();
		int loopEnd = this->audio->GetLoopStart() + this->audio->GetLoopLength();

		//4096バイトよりループ終端までの距離が短いときはそっちを読み込み量とする
		if (fillSize > (loopEnd - offset) * this->audio->GetBlockSize()) {
			fillSize = (loopEnd - offset) * this->audio->GetBlockSize();
		}

		//処理済みキューがない場合はそのまま帰る予定
		int numProcessed = 0;

		while (this->isPlayed) {
			alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &numProcessed);
			if (numProcessed == 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(600));
				continue;
			}

			ALuint soundBuffer;
			alSourceUnqueueBuffers(this->sourceID, 1, &soundBuffer);

			int readSize = ReadBuffer(buffer, fillSize - size);
			alBufferData(soundBuffer, this->format, buffer, readSize, this->audio->GetSamplingRate());
			alSourceQueueBuffers(this->sourceID, 1, &soundBuffer);

			offset = this->audio->GetPcmOffset();
			if (readSize == 0 || offset == loopEnd) {
				//ループしない場合は終端に達したらストリーミングを停止
				if (this->isLoop) {
					this->audio->Seek(this->audio->GetLoopStart());
					offset = this->audio->GetLoopStart();
					//std::cout << "Loop!" << std::endl;
				}
				else {
					Stop();
					break;
				}
			}

			size += readSize;
			if (size == fillSize) {
				break;
			}
			alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &numProcessed);
		}
	}

	//bufferに最大maxReadSize分だけ読んでキューする、実際の読み込みサイズが返る
	int SoundSource::ReadBuffer(char* buffer, int maxReadSize) {
		//readBufferが埋まるまで読みこむ
		int readSize = 0;
		while (readSize != maxReadSize) {
			char tmp[4096];
			int size = maxReadSize - readSize;
			if (size > 4096) {
				size = 4096;
			}
			int singleReadSize = this->audio->Read(tmp, size);
			if (singleReadSize <= 0) {
				break;
			}
			for (int i = 0; i < singleReadSize; ++i) {
				buffer[i + readSize] = tmp[i];
			}
			readSize += singleReadSize;
		}
		if (readSize == 0) {
			return 0;
		}

		return readSize;
	}

}