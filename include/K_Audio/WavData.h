#pragma once

#include"K_DLL_DEFINE.h"
#include<fstream>
#include"AudioData.h"

namespace K_Audio {

	//Waveの読み込みとデータ保持を担当する
	class DLL_DECLSPEC WavData : public AudioData {
	public:


	public:
		WavData(const char* filePass);
		~WavData();

		void Seek(int offset) override;
		int Read(char* buffer, int maxSize) override;


	private:
		void LoadFile(const char* filePass);
		void PcmSeek(int pcmOffset);

	private:
		std::ifstream waveFile;
		int dataStartOffset;
	};

}