#pragma once
#include"K3MDLoader.h"
#include"K_Math\MyMathFanctions.h"
#include<fbxsdk\fbxsdk.h>
#include<vector>
#include<unordered_map>
#include<string>
#include"ModelData.h"

namespace K_Loader {

	//FBXファイルから頂点、マテリアル、ボーン情報を読み取って、K3MD形式のモデルへとデコードしてモデルデータを作成
	//ボーンは４つ以下のウェイトで決め打ちとする　(５つ以上では不具合が出る)　のでモデル側で気を付ける
	class FbxModelLoader {
	private:
		//頂点からポリゴンを逆引きするテーブル
		struct PolygonTable {
			//ポリゴン番号
			int polygonIndex[255] = {};
			//ポリゴン内インデックス
			int polygon123[255] = {};
			int numPolygon = 0;
		};

		struct NewVertexData {
			//UV情報
			FbxArray<FbxVector2> uv;
			FbxArray<int> index;
		};

		//頂点がもつUV配列と新たな頂点座標に対応するインデックスをもつテーブル
		typedef FbxMap<int, NewVertexData> VertexUVs;

	public:
		FbxModelLoader();
		~FbxModelLoader();
		bool LoadFBX(const std::string& fileName, K_Graphics::TextureList* list);

		K_Graphics::FbxData*       PassFbxData();
		K3MDHierarchy*             PassModelData();

	private:
		void          Finalize();
		void          InitializeFBX(const std::string& fileName);
		void          RecursiveNode(FbxNode* node);
		void          LoadFbxMesh(FbxMesh* mesh);
		void          LoadVertex(FbxMesh* mesh, K_Graphics::Vertex* vertex);
		void          LoadMaterial(FbxMesh* mesh, VertexUVs& vertexData, std::vector<K3MDMaterial>& material);
		void          LoadBones(FbxMesh* mesh, K_Graphics::Vertex* vertex, std::vector<K3MDBone>& bone, PolygonTable *table);
		void          CalcCurrentBoneMatrix(std::vector<K3MDBone>& bone, std::vector<FbxCluster*>& cluster);

		PolygonTable* CreatePolygonTable(FbxMesh *mesh, int numVertex, int numFace);
		int CreateUVBaseVertex(FbxMesh* mesh, VertexUVs& uvMap);

	private:
		bool loaded;

		K3MDHierarchy* modelData;

		K_Graphics::FbxData       *fbxData;

		//クラス内でアクセスするためのもの。deleteする責任はない
		K_Graphics::TextureList *textureList;

		//FBXファイルの場所をルートにする
		char* fileRoot;

		K_Graphics::Vertex* vertexData;
		int numVertex;
		int numUV;
		int numFace;
	};
}