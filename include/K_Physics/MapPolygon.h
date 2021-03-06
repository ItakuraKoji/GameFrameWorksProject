#pragma once

#include"K_Math\MyMathFanctions.h"
#include<fbxsdk\fbxsdk.h>

#include<vector>

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
		//!@brief FBXを読み込みデータをもとに地形コリジョンを作成
		//!@param[in] filename ファイルのパス
		//!@param[in] physics コリジョンを管理する物理クラスへのポインタ
		//!@param[in] myselfMask 自分が使う衝突マスク、詳しくはサンプルコード３参照
		//!@param[in] giveMask 相手に使わせる衝突マスク、詳しくはサンプルコード３参照
		MapPolygon(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask);
		~MapPolygon();
		bool Initialize(const char* fbxFilePath, BulletPhysics *physics, int myselfMask, int giveMask);
		void Finalize();

		//!@brief コリジョンを拡大
		//!@param[in] scale 拡縮の倍率
		void SetScaling(const K_Math::Vector3& scale);
		//!@return 読み込んだモデルのポリゴン数
		int GetNumFace();
		//!@return 剛体オブジェクトを取得
		K_Physics::RigidBodyData* GetRigidBody();

	private:
		//!@brief FBX読み込み
		//!@param[in] filename ファイルのパス
		//!@return 成功するとtrue
		bool LoadModel(const char *filename);

		bool LoadFBX(FbxMesh *mesh);
		bool InitializeFBX(FbxManager** manager, FbxScene** scene, const char* filename);
		void FinalizeFBX(FbxManager** manager);
		bool LoadFBXNodeRecursive(FbxNode *node);

		//!@brief 読み込んだデータをもとに地形コリジョンを作成
		//!@param[in] physics コリジョンを管理する物理クラスへのポインタ
		void SetCollisionWorld(BulletPhysics *physics, int myselfMask, int giveMask);


	private:
		//アクセス用の物理クラス
		K_Physics::BulletPhysics* physics;

		//判定用の頂点配列
		std::vector<PolygonData> polygonStack;

		int numFace;

		btTriangleMesh* collisionMesh;
		K_Physics::RigidBodyData* rigid;
		K_Physics::CollisionShape* shape;
	};
}
