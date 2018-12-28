#include"DrawableObject.h"
#include"SceneRenderer.h"

namespace K_Graphics {

	//’l‚Æ‚µ‚Ä•`‰æƒpƒX‚ÌŠÇ—ƒNƒ‰ƒX‚ª•K{‚É‚È‚é
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
		//‘S‚Ä‚Ì•`‰æƒpƒX‚Ö‚Ì“o˜^‚ð‰ðœ
		int count = 0;
		for (auto id : this->passList) {
			this->passManager->RemoveDrawableObject(id.first, id.second);
		}
		this->passList.clear();
	}

	void DrawableObject::AttachDrawPass(int passID) {
		//d•¡ƒ`ƒFƒbƒN
		if (this->passList.find(passID) != this->passList.end()) {
			return;
		}

		//“o˜^
		int objId = this->passManager->JoinDrawableObject(passID, this);
		this->passList[passID] = objId;
	}
}