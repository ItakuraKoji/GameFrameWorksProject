#pragma once

#include"K_DLL_DEFINE.h"
#include"AudioData.h"
#include<vorbis\vorbisfile.h>
#include<string>

namespace K_Audio {

	//Ogg�̓ǂݍ��݂ƃf�[�^�ێ���S������
	class DLL_DECLSPEC OggData : public AudioData {
	public:
		struct OggBinaryData {
			//�ÓI�����o�ɓn�����߂̗̈�
			OggData* classPtr;
			size_t dataSize;
			size_t binaryPosition;
			//���ۂ̃o�C�i���f�[�^
			char* oggData;
		};

	public:
		OggData(const char* filePass);
		OggData(char* binaryData, size_t binarySize);
		~OggData();

		void Seek(int offset);
		int Read(char* buffer, int maxSize);

		//���������Ogg��ǂݍ��ނ̂Ɏg�p����R�[���o�b�N
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