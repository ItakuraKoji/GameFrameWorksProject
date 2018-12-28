#include"SoundClass.h"

namespace K_Audio {

	/////////
	//SoundGroup
	////
	SoundClass::SoundGroup::SoundGroup(){
		//ボリュームの初期化
		this->groupVolume = 1.0f;
	}





	////////
	//public
	////

	SoundClass::SoundClass() {
		//初期の状態でもサウンドグループの０番が使える
		this->CreateSoundGroup(1);
		//マスターボリュームは最初は１
		this->masterVolume = 1.0f;

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

	//指定されたサイズ分だけグループの領域を作る
	void SoundClass::CreateSoundGroup(int size) {
		this->soundGroup.resize(size);
	}


	void SoundClass::CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, int group, float baseVolume, int numBuffer) {
		//ソース名の重複は許さない
		if (this->source.find(sourceName) != this->source.end()) {
			throw std::runtime_error("SoundClass createSource error sourceName has already existed : " + std::string(sourceName));
		}
		//存在しないグループも例外
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("SoundClass createSource error group not existed : " + std::string(sourceName));
		}

		SoundSource* audioSource = new SoundSource(this, group, sourceName, filePass, mode, baseVolume, numBuffer);

		//サウンドソース追加
		this->source[sourceName] = audioSource;
		//グループに追加
		this->soundGroup[group].source.push_back(audioSource);
	}

	//ソースの明示的解放(ほっといてもデストラクタですべて解放される)
	void SoundClass::DeleteSource(const char* sourceName) {
		//ソース名の存在チェック
		if (this->source.find(sourceName) == this->source.end()) {
			throw std::runtime_error("SoundClass deleteSource error sourceName is not exist : " + std::string(sourceName));
		}
		//まずはグループから外す
		this->soundGroup.erase(this->soundGroup.begin() + this->source[sourceName]->GetSoundGroup());
		//ソースを削除
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

	//ボリューム取得
	float SoundClass::GetMasterVolume() {
		return this->masterVolume;
	}
	float SoundClass::GetGroupVolume(int group) {
		//存在しないグループは例外
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("GetGroupVolume() error group not existed : " + std::to_string(group));
		}

		return this->soundGroup[group].groupVolume;
	}

	//グループを全部止める
	void SoundClass::StopSoundGroup(int group) {
		//存在しないグループは例外
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("StopSoundGroup() error group not existed : " + std::to_string(group));
		}

		for (auto soundList : this->soundGroup) {
			//グループに属するすべてにStopを行う
			for (auto sound : soundList.source) {
				sound->StopAll();
			}
		}
	}
	//全部止める
	void SoundClass::StopSoundAll() {
		for (auto sound : this->source) {
			sound.second->StopAll();
		}
	}

	//リスナーの移動
	void SoundClass::SetListnerPosition(float x, float y, float z) {
		alListener3f(AL_POSITION, x, y, z);
	}
	void SoundClass::SetListnerVelocity(float x, float y, float z) {
		alListener3f(AL_VELOCITY, x, y, z);
	}

	//ボリューム設定
	void SoundClass::SetMasterVolume(float volume) {
		this->masterVolume = volume;
	}
	void SoundClass::SetGroupVolume(int group, float volume) {
		//存在しないグループは例外
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("GetGroupVolume() error group not existed : " + std::to_string(group));
		}

		this->soundGroup[group].groupVolume = volume;
	}

}
