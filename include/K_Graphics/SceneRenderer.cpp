#include"K_Graphics/SceneRenderer.h"
#include"K_Graphics/DrawableObject.h"

namespace K_Graphics {

	DrawPass::DrawPass(const DrawData& drawData){
		this->drawData = drawData;
	}

	DrawPass::~DrawPass() {
		//����������͓��ɃI�u�W�F�N�g�ɂ͂Ȃɂ����Ȃ��i�o�^�����Ȃǂ̐ӔC�͎����Ȃ��j
	}

	void DrawPass::Draw() {
		ShaderSettings();
		DrawObject();
		EndDraw();
	}

	void DrawPass::DrawObject() {
		//���X�g����Ȃ牽�����Ȃ�
		if (this->drawObjects.GetSize() == 0) {
			return;
		}

		//�S�ẴI�u�W�F�N�g��`��
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
		//�d���`�F�b�N
		if (std::find(this->drawPass.begin(), this->drawPass.end(), pass) != this->drawPass.end()) {
			return;
		}

		this->drawPass.push_back(pass);
	}

	void DrawPassManager::DeleteAllPass() {
		//�S�Ă̕`��p�X�̓o�^�������A�p�X��delete����
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
		//�L����ID���ǂ������`�F�b�N
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

		//�폜����`��p�X�ɓo�^���ꂽ�I�u�W�F�N�g�ɕύX�𔽉f
		auto& list = pass->GetDrawList();

		//���X�g����Ȃ牽�����Ȃ�
		if (list.GetSize() == 0) {
			return;
		}

		for (int i = list.GetFirstIndex(); i != -1; i = list.GetNextIndex(i)) {
			list.Get(i)->RemoveDrawPass(passID);
		}
	}


}