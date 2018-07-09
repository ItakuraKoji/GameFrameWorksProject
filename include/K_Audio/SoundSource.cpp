#include"SoundSource.h"

namespace K_Audio {

	////////
	//public
	////
	SoundSource::SoundSource(const char* sourceName, const char* filePass, LoadMode mode, int numBuffer) : name(sourceName) {
		try {
			this->bufferIDs = nullptr;
			this->isPlayed = false;
			this->isLoop = false;
			this->isEnd = false;
			this->numBuffer = 0;
			this->mode = mode;
			this->volume = 1.0f;
			this->pitch = 1.0f;

			alGenSources(1, &this->sourceID);
			alDistanceModel(AL_EXPONENT_DISTANCE);

			//�ǂݍ���
			AudioDataFactory factory;
			this->audio = factory.Create(filePass);

			switch (this->audio->GetFormat()) {
			case AudioData::SoundFormat::Stereo16:
				this->format = AL_FORMAT_STEREO16;
				break;
			case AudioData::SoundFormat::Stereo8:
				this->format = AL_FORMAT_STEREO8;
				break;
			case AudioData::SoundFormat::Mono16:
				this->format = AL_FORMAT_MONO16;
				break;
			case AudioData::SoundFormat::Mono8:
				this->format = AL_FORMAT_MONO8;
				break;
			default:
				throw std::runtime_error("Unknown Sound Format");
				break;
			}

			this->bufferIDs = new ALuint[numBuffer];
			this->numBuffer = numBuffer;

			//�o�b�t�@���������O�ǂݍ���
			switch (this->mode) {
			case LoadMode::Streaming:
				//�X�g���[�~���O���[�h�̓o�b�t�@������ăL���[����
				char buffer[4096];
				for (int i = 0; i < this->numBuffer; ++i) {
					alGenBuffers(1, &this->bufferIDs[i]);
					int readSize = ReadBuffer(buffer, 4096);
					alBufferData(this->bufferIDs[i], this->format, buffer, readSize, this->audio->GetSamplingRate());
					alSourceQueueBuffers(this->sourceID, 1, &this->bufferIDs[i]);
				}
				//�X���b�h�J�n (�X���b�h�Ƀ����o�[�֐����w�肷��ۂ͑�������this�|�C���^�[���w�肷��)
				this->thread = new std::thread(&SoundSource::StreamingThread, this);
				break;

			case LoadMode::AllRead:
				//�I�[�����[�h���[�h�͂��ׂēǂ�Ńo�b�t�@�ɓ˂�����
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
		catch (std::exception& e) {
			Finalize();
			throw e;
		}
	}

	SoundSource::~SoundSource() {
		Finalize();
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
		//�X�g���[�~���O���[�h�ł͗��p�ł��Ȃ�
		if (this->mode == LoadMode::Streaming) {
			throw std::runtime_error("Streaming Sound Source can't use PlayCopy() :" + this->name);
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
		alSourcef(source, AL_MAX_GAIN, this->volume);
		alSourcef(source, AL_PITCH, this->pitch);
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
		//�o�b�t�@��������
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

	//���ʕύX
	void SoundSource::SetVolume(float volume) {
		alSourcef(this->sourceID, AL_MAX_GAIN, volume);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->volume = volume;
		}
	}
	//�s�b�`�ύX�A�O�ȉ��̓G���[
	void SoundSource::SetPitch(float pitch) {
		alSourcef(this->sourceID, AL_PITCH, pitch);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->pitch = pitch;
		}
	}

	//�ʒu�ύX�A�X�e���I�����ɂ͐ݒ�ł��Ȃ�
	void SoundSource::SetPosition(float x, float y, float z) {
		alSource3f(this->sourceID, AL_POSITION, x, y, z);
		{
			std::lock_guard<std::recursive_mutex> lock(this->_mutex);
			this->posX = x;
			this->posY = y;
			this->posZ = z;
		}
	}
	//���x�ݒ�A�h�b�v���[���ʂ𐶂�
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

	void SoundSource::Finalize() {
		EndThread();

		if (this->thread != nullptr) {
			this->thread->join();
			delete this->thread;
			this->thread = nullptr;
		}
		if (this->audio != nullptr) {
			delete this->audio;
		}

		alDeleteBuffers(this->numBuffer, this->bufferIDs);
		alDeleteSources(1, &this->sourceID);

		if (this->bufferIDs != nullptr) {
			delete[] this->bufferIDs;
		}
	}

	//�X�g���[�~���O���čĐ�����X���b�h
	void SoundSource::StreamingThread() {
		while (!this->isEnd) {
			if (!this->isPlayed) {
				//�X���[�v�͂�������1�t���[�����̃E�F�C�g
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

	//AllRead�̏ꍇ�́A���������\�[�X�̃R�s�[�̊Ǘ����s��
	void SoundSource::AllReadThread() {

		//�Đ��I��������폜
		while (!this->isEnd) {
			//���1�b�Ɉ��ł��s�������ł͂��邪�A�Ƃ肠����60FPS�����̃E�F�C�g
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


	//�t���O�𗧂ĂăX���b�h���I��点��
	void SoundSource::EndThread() {
		alSourceStop(this->sourceID);
		alSourcei(this->sourceID, AL_BUFFER, AL_NONE);

		std::lock_guard<std::recursive_mutex> lock(this->_mutex);
		this->isEnd = true;
		this->isPlayed = false;
	}

	//LOOPSTART��LOOPLENGTH�𑖍�
	int SoundSource::OggCommentValue(vorbis_comment* comment, const char* key) {
		int loopStart = 0;
		for (int i = 0; i < comment->comments; ++i) {
			std::string str(comment->user_comments[i]);
			std::string keyword(key);

			//keyword��str�Ƃ̕�����v�𔻒�
			if (str.find(keyword) != std::string::npos) {
				int start = (int)str.find("=") + 1;
				int end = (int)str.length();
				//�R�����g�ɕt�����Ă���PCM�̈ʒu���擾
				return std::stoi(str.substr(start, end - start));
			}
		}
		//������Ȃ��Ƃ��� 0 ��Ԃ�
		return 0;
	}

	//��{4096�o�C�g�A���ۂɃo�b�t�@��ǂފ֐�
	void SoundSource::FillBuffer() {
		//���݂̓ǂݍ��݃T�C�Y
		int size = 0;

		char buffer[4096];
		int fillSize = 4096;
		int offset = this->audio->GetPcmOffset();
		int loopEnd = this->audio->GetLoopStart() + this->audio->GetLoopLength();

		//4096�o�C�g��胋�[�v�I�[�܂ł̋������Z���Ƃ��͂�������ǂݍ��ݗʂƂ���
		if (fillSize > (loopEnd - offset) * this->audio->GetBlockSize()) {
			fillSize = (loopEnd - offset) * this->audio->GetBlockSize();
		}

		//�����ς݃L���[���Ȃ��ꍇ�͂��̂܂܋A��\��
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
				//���[�v���Ȃ��ꍇ�͏I�[�ɒB������X�g���[�~���O���~
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

	//buffer�ɍő�maxReadSize�������ǂ�ŃL���[����A���ۂ̓ǂݍ��݃T�C�Y���Ԃ�
	int SoundSource::ReadBuffer(char* buffer, int maxReadSize) {
		//readBuffer�����܂�܂œǂ݂���
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