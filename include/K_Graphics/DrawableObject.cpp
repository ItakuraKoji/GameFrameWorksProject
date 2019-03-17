#include"DrawableObject.h"
#include"SceneRenderer.h"

namespace K_Graphics {

	//’l‚Æ‚µ‚Ä•`‰æƒpƒX‚ÌŠÇ—ƒNƒ‰ƒX‚ª•K{‚É‚È‚éASetDrawPassManager()‚ÅƒZƒbƒg‚µ‚È‚­‚Ä‚Í‚È‚ç‚È‚¢
	DrawableObject::DrawableObject() {
		this->passManager = nullptr;
	}

	DrawableObject::~DrawableObject(){
		RemoveDrawPassAll();
	}

	void DrawableObject::RemoveDrawPass(int drawPassID) {
		//ˆê“x‚à“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚ÆA•`‰æƒpƒXŠÇ—ƒNƒ‰ƒX‚ªnull‚É‚È‚Á‚Ä‚¢‚é
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : RemoveDrawPass()");
		}

		this->passManager->RemoveDrawableObject(drawPassID, this->passList[drawPassID]);
		this->passList.erase(drawPassID);
	}



	void DrawableObject::RemoveDrawPassAll() {
		//ˆê“x‚à“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚ÆA•`‰æƒpƒXŠÇ—ƒNƒ‰ƒX‚ªnull‚É‚È‚Á‚Ä‚¢‚é
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : RemoveDrawPassAll()");
		}

		//‘S‚Ä‚Ì•`‰æƒpƒX‚Ö‚Ì“o˜^‚ğ‰ğœ
		int count = 0;
		for (auto id : this->passList) {
			this->passManager->RemoveDrawableObject(id.first, id.second);
		}
		this->passList.clear();
	}

	void DrawableObject::AttachDrawPass(int passID) {
		//ˆê“x‚à“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚ÆA•`‰æƒpƒXŠÇ—ƒNƒ‰ƒX‚ªnull‚É‚È‚Á‚Ä‚¢‚é
		if (this->passManager == nullptr) {
			throw new std::runtime_error("nobody attach DrawPass : AttachDrawPass()");
		}

		//d•¡ƒ`ƒFƒbƒN
		if (this->passList.find(passID) != this->passList.end()) {
			return;
		}

		//“o˜^
		int objId = this->passManager->JoinDrawableObject(passID, this);
		this->passList[passID] = objId;
	}

	void DrawableObject::SetDrawPassManager(DrawPassManager * passManager){
		//‘½d“o˜^‚Ì‹Ö~
		if (this->passManager != nullptr) {
			throw new std::runtime_error("already set DrawPassManager: SetDrawPassManager()");
		}
		this->passManager = passManager;
	}

	//•`‰æ‘O‚Ìî•ñ“o˜^
	void DrawableObject::SetCurrentDrawPass(DrawPass * currentPass){
		this->currentPass = currentPass;
	}
	DrawPass * DrawableObject::GetCurrentDrawPass()
	{
		return this->currentPass;
	}
}