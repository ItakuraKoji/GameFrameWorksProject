#pragma once

#include"K_DLL_DEFINE.h"
#include<AL\al.h>
#include<AL\alc.h>
#include<unordered_map>
#include"SoundSource.h"

namespace K_Audio {

	///@brief OpenAL�����b�s���O�����N���X
	class DLL_DECLSPEC SoundClass {
	public:
		///@brief OpenAL�̏�����
		SoundClass();
		///@brief �T�E���h�\�[�X��S�Ĕj��
		~SoundClass();

		///@brief �T�E���h�\�[�X�쐬
		///@param[in] sourceName �T�E���h�\�[�X�̃��[�U�[��`��
		///@param[in] filePass �T�E���h�̃t�@�C���p�X�iOGG �� WAVE �̂݁j
		///@return ���������true
		bool CreateSource(const char* sourceName, const char* filePass, SoundSource::LoadMode mode);
		///@brief �T�E���h�\�[�X���폜
		///@param[in] �폜����T�E���h�\�[�X�̖��O
		void DeleteSource(const char* sourceName);

		///@param[in] �T�E���h�\�[�X�̖��O
		///@return �T�E���h�\�[�X�ւ̃|�C���^
		SoundSource* GetSource(const char* sourceName);

	private:
		ALCdevice * device;
		ALCcontext* context;

		std::unordered_map<std::string, SoundSource*> source;
	};
}
