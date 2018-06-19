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
		CheckHierarchyRange(arrayIndex);
		CheckMaterialArrayRange(arrayIndex, materialIndex);

		this->materialArray[arrayIndex][materialIndex].texture = texture;
	}

	Material& MaterialData::GetMaterial(int arrayIndex, int materialIndex) {
		CheckHierarchyRange(arrayIndex);
		CheckMaterialArrayRange(arrayIndex, materialIndex);
		return this->materialArray[arrayIndex][materialIndex];
	}

	int MaterialData::GetNumMaterial(int arrayIndex) {
		CheckHierarchyRange(arrayIndex);
		return (int)this->materialArray[arrayIndex].size();
	}
	int MaterialData::GetNumFace(int arrayIndex, int materialIndex) {
		CheckHierarchyRange(arrayIndex);
		CheckMaterialArrayRange(arrayIndex, materialIndex);
		return this->materialArray[arrayIndex][materialIndex].numFace;
	}

	////////
	//private
	////
	void MaterialData::CheckHierarchyRange(int index) {
		if (this->materialArray.size() <= index) {
			throw std::out_of_range("mesh model hirarchy out of range");
		}
	}
	void MaterialData::CheckMaterialArrayRange(int arrayIndex, int materialIndex) {
		if (this->materialArray[arrayIndex].size() <= materialIndex) {
			throw std::out_of_range("mesh model material array out of range");
		}
	}

}
