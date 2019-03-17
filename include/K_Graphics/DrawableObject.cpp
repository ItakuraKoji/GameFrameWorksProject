#include"DrawableObject.h"
#include"SceneRenderer.h"

namespace K_Graphics {

	//�l�Ƃ��ĕ`��p�X�̊Ǘ��N���X���K�{�ɂȂ�ASetDrawPassManager()�ŃZ�b�g���Ȃ��Ă͂Ȃ�Ȃ�
	DrawableObject::DrawableObject() {
		this->passManager = nullptr;
	}

	DrawableObject::~DrawableObject(){
		RemoveDrawPassAll();
	}

	void DrawableObject::RemoveDrawPass(int drawPassID) {
		//��x���o�^����Ă��Ȃ��ƁA�`��p�X�Ǘ��N���X��null�ɂȂ��Ă���
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : RemoveDrawPass()");
		}

		this->passManager->RemoveDrawableObject(drawPassID, this->passList[drawPassID]);
		this->passList.erase(drawPassID);
	}



	void DrawableObject::RemoveDrawPassAll() {
		//��x���o�^����Ă��Ȃ��ƁA�`��p�X�Ǘ��N���X��null�ɂȂ��Ă���
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : RemoveDrawPassAll()");
		}

		//�S�Ă̕`��p�X�ւ̓o�^������
		int count = 0;
		for (auto id : this->passList) {
			this->passManager->RemoveDrawableObject(id.first, id.second);
		}
		this->passList.clear();
	}

	void DrawableObject::AttachDrawPass(int passID) {
		//��x���o�^����Ă��Ȃ��ƁA�`��p�X�Ǘ��N���X��null�ɂȂ��Ă���
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : AttachDrawPass()");
		}

		//�d���`�F�b�N
		if (this->passList.find(passID) != this->passList.end()) {
			return;
		}

		//�o�^
		int objId = this->passManager->JoinDrawableObject(passID, this);
		this->passList[passID] = objId;
	}

	void DrawableObject::SetDrawPassManager(DrawPassManager * passManager){
		//���d�o�^�̋֎~
		if (this->passManager != nullptr) {
			throw new std::runtime_error("already set DrawPassManager: SetDrawPassManager()");
		}
		this->passManager = passManager;
	}

	//�`��O�̏��o�^
	void DrawableObject::SetCurrentDrawPass(DrawPass * currentPass){
		this->currentPass = currentPass;
	}
	DrawPass * DrawableObject::GetCurrentDrawPass()
	{
		return this->currentPass;
	}
}