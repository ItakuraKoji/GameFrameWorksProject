#pragma once
#include<unordered_map>
#include"SceneRenderer.h"

namespace K_Graphics {
	//�`��I�u�W�F�N�g���w���A3D�̕`����N���X
	//MeshObject�ɑ΂��āA�������͕`��̏ڍ׏�񂪒ǉ�����Ă���
	class DrawableObject {
	public:
		//�R�s�[�֎~
		DrawableObject(const DrawableObject&) = delete;
		void operator = (const DrawableObject&) = delete;

		DrawableObject();
		virtual ~DrawableObject();


		//�Ǝ��̕`�揈�����p���Ŏ��A��ɕ`��p�X�����g��
		virtual void Draw() = 0;

		//�`��p�X�̓o�^��S�ď���
		virtual void RemoveDrawPassAll() final;
		//�w���ID�̕`��p�X����O���
		virtual void RemoveDrawPass(int passID) final;
		//�w�肵���`��p�X�֎�����`�悷��悤�ɓo�^
		virtual void AttachDrawPass(int passID) final;

		virtual void SetDrawPassManager(DrawPassManager* passManager) final;

		//�`��p�X��`��O�ɃZ�b�g���Ă���
		virtual void SetCurrentDrawPass(DrawPass* currentPass) final;
		virtual DrawPass* GetCurrentDrawPass() final;

	private:
		//�֐����Ăяo���O�ɌĂяo�������킩��悤�ɓo�^������
		DrawPass* currentPass;

		//�p�X�ւ̃A�N�Z�X
		DrawPassManager* passManager;
		//�o�^����Ă���`��p�X����^����ꂽID���w���A�o�^�����͎�ɂ���ŃA�N�Z�X����
		std::unordered_map<int, int> passList;
	};
}