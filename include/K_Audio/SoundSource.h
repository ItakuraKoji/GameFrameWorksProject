#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<string>
#include<thread>
#include<mutex>
#include<iostream>
#include<vector>
#include<list>


#include"AudioDataFactory.h"


namespace K_Audio {


	///@brief �����N���X\n���[�v�t���O������Ă���Ƃ��̓t�@�C���I�[�ɓ��B�������_�ŃX�g���[�~���O���I����
	class DLL_DECLSPEC SoundSource {
	public:
		///@brief �u�S�ēǂݍ��ށv�u�X�g���[�~���O�Đ��v�̓������
		enum LoadMode {
			Streaming,
			AllRead,
		};

	public:
		///@brief ����������
		///@param[in] sourceName ���̃\�[�X�̃��[�U�[��`��
		///@param[in] filePass �����t�@�C���̃p�X
		///@param[in] mode �ǂݍ��݃��[�h�iPlayCopy()��AllRead���[�h�̎��ɂ̂ݎg�p�\�j
		///@param[in] numBuffer �X�g���[�~���O�Đ����̃o�b�t�@�̐��i�ȗ���32�j
		SoundSource(const char* sourceName, const char* filePass, LoadMode mode, int numBuffer = 32);
		///@brief �����t�@�C���̃|�C���^��delete����
		~SoundSource();

		///@brief �������Đ�
		///@param[in] loop true�̎��ɉ������J��Ԃ��Đ�
		void Play(bool loop);
		///@brief �������R�s�[���čĐ��iLoadMode��AllRead���̂ݎg���A�A���Đ��������d�Ȃ�j
		void PlayCopy();
		///@brief �������ꎞ��~
		void Pause();
		///@brief �������~
		void Stop();

		///@brief �����̃{�����[����ݒ�i0.0f���Œ�ŁA1.0f���ő�j
		///@param[in] volume ���ʂ̒l
		void SetVolume(float volume);
		///@brief �����̈ʒu��ݒ�
		///@param[in] x �ʒu���WX��
		///@param[in] y �ʒu���WY��
		///@param[in] z �ʒu���WZ��
		void SetPosition(float x, float y, float z);
		///@brief �����̈ړ����x��ݒ�i�h�b�v���[���ʂ𓾂���j
		///@param[in] x ���xX����
		///@param[in] y ���xY����
		///@param[in] z ���xZ����
		void SetVelocity(float x, float y, float z);
		///@return �������Đ�����
		bool IsPlay();

	private:
		void StreamingThread();
		void AllReadThread();
		void EndThread();
		void FillBuffer();
		int ReadBuffer(char* buffer, int maxReadSize);
		int OggCommentValue(vorbis_comment* comment, const char* key);

	private:
		const std::string name;
		AudioData* audio;
		LoadMode mode;
		std::vector<char> allReadData;
		//CopyPlay�p�̊Ǘ����X�g
		std::list<ALuint> copySources;

		std::thread* thread;
		std::recursive_mutex _mutex;

		ALuint sourceID;
		ALuint *bufferIDs;
		int numBuffer;

		ALuint format;

		float volume;
		float posX, posY, posZ;
		float velocityX, velocityY, velocityZ;

		bool isPlayed;
		bool isLoop;
		bool isEnd;
	};

}
