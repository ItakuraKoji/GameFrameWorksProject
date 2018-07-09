#include"SoundClass.h"

namespace K_Audio {

	////////
	//public
	////

	SoundClass::SoundClass() {
		this->device = nullptr;
		this->context = nullptr;

		this->device = alcOpenDevice(nullptr);
		if (this->device == nullptr) {
			throw std::runtime_error("OpenAL Initialize Failed : device");
		}
		this->context = alcCreateContext(this->device, nullptr);
		if (this->context == nullptr) {
			alcCloseDevice(this->device);
			throw std::runtime_error("OpenAL Initialize Failed : context");
		}
		if (alcMakeContextCurrent(this->context) == ALC_FALSE) {
			alcDestroyContext(this->context);
			alcCloseDevice(this->device);
			throw std::runtime_error("OpenAL Initialize Failed : alcMakeContextCullent() Failed");
		}
		SetListnerPosition(0.0f, 0.0f, 0.0f);
	}
	SoundClass::~SoundClass() {
		for (auto i : this->source) {
			delete i.second;
		}
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(this->context);
		alcCloseDevice(this->device);
	}


	void SoundClass::CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, int numBuffer) {
		//ソース名の重複は許さない
		if (this->source.find(sourceName) != this->source.end()) {
			throw std::runtime_error("SoundClass createSource error sourceName has already existed : " + std::string(sourceName));
		}
		SoundSource* audioSource = new SoundSource(sourceName, filePass, mode, numBuffer);

		this->source[sourceName] = audioSource;
	}

	//ソースの明示的解放(ほっといてもデストラクタですべて解放される)
	void SoundClass::DeleteSource(const char* sourceName) {
		//ソース名の存在チェック
		if (this->source.find(sourceName) == this->source.end()) {
			throw std::runtime_error("SoundClass deleteSource error sourceName is not exist : " + std::string(sourceName));
		}
		delete this->source[sourceName];
		this->source.erase(sourceName);
	}

	//ソースの獲得
	SoundSource* SoundClass::GetSource(const char* sourceName) {
		//ソース名の存在チェック
		if (this->source.find(sourceName) == this->source.end()) {
			throw std::runtime_error("SoundClass deleteSource error sourceName is not exist : " + std::string(sourceName));
		}
		return this->source[sourceName];
	}

	//リスナーの移動
	void SoundClass::SetListnerPosition(float x, float y, float z) {
		alListener3f(AL_POSITION, x, y, z);
	}
	void SoundClass::SetListnerVelocity(float x, float y, float z) {
		alListener3f(AL_VELOCITY, x, y, z);
	}

}
