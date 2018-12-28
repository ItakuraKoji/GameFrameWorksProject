#pragma once

#include<unordered_map>
#include<string>
#include"FrameBuffer.h"
#include"TextureList.h"

namespace K_Graphics {

	//�t���[���o�b�t�@�������Ǘ��N���X�A�e�N�X�`���������̂�TextureList�Ȃǂ̃e�N�X�`���N���X�Ƃ̘A�g�K�{
	class FrameBufferList {
	public:
		//!@brief �e�N�X�`�����X�g�ւ̃|�C���^��n���ď�����
		FrameBufferList(TextureList* list);
		//!@brief �I������������
		~FrameBufferList();


		//!@brief�e�N�X�`�����X�g�ɐV�K�쐬���˗����A�t���[���o�b�t�@�쐬�����X�g�o�^
		//!�[�x�o�b�t�@���g���܂킷�Ƃ��͂��̃t���[���o�b�t�@�̖��O�������ɓn��
		//!@param[in] name �쐬����t���[���o�b�t�@�̃��[�U�[��`��
		//!@param[in] width �쐬����t���[���o�b�t�@�̃r���[�|�[�g��
		//!@param[in] height �쐬����t���[���o�b�t�@�̃r���[�|�[�g����
		bool CreateFrameBuffer(const std::string& name, int width, int height);
		bool CreateFrameBuffer(const std::string& name, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor);

		//!@brief�e�N�X�`�����X�g�ɐV�K�쐬���˗����A�t���[���o�b�t�@�쐬�����X�g�o�^
		//!�[�x�o�b�t�@���g���܂킷�Ƃ��͂��̃t���[���o�b�t�@�̖��O�������ɓn��
		//!@param[in] name �쐬����t���[���o�b�t�@�̃��[�U�[��`��
		//!@param[in] depthBuffer �[�x���g���܂킷�t���[���o�b�t�@�̖��O
		//!@param[in] width �쐬����t���[���o�b�t�@�̃r���[�|�[�g��
		//!@param[in] height �쐬����t���[���o�b�t�@�̃r���[�|�[�g����
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height);
		bool CreateFrameBuffer(const std::string& name, const std::string& depthBufferTextureName, int width, int height, TextureType dataType, TextureColorType color, TextureColorType dataColor);
		
		//!@brief �w��F�Ńt���[���o�b�t�@���N���A����
		//!@param[in] name �N���A����t���[���o�b�t�@�̖��O
		//!@param[in] r �N���A�F��R����
		//!@param[in] g �N���A�F��G����
		//!@param[in] b �N���A�F��B����
		//!@param[in] notDeleteDepthStencil true�ɂ���Ɛ[�x�ƃX�e���V�����N���A���Ȃ��i�ȗ���false�j
		void BeginDraw(const std::string& name, float r, float g, float b, float a);
		
		//!@brief �r���[�|�[�g��ݒ肵�A�w��F�Ńo�b�N�o�b�t�@���N���A����
		//!@param[in] viewPortWidth �r���[�|�[�g�̕�
		//!@param[in] viewPortHeight �r���[�|�[�g�̍���
		//!@param[in] r �N���A�F��R����
		//!@param[in] g �N���A�F��G����
		//!@param[in] b �N���A�F��B����
		//!@param[in] notDeleteDepthStencil true�ɂ���Ɛ[�x�ƃX�e���V�����N���A���Ȃ��i�ȗ���false�j
		void BeginDraw(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a);
		//!@brief �o�C���h�������ĕ`��I��
		void EndDraw();

		//�o�b�t�@�����̃t���O�ݒ�
		void SetClearFlag(bool color, bool depth, bool stencil);

		Framebuffer* GetFrameBuffer(const std::string& name);

	private:
		void ClearBuffer(int viewPortWidth, int viewPortHeight, float r, float g, float b, float a);

	private:
		std::unordered_map<std::string, Framebuffer*> frameBuffers;

		//�Q�Ɨp�A�f�X�g���N�^�ȂǂŎg��
		TextureList* list;

		//
		bool colorClear;
		bool depthClear;
		bool stencilClear;
	};
}
