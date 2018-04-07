#pragma once

#include<fbxsdk\fbxsdk.h>
#include<Eigen\Core>
#include<bullet\btBulletDynamicsCommon.h>

#include<vector>

#include"MyMathFanctions.h"
#include"BulletPhysics.h"

namespace K_Physics {

	//判定用モデルのポリゴン情報を持つクラス、描画は一切できない。行列による回転にも対応していない
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
			btVector3 point[3];
		};
		struct PolygonData {
			int numPolygon;
			PolygonType* polygon;
		};


	public:
		MapPolygon();
		~MapPolygon();
		bool Initialize();
		bool LoadModel(const char *filename);
		void Finalize();
		int GetNumFace();

		void setCollisionWorld(BulletPhysics *physics);


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
