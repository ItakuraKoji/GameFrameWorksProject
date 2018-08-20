#pragma once

#include"K_DLL_DEFINE.h"
#include"AudioData.h"
#include<vorbis\vorbisfile.h>
#include<string>

namespace K_Audio {

	//Oggの読み込みとデータ保持を担当する
	class DLL_DECLSPEC OggData : public AudioData {
	public:
		struct OggBinaryData {
			//静的メンバに渡すための領域
			OggData* classPtr;
			size_t dataSize;
			size_t binaryPosition;
			//実際のバイナリデータ
			char* oggData;
		};

	public:
		OggData(const char* filePass);
		OggData(char* binaryData, size_t binarySize);
		~OggData();

		void Seek(int offset);
		int Read(char* buffer, int maxSize);

		//メモリ上のOggを読み込むのに使用するコールバック
		static size_t ReadCallBack(void* buffer, size_t size, size_t maxCount, void* stream);
		static int SeekCallBack(void* buffer, ogg_int64_t offset, int flag);
		static int CloseCallBack(void *datasource);
		static long TellCallBack(void *datasource);

	private:
		void CreateOggFile(const char* filePass);
		void CreateOggFile(char* binaryData, size_t binarySize);
		void LoadFile();
		int OggCommentValue(vorbis_comment* comment, const char* key);

	private:
		OggVorbis_File oggFile;
		OggBinaryData binaryData;
	};
}