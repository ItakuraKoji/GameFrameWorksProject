#pragma once
#include<fbxsdk\fbxsdk.h>
#include<vector>
#include<unordered_map>
#include<string>
#include"K_Math\MyMathFanctions.h"
#include"ModelData.h"

namespace K_Loader {

	//FBXファイルから頂点、マテリアル、ボーン情報を読み取って保持し、ほかのクラスに渡す
	//読み込みができなかったデータはdeleteすることによって、無い物として扱える（NULL）
	//このクラスでは解放責任を負わない
	//ボーンは４つ以下のウェイトで決め打ちとする　(５つ以上では不具合が出る)　のでモデル側で気を付ける
	class FbxModelLoader {
	private:
		struct Vector4i {
			Vector4i() : data{ 0 } {}
			int data[4];
		};
		struct Vertex {
			K_Math::Vector3 position = K_Math::Vector3::Zero();
			K_Math::Vector2 texcoord = K_Math::Vector2::Zero();
			K_Math::Vector3 normal = K_Math::Vector3::Zero();
			K_Math::Vector4 boneWeight = K_Math::Vector4::Zero();
			Vector4i boneIndex;
		};

		//頂点からポリゴンを逆引きするテーブル
		struct PolygonTable {
			int polygonIndex[255] = {};
			int polygon123[255] = {};
			int numPolygon = 0;
		};


	public:
		FbxModelLoader();
		~FbxModelLoader();
		bool LoadFBX(const std::string& fileName, K_Graphics::TextureList* list);

		K_Graphics::FbxData*       PassFbxData();
		K_Graphics::VertexData*    PassVertexBuffer();
		K_Graphics::MaterialData*  PassMaterialData();
		K_Graphics::AnimationData* PassAnimationData();
		K_Graphics::BoneData*      PassBoneData();


	private:
		void          Finalize();
		bool          InitializeFBX(const std::string& fileName);
		bool          RecursiveNode(FbxNode* node);
		bool          LoadFbxMesh(FbxMesh* mesh);
		void          LoadVertex(FbxMesh* mesh, Vertex* vertex);
		void          LoadMaterial(FbxMesh* mesh, std::vector<K_Graphics::Material>& material, std::vector<GLuint>& IBOs);
		bool          LoadBones(FbxMesh* mesh, Vertex* vertex, PolygonTable *table);

	private:
		bool loaded;

		K_Graphics::FbxData       *fbxData;
		K_Graphics::VertexData    *bufferData;
		K_Graphics::MaterialData  *materialData;
		K_Graphics::AnimationData *animationData;
		K_Graphics::BoneData      *boneData;

		//クラス内でアクセスするためのもの。deleteする責任はない
		K_Graphics::TextureList *textureList;

		//FBXファイルの場所をルートにする
		char fileRoot[100];
		int numVertex;
		int numUV;
		int numFace;
	};
}