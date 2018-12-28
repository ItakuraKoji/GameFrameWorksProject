#include"DrawableObject.h"
#include"SceneRenderer.h"

namespace K_Graphics {

	//値として描画パスの管理クラスが必須になる
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
		//全ての描画パスへの登録を解除
		int count = 0;
		for (auto id : this->passList) {
			this->passManager->RemoveDrawableObject(id.first, id.second);
		}
		this->passList.clear();
	}

	void DrawableObject::AttachDrawPass(int passID) {
		//重複チェック
		if (this->passList.find(passID) != this->passList.end()) {
			return;
		}

		//登録
		int objId = this->passManager->JoinDrawableObject(passID, this);
		this->passList[passID] = objId;
	}
}