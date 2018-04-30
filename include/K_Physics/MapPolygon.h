#pragma once

#include<fbxsdk\fbxsdk.h>

#include<vector>

#include"K_Math\MyMathFanctions.h"
#include"BulletPhysics.h"

namespace K_Physics {

	//!@brief 判定用モデルのポリゴン情報を持つクラス、描画は一切できない。行列による回転にも対応していない
	class MapPolygon {
	public:
		//struct PolygonType{
		//	Eigen::Vector3f point[3];
		//};
		//struct PolygonData {
		//	int numPolygon;
		//	PolygonType* polygon;
		//};

		//bullet用
		struct PolygonType {
			K_Math::Vector3 point[3];
		};
		struct PolygonData {
			int numPolygon;
			PolygonType* polygon;
		};


	public:
		MapPolygon();
		~MapPolygon();
		bool Initialize();
		void Finalize();
		//!@brief FBX読み込み
		//!@param[in] filename ファイルのパス
		//!@return 成功するとtrue
		bool LoadModel(const char *filename);
		//!@brief 読み込んだデータをもとに地形コリジョンを作成
		//!@param[in] physics コリジョンを管理する物理クラスへのポインタ
		void setCollisionWorld(BulletPhysics *physics, int mask);
		//!@return 読み込んだモデルのポリゴン数
		int GetNumFace();

	private:
		bool InitializeFBX(const char* filename);
		bool LoadFBXNodeRecursive(FbxNode *node);
		bool LoadFBX(FbxMesh *mesh);

	private:
		//判定用の頂点配列
		std::vector<PolygonData> m_polygonStack;

		int m_numFace;
		FbxManager *mfbx_manager;
		FbxScene *mfbx_scene;

		btTriangleMesh* collisionMesh;
	};
}
