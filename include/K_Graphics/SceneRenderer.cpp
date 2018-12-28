#include"K_Graphics/SceneRenderer.h"
#include"K_Graphics/DrawableObject.h"

namespace K_Graphics {

	DrawPass::DrawPass(const DrawData& drawData){
		this->drawData = drawData;
	}

	DrawPass::~DrawPass() {
		//こっちからは特にオブジェクトにはなにもしない（登録解除などの責任は持たない）
	}

	void DrawPass::Draw() {
		ShaderSettings();
		DrawObject();
		EndDraw();
	}

	void DrawPass::DrawObject() {
		//リストが空なら何もしない
		if (this->drawObjects.GetSize() == 0) {
			return;
		}

		//全てのオブジェクトを描画
		for (int i = this->drawObjects.GetFirstIndex(); i >= 0; i = this->drawObjects.GetNextIndex(i)) {
			this->drawObjects.Get(i)->Draw(this);
		}
	}

	IndexVector<DrawableObject>& DrawPass::GetDrawList(){
		return this->drawObjects;
	}

	const DrawPass::DrawData& DrawPass::GetDrawData()
	{
		return this->drawData;
	}

	int DrawPass::JoinDrawableObject(DrawableObject* obj) {
		return this->drawObjects.EntryValue(obj);
	}

	void DrawPass::RemoveDrawableObject(int id) {
		this->drawObjects.PopValue(id);
	}

	DrawPassManager::DrawPassManager() {
	}

	DrawPassManager::~DrawPassManager() {
		DeleteAllPass();
	}

	void DrawPassManager::Draw(){
		for (auto pass : this->drawPass) {
			pass->Draw();
		}
	}

	void DrawPassManager::AddPass(DrawPass* pass){
		//重複チェック
		if (std::find(this->drawPass.begin(), this->drawPass.end(), pass) != this->drawPass.end()) {
			return;
		}

		this->drawPass.push_back(pass);
	}

	void DrawPassManager::DeleteAllPass() {
		//全ての描画パスの登録を解除、パスはdeleteする
		int count = 0;
		for (auto pass : this->drawPass) {
			FinalizeList(pass, count);
			++count;
			delete pass;
		}
		this->drawPass.clear();
	}

	bool DrawPassManager::IsEnablePass(int drawPassID)
	{
		//有効なIDかどうかをチェック
		if (drawPassID >= this->drawPass.size()) {
			return false;
		}
		if (drawPassID < 0){
			return false;
		}

		return true;
	}

	int DrawPassManager::JoinDrawableObject(int drawPassID, DrawableObject* obj){
		return this->drawPass.at(drawPassID)->JoinDrawableObject(obj);
	}

	void DrawPassManager::RemoveDrawableObject(int drawPassID, int objectHandle) {
		this->drawPass.at(drawPassID)->RemoveDrawableObject(objectHandle);
	}

	void DrawPassManager::FinalizeList(DrawPass* pass, int passID) {

		//削除する描画パスに登録されたオブジェクトに変更を反映
		auto& list = pass->GetDrawList();

		//リストが空なら何もしない
		if (list.GetSize() == 0) {
			return;
		}

		for (int i = list.GetFirstIndex(); i != -1; i = list.GetNextIndex(i)) {
			list.Get(i)->RemoveDrawPass(passID);
		}
	}


}