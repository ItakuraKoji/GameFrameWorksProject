#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<bullet\BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include<bullet\BulletCollision\CollisionDispatch\btGhostObject.h>
#include"ShaderClass.h"
#include"CameraClass.h"
#include"bulletDebugDraw.h"
#include"MyMathFanctions.h"

#include<iostream>
#include<vector>

namespace K_Physics {

	//衝突時に返すタグ情報
	struct CollisionTag {
		std::string tagName;
		int tagIndex;
		void* userData;
	};

	//bulletのコリジョン情報を扱いやすくしたクラス
	class CollisionData {
	public:
		CollisionData(btCollisionObject* obj, CollisionTag tag);
		void SetCollisionPosition(const btVector3& position);
		btVector3 GetCollisionPosition();

	public:
		btCollisionObject * const collision;
		CollisionTag tag;
	};

	//bulletのワールドを管理するクラス
	class BulletPhysics {
	public:

	public:
		BulletPhysics();
		~BulletPhysics();
		bool Initialize();
		void Finalize();
		void Run();
		void DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans);


		btCollisionShape* CreateTriangleHullShape(const btVector3& point1, const btVector3& point2, const btVector3& point3);
		btTriangleMesh* CreateTriangleMesh(btVector3* vectice, int numFace);
		btCollisionShape* CreateTriangleMeshShape(btTriangleMesh* mesh);
		btCollisionShape* CreateSphereShape(float radius);
		btCollisionShape* CreateCapsuleShape(float radius, float height);
		btCollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);


		//・剛体オブジェクトを作成し、ポインタを返す
		//・形状、質量、位置、回転の順
		//・衝突フィルタのクループとビットマスク
		//・位置と回転は省略可(省略時すべて０)
		CollisionData* CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, const btVector3& pos = btVector3(0, 0, 0), const btVector3& rot = btVector3(0, 0, 0));
		//物理演算で衝突するコリジョンを生成、ghostをtrueにすると物理演算はこのオブジェクトには働かない
		CollisionData* CreateCollisionObject(btCollisionShape* shape, bool ghost, int mask, const btVector3& pos = btVector3(0, 0, 0), const btVector3& rot = btVector3(0, 0, 0));

		//明示的に世界に登録している剛体を世界から外してからポインタをdelete
		//このクラスのデストラクタにてこの関数によって全て開放している
		void RemoveCollisionObject(btCollisionObject* rigidbody);

		//明示的にリストに存在するbtCollisionShapeをリストから外してdelete
		//この関数を呼ばなくても、このクラスのデストラクタで全て開放している
		void RemoveCollisionShape(btCollisionShape* shape);

		//・操作性を意識したコリジョンの移動、壁判定も行う(重いが正確)
		//引数
		//・移動するコリジョンオブジェクト
		//・移動ベクトル
		void MoveCharacter(btCollisionObject* obj, const btVector3& move);

		//・離散的なコリジョンの移動、判定が MoveCharacter よりも大雑把(ただし軽い)
		//引数
		//・移動するコリジョンオブジェクト
		//・移動ベクトル
		void MoveCharacterDiscrete(btCollisionObject* obj, const btVector3& hMove, const btVector3& vMove);

		//・現在の物理世界での特定のオブジェクトに対する衝突のチェック
		//戻り値
		//・衝突の有無
		//引数
		//・結果として返ってくるオブジェクトのvector
		//・衝突をチェックしたいオブジェクト
		//・衝突時に使用するビットマスク（BulletBitMask）
		std::vector<CollisionTag>& FindConfrictionObjects(btCollisionObject* myself);


		void SetSkyVector(const btVector3& vector);
	private:
		//コリジョンを移動
		void MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector);
		//指定方向に移動（離散判定）
		void MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection);
		//指定方向に移動（Sweep使用）
		btVector3 MoveSmooth(btCollisionObject* obj, const btVector3& moveVector, float limitAngle, bool limitDirection);
		//移動部分をまとめ
		btVector3 MoveBySweep(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection, float allowDistance = 0.2f);
	private:
		std::vector<CollisionTag> confrictResult;
		//空への単位ベクトル、あたり判定で使用
		btVector3 toSkyVector;

		btDiscreteDynamicsWorld*                bulletWorld;
		btDefaultCollisionConfiguration*        config;
		btCollisionDispatcher*                  dispatcher;
		btConstraintSolver*                     solver;
		btBroadphaseInterface*                  broadphase;
		bulletDebugDraw                         debugDrawer;
		btAlignedObjectArray<btCollisionShape*> shapeArray;
	};


	////自身と衝突しないsweepTestのコールバック
	struct SweepTestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
	public:
		SweepTestCallBack(btCollisionObject *myself);

		//自分自身にヒットしないようにオーバーライド
		//戻り値に意味はないみたい
		virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);
	public:
		btCollisionObject * myself;

	};

	//めり込み最大の法線ベクトルを見つけるコールバック
	struct FixContactCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		FixContactCallBack(btCollisionObject* obj);
		//当たったオブジェクトを記録
		//戻り値に意味はないみたい
		//こっちは自分自身（contactTestで渡したオブジェクト）とは衝突しないようです
		virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	public:
		//押し出すオブジェクト
		btCollisionObject * obj;
		//距離の最大値とその方向
		float maxDistance;
		btVector3 fixVec;

		int count;
		bool isLoop;
	};

	//すべての衝突を記録する
	struct CollectCollisionCallBack : public btCollisionWorld::ContactResultCallback {
	public:
		CollectCollisionCallBack(std::vector<CollisionTag>& tagList);
		virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	public:
		btCollisionObject * obj;
		std::vector<CollisionTag>& result;
		bool isHit;
	};

}