#pragma once

#include"K_DLL_DEFINE.h"
#include<fstream>
#include"AudioData.h"

namespace K_Audio {

	//Wave‚Ì“Ç‚İ‚İ‚Æƒf[ƒ^•Û‚ğ’S“–‚·‚é
	class DLL_DECLSPEC WavData : public AudioData {
	public:
		WavData(const char* filePath);
		WavData(char* binaryData, size_t binarySize);
		~WavData();

		void Seek(int offset) override;
		int Read(char* buffer, int maxSize) override;


	private:
		void LoadFile(const char* filePath);
		void LoadFile(char* binaryData, size_t binarySize);
		void PcmSeek(int pcmOffset);
		void ReadBinary(char* dest, char* src, size_t* binaryPosition, size_t readSize);

	private:
		int dataStartOffset;

		char* wavData;
		size_t binaryPosition;
		size_t binarySize;

	};

}