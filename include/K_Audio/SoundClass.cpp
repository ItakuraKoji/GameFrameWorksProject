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
			throw("OpenAL Initialize Failed : device");
		}
		this->context = alcCreateContext(this->device, nullptr);
		if (this->context == nullptr) {
			alcCloseDevice(this->device);
			throw("OpenAL Initialize Failed : context");
		}
		if (alcMakeContextCurrent(this->context) == ALC_FALSE) {
			alcDestroyContext(this->context);
			alcCloseDevice(this->device);
			throw("OpenAL Initialize Failed : alcMakeContextCullent() Failed");
		}
	}
	SoundClass::~SoundClass() {
		for (auto i : this->source) {
			delete i.second;
		}
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(this->context);
		alcCloseDevice(this->device);
	}


	bool SoundClass::CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode) {
		//ソース名の重複は許さない
		if (this->source.find(sourceName) != this->source.end()) {
			return false;
		}
		SoundSource* audioSource = new SoundSource(sourceName, filePass, mode);
		if (!audioSource) {
			return false;
		}

		this->source[sourceName] = audioSource;
		return true;
	}

	//ソースの明示的解放(ほっといてもデストラクタですべて解放される)
	void SoundClass::DeleteSource(const char* sourceName) {
		//ソース名の存在チェック
		if (this->source.find(sourceName) == this->source.end()) {
			return;
		}
		delete this->source[sourceName];
		this->source.erase(sourceName);
	}

	//ソースの獲得。ポインターを返す関数は本当は作りたくないんだがなあ
	SoundSource* SoundClass::GetSource(const char* sourceName) {
		//ソース名の存在チェック
		if (this->source.find(sourceName) == this->source.end()) {
			return nullptr;
		}
		return this->source[sourceName];
	}

}
