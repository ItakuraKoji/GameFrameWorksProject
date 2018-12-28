#include"SoundClass.h"

namespace K_Audio {

	/////////
	//SoundGroup
	////
	SoundClass::SoundGroup::SoundGroup(){
		//�{�����[���̏�����
		this->groupVolume = 1.0f;
	}





	////////
	//public
	////

	SoundClass::SoundClass() {
		//�����̏�Ԃł��T�E���h�O���[�v�̂O�Ԃ��g����
		this->CreateSoundGroup(1);
		//�}�X�^�[�{�����[���͍ŏ��͂P
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

	//�w�肳�ꂽ�T�C�Y�������O���[�v�̗̈�����
	void SoundClass::CreateSoundGroup(int size) {
		this->soundGroup.resize(size);
	}


	void SoundClass::CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode, int group, float baseVolume, int numBuffer) {
		//�\�[�X���̏d���͋����Ȃ�
		if (this->source.find(sourceName) != this->source.end()) {
			throw std::runtime_error("SoundClass createSource error sourceName has already existed : " + std::string(sourceName));
		}
		//���݂��Ȃ��O���[�v����O
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("SoundClass createSource error group not existed : " + std::string(sourceName));
		}

		SoundSource* audioSource = new SoundSource(this, group, sourceName, filePass, mode, baseVolume, numBuffer);

		//�T�E���h�\�[�X�ǉ�
		this->source[sourceName] = audioSource;
		//�O���[�v�ɒǉ�
		this->soundGroup[group].source.push_back(audioSource);
	}

	//�\�[�X�̖����I���(�ق��Ƃ��Ă��f�X�g���N�^�ł��ׂĉ�������)
	void SoundClass::DeleteSource(const char* sourceName) {
		//�\�[�X���̑��݃`�F�b�N
		if (this->source.find(sourceName) == this->source.end()) {
			throw std::runtime_error("SoundClass deleteSource error sourceName is not exist : " + std::string(sourceName));
		}
		//�܂��̓O���[�v����O��
		this->soundGroup.erase(this->soundGroup.begin() + this->source[sourceName]->GetSoundGroup());
		//�\�[�X���폜
		delete this->source[sourceName];
		this->source.erase(sourceName);
	}

	//�\�[�X�̊l��
	SoundSource* SoundClass::GetSource(const char* sourceName) {
		//�\�[�X���̑��݃`�F�b�N
		if (this->source.find(sourceName) == this->source.end()) {
			throw std::runtime_error("SoundClass deleteSource error sourceName is not exist : " + std::string(sourceName));
		}
		return this->source[sourceName];
	}

	//�{�����[���擾
	float SoundClass::GetMasterVolume() {
		return this->masterVolume;
	}
	float SoundClass::GetGroupVolume(int group) {
		//���݂��Ȃ��O���[�v�͗�O
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("GetGroupVolume() error group not existed : " + std::to_string(group));
		}

		return this->soundGroup[group].groupVolume;
	}

	//�O���[�v��S���~�߂�
	void SoundClass::StopSoundGroup(int group) {
		//���݂��Ȃ��O���[�v�͗�O
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("StopSoundGroup() error group not existed : " + std::to_string(group));
		}

		for (auto soundList : this->soundGroup) {
			//�O���[�v�ɑ����邷�ׂĂ�Stop���s��
			for (auto sound : soundList.source) {
				sound->StopAll();
			}
		}
	}
	//�S���~�߂�
	void SoundClass::StopSoundAll() {
		for (auto sound : this->source) {
			sound.second->StopAll();
		}
	}

	//���X�i�[�̈ړ�
	void SoundClass::SetListnerPosition(float x, float y, float z) {
		alListener3f(AL_POSITION, x, y, z);
	}
	void SoundClass::SetListnerVelocity(float x, float y, float z) {
		alListener3f(AL_VELOCITY, x, y, z);
	}

	//�{�����[���ݒ�
	void SoundClass::SetMasterVolume(float volume) {
		this->masterVolume = volume;
	}
	void SoundClass::SetGroupVolume(int group, float volume) {
		//���݂��Ȃ��O���[�v�͗�O
		if (group >= this->soundGroup.size() || group < 0) {
			throw std::runtime_error("GetGroupVolume() error group not existed : " + std::to_string(group));
		}

		this->soundGroup[group].groupVolume = volume;
	}

}
