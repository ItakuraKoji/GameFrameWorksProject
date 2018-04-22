#include"AudioDataFactory.h"

namespace K_Audio {
	////////
	//public
	////
	AudioDataFactory::AudioDataFactory() {

	}
	AudioDataFactory::~AudioDataFactory() {

	}
	AudioData* AudioDataFactory::Create(const char* filePass) {
		AudioData* data;

		switch (CheckType(filePass))
		{
		case AudioType::Wave:
			data = new WavData(filePass);
			break;
		case AudioType::Ogg:
			data = new OggData(filePass);
			break;

		case AudioType::NonSupport:
		default:
			data = nullptr;
		}

		return data;
	}

	////////
	//private
	////

	//拡張子から種類を判別
	AudioDataFactory::AudioType AudioDataFactory::CheckType(const char* filePass) {
		//拡張子は初めの「.」から終端までの文字列とする
		int i = 0;
		char ext[16] = {};
		while (filePass[i] != '.') {
			++i;
		}
		//ドットは飛ばす
		++i;

		//終端までコピー
		int extCount = 0;
		while (filePass[i] != 0 && extCount < 15) {
			ext[extCount] = filePass[i];
			++extCount;
			++i;
		}
		ext[extCount] = 0;

		AudioType type = AudioType::NonSupport;
		if (strcmp(ext, "ogg") == 0) {
			type = AudioType::Ogg;
		}
		if (strcmp(ext, "wav") == 0) {
			type = AudioType::Wave;
		}

		return type;
	}

}

