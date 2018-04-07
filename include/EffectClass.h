#pragma once

#define __EFFEKSEER_RENDERER_GL_GLEW__

#include<Effekseer\Effekseer.h>
#include<Effekseer\EffekseerRendererGL.h>
#include<Effekseer\EffekseerSoundAL.h>
#include<unordered_map>
#include<string>
#include<gl\GLU.h>

#include"CameraClass.h"

namespace K_Graphics {

	typedef Effekseer::Handle EffectHandle;

	class EffectClass {
	public:
		EffectClass();
		~EffectClass();

		void SetMatrix(CameraClass* viewCamera);
		//�G�t�F�N�g�S�̂̍X�V
		void Run();
		//�G�t�F�N�g�͂��̊֐��ł܂Ƃ߂ĕ`�悳���
		void Draw();

		//�G�t�F�N�g�ǂݍ���
		void AddEffectSource(const std::string& effectName, const char* filePass);
		void DeleteEffectSource(const std::string& effectName);

		//�Đ�
		//�G�t�F�N�g���A�ʒu��n��
		//�G�t�F�N�g�𑖍�����n���h���ԍ����Ԃ�A�ړ��Ȃǂ̑�������Ȃ��Ȃ�ϐ��Ŏ���Ă����Ȃ��Ă�����
		EffectHandle Play(const std::string& effectName, float posX, float posY, float posZ);
		//�����I�ɒ�~�A������͌Ă񂾏u�Ԃɂ���������
		void Stop(EffectHandle handle);
		//�����I�ɒ�~�A�������G�t�F�N�g�̐�������߂邾���Ȃ̂ł��΂炭�c��
		void StopRoot(EffectHandle handle);



	private:
		Effekseer::Manager* manager = nullptr;
		EffekseerRenderer::Renderer* renderer = nullptr;
		EffekseerSound::Sound* sound = nullptr;
		std::unordered_map<std::string, Effekseer::Effect*> effect;
		CameraClass* viewCamera;
	};
}