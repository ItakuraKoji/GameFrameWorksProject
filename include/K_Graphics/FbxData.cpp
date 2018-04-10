#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	FbxData::FbxData() {
		this->manager = nullptr;
		this->importer = nullptr;
		this->scene = nullptr;
	}
	FbxData::~FbxData() {
		if (this->manager != nullptr) {
			this->manager->Destroy();
		}
	}

	void FbxData::Add(FbxManager* manager, FbxImporter *importer, FbxScene *scene) {
		this->manager = manager;
		this->importer = importer;
		this->scene = scene;
	}

	FbxScene* FbxData::GetScene() {
		return this->scene;
	}

}