#pragma once
#include"K_DLL_DEFINE.h"

namespace K_Audio {

	//音声ファイル読み込みの基底クラス
	class DLL_DECLSPEC AudioData {
	public:
		enum SoundFormat {
			Mono8,
			Mono16,
			Stereo8,
			Stereo16,
		};
	public:
		AudioData();
		virtual ~AudioData();

		virtual bool LoadFile(const char* filePass) = 0;
		virtual int Read(char* buffer, int maxSize) = 0;
		virtual void Seek(int pcmOffset) = 0;

		virtual int GetPcmOffset();
		virtual int GetLoopStart();
		virtual int GetLoopLength();
		virtual int GetPcmSize();
		virtual int GetBlockSize();
		virtual SoundFormat GetFormat();
		virtual int GetSamplingRate();

	protected:
		int pcmSize;
		int pcmOffset;
		int loopStart;
		int loopLength;
		SoundFormat format;
		int samplingRate;
		int blockSize;
	};

}
