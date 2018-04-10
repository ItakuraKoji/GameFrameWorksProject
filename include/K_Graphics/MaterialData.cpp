#include"ModelData.h"

namespace K_Graphics {

	////////
	//public
	////
	MaterialData::MaterialData() {

	}
	MaterialData::~MaterialData() {
	}

	void MaterialData::Add(std::vector<Material> material) {
		this->materialArray.push_back(material);
	}

	void MaterialData::SetTexture(Texture* texture, int arrayIndex, int materialIndex) {
		if (this->materialArray.size() < arrayIndex) {
			return;
		}
		if (this->materialArray[arrayIndex].size() < materialIndex) {
			return;
		}
		this->materialArray[arrayIndex][materialIndex].texture = texture;
	}

	Material& MaterialData::GetMaterial(int arrayIndex, int materialIndex) {
		return this->materialArray[arrayIndex][materialIndex];
	}

	int MaterialData::GetNumMaterial(int arrayIndex) {
		return (int)this->materialArray[arrayIndex].size();
	}
	int MaterialData::GetNumFace(int arrayIndex, int materialIndex) {
		return this->materialArray[arrayIndex][materialIndex].numFace;
	}

}
