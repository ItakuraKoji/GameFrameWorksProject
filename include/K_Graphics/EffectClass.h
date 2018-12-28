#pragma once

#define __EFFEKSEER_RENDERER_GL_GLEW__

#include<Effekseer\Effekseer.h>
#include<Effekseer\EffekseerRendererGL.h>
#include<unordered_map>
#include<string>
#include<gl\GLU.h>

#include"CameraClass.h"

namespace K_Graphics {

	typedef Effekseer::Handle EffectHandle;

	//!@brief �G�t�F�N�g�Ǘ��N���X\n�ق�Effekseer�̋@�\�����b�s���O���������ɂƂǂ܂��Ă���
	class EffectClass {
	public:
		//!@brief Effekseer��������
		EffectClass(int numMaxEffect, int numMaxRender);
		//!@brief Effekseer�̏I������
		~EffectClass();

		//!@brief �G�t�F�N�g�`��Ɏg���J������ݒ�
		//!@param[in] �J�����̃|�C���^
		void SetMatrix(CameraClass* viewCamera);
		//!@brief �G�t�F�N�g�S�̂̍X�V
		void Run(float timeSpeed = 1.0f);
		//!@brief �G�t�F�N�g�͂��̊֐��ł܂Ƃ߂ĕ`�悳���
		void Draw();

		//!@brief �G�t�F�N�g�̓ǂݍ���
		//!@param[in] effectName �G�t�F�N�g�̃��[�U�[��`���i�������O�͓o�^�ł��Ȃ��j
		//!@param[in] filePath Effekseer�Ő��삵���G�t�F�N�g�ւ̃t�@�C���p�X
		bool AddEffectSource(const std::string& effectName, const char* filePath);
		//!@brief �G�t�F�N�g�̏���
		//!@param[in] effectName �G�t�F�N�g��
		void DeleteEffectSource(const std::string& effectName);

		//!@brief �G�t�F�N�g���w��ʒu�ōĐ�
		//!@param[in] effectName �G�t�F�N�g��
		//!@param[in] position �������ʒu
		//!@return �G�t�F�N�g�𑖍�����n���h���ԍ��A�ړ��Ȃǂ̑�������Ȃ��Ȃ�ϐ��Ŏ���Ă����Ȃ��Ă�����
		EffectHandle Play(const std::string& effectName, const K_Math::Vector3& position, float speed = 1.0f);
		//!@brief �G�t�F�N�g�𖾎��I�ɒ�~�A������͌Ă񂾏u�Ԃɂ���������
		//!@param[in] handle Play()�ŋA���Ă���G�t�F�N�g�ԍ�
		void Stop(EffectHandle handle);
		//!@brief �G�t�F�N�g�𖾎��I�ɒ�~�A�������G�t�F�N�g�̐�������߂邾���Ȃ̂ł��΂炭�c��
		//!@param[in] handle Play()�ŋA���Ă���G�t�F�N�g�ԍ�
		void StopRoot(EffectHandle handle);
		//!@brief �G�t�F�N�g���ړ�
		//!@param[in] position �������ʒu
		void SetPosition(EffectHandle handle, const K_Math::Vector3& position);
		//!@brief �G�t�F�N�g����]
		//!@param[in] rotation ��]�p�x
		//!@param[in] axis ��]�̎��x�N�g��
		void SetRotation(EffectHandle handle, float rotation, const K_Math::Vector3& axis);
		//!@brief �G�t�F�N�g���X�P�[�����O
		//!@param[in] scale �X�P�[��
		void SetScale(EffectHandle handle, const K_Math::Vector3& scale);
		//!@brief �G�t�F�N�g�̍Đ����x
		//!@param[in] speed �Đ����x�i1.0f�������j
		void SetSpeed(EffectHandle handle, float speed);

		//!@brief �G�t�F�N�g�̌����擾�i�f�o�b�O�p�j
		int GetNumEffectinstance();

	private:
		Effekseer::Manager* manager = nullptr;
		EffekseerRenderer::Renderer* renderer = nullptr;
		//EffekseerSound::Sound* sound = nullptr;
		std::unordered_map<std::string, Effekseer::Effect*> effect;
		CameraClass* viewCamera;
	};
}
