#pragma once
#include<fstream>
#include"K_Math\MyMathFanctions.h"
#include"ModelData.h"

namespace K_Loader {
	//マテリアル情報
	struct K3MDMaterial {
		//このマテリアルに割り当てられた頂点インデックス
		std::vector<unsigned int> vertexIndex;
		//マテリアル情報
		std::string materialName;
		std::string texturePath;
		K_Math::Vector4 diffuse;
		K_Math::Vector4 ambient;
		float           ambientPower;
		K_Math::Vector4 specular;
		float           specurarShininess;
		float           specurarPower;
	};

	struct K3MDMat {
		//列ベース4x3行列
		K_Math::Vector3 line[4];
	};

	struct K3MDBone {
		K3MDMat bindMat;
		//アニメ数とフレーム数の配列
		std::vector<std::vector<K3MDMat>> mat;
	};

	//モデルデータ構造体、階層のあるモデルはこれが複数ある
	struct K3MDData {
		std::vector<K_Graphics::Vertex> vertexData;
		std::vector<K3MDMaterial> materialData;
		//ボーン数による配列
		std::vector<K3MDBone> boneData;
	};
	//階層モデルデータ
	struct K3MDHierarchy {
		//読みこまれているであろうモデルの位置
		std::string modelPath;
		std::vector<K3MDData> modelHierarchy;
		//アニメーション情報
		std::unordered_map<std::string, K_Graphics::AnimType> animList;
		std::vector<std::string> animNameList;
	};

	//モデルリソースデータからK3MD形式のバイナリファイルを作ったり、K3MD形式ファイルを読み込んだりできる
	class K3MDLoader {
	public:
		K3MDLoader();
		~K3MDLoader();

		void CreateBinaryFile(K3MDHierarchy* data, const std::string& fileName, const std::string& outputFilePath);
		void CreateBinaryFile(K3MDHierarchy* data, const std::string& outputFilePath);
		
		K_Graphics::ModelResource* LoadModel(K3MDHierarchy* data, K_Graphics::TextureList* list);
		K_Graphics::ModelResource* LoadModel(const char* filepath, K_Graphics::TextureList* list);
		K3MDHierarchy* LoadK3MDHierarchy(const char* filepath);

		K3MDHierarchy* LoadK3MDHierarchyFromMemory(char* binaryData);

	private:
		void CreateVertex(K3MDData* modelData, GLuint VAO, const std::vector<GLuint>& IBOs);
		void CreateMaterial(const std::string& fileRoot, K3MDData* modelData, std::vector<GLuint>& IBOs);
		void CreateBone(K3MDHierarchy* data, K3MDData* modelData);

		void ReadBinary(char* dest, char* src, size_t* position, size_t size);

	private:
		std::string fileRoot;

		K_Graphics::TextureList* textureList;
		K_Graphics::VertexData* bufferData;
		K_Graphics::MaterialData* materialData;
		K_Graphics::BoneData* boneData;
	};
}
