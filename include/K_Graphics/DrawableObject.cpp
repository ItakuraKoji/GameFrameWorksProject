#include"DrawableObject.h"
#include"SceneRenderer.h"

namespace K_Graphics {

	//�l�Ƃ��ĕ`��p�X�̊Ǘ��N���X���K�{�ɂȂ�
	DrawableObject::DrawableObject(DrawPassManager* passManager) {
		this->passManager = passManager;
	}

	DrawableObject::~DrawableObject(){
		RemoveDrawPassAll();
	}

	void DrawableObject::RemoveDrawPass(int drawPassID) {
		this->passManager->RemoveDrawableObject(drawPassID, this->passList[drawPassID]);
		this->passList.erase(drawPassID);
	}



	void DrawableObject::RemoveDrawPassAll() {
		//�S�Ă̕`��p�X�ւ̓o�^������
		int count = 0;
		for (auto id : this->passList) {
			this->passManager->RemoveDrawableObject(id.first, id.second);
		}
		this->passList.clear();
	}

	void DrawableObject::AttachDrawPass(int passID) {
		//�d���`�F�b�N
		if (this->passList.find(passID) != this->passList.end()) {
			return;
		}

		//�o�^
		int objId = this->passManager->JoinDrawableObject(passID, this);
		this->passList[passID] = objId;
	}
}