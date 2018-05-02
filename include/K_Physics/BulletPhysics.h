#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<bullet\BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include<bullet\BulletCollision\CollisionDispatch\btGhostObject.h>
#include"K_Graphics\ShaderClass.h"
#include"K_Graphics\CameraClass.h"
#include"bulletDebugDraw.h"
#include"K_Math\MyMathFanctions.h"

#include<vector>

namespace K_Physics {

	typedef btCollisionShape CollisionShape;

	//!@brief 衝突時に返すタグ情報\n
	//!tagName : タグの名前 デフォルトで "default"\n
	//!tagIndex : タグに付属させる数字 デフォルトで 0\n
	//!userData : タグと共に送りたい情報へのポインタ デフォルトで nullptr\n
	struct CollisionTag {
		std::string tagName;
		int tagIndex;
		void* userData;
	};

	//!@brief コリジョンの移動機能を持つ、要はbulletのコリジョン情報を扱いやすくしたクラス
	class CollisionData {
	public:
		CollisionData(btCollisionObject* obj, int myselfMask, int giveMask, CollisionTag tag);
		void SetCollisionPosition(const K_Math::Vector3& position);
		K_Math::Vector3 GetCollisionPosition();
		btCollisionObject* GetCollision();

		void SetMyselfMask(int mask);
		void SetGiveMask(int mask);

		int GetMyselfMask();
		int GetGiveMask();

	public:
		CollisionTag tag;
		
	protected:
		btCollisionObject* const collision;
		//!@brief 衝突判定時に「自分が」使うビットマスク
		int myselfMask;
		//!@brief 衝突判定時に「相手が」使うビットマスク
		int giveMask;
	};

	//!@brief CollisionDataに加えて、物理系に必要な関数を追加した派生クラス
	class RigidBodyData : public CollisionData {
	public:
		RigidBodyData(btRigidBody* obj, int myselfMask, int giveMask, CollisionTag tag);
		void AddForce(const K_Math::Vector3& vector);
		void Activate(bool frag = true);
	};


	//!@brief bulletのワールドを管理するクラス\nコリジョンの生成・判定関数を提供している
	class BulletPhysics {
	public:

	public:
		//!@brief Initialize()を呼ぶ
		BulletPhysics();
		//!@brief Finalize()を呼ぶ
		~BulletPhysics();
		//!@brief bulletの初期化を行う
		bool Initialize();
		//!@brief bulletの終了処理を行う
		void Finalize();

		//!@brief bulletの物理世界を更新して座標を変更する\nプレイヤーの地形判定関数は別駆動なのでフレームの初めに呼ぶとよい
		void Run();
		//!@brief bulletのコリジョンを可視化する
		//!@param[in] shader 使用するシェーダー
		//!@param[in] camera 使用するカメラ
		//!@param[in] trans 変形行列（省略時単位行列）
		void DebugDraw(K_Graphics::ShaderClass* shader, K_Graphics::CameraClass* camera, const K_Math::Matrix4x4& trans = K_Math::Matrix4x4::Identity());

		//!@brief 三角形の形状を作成
		//!@param[in] point1 角の座標
		//!@param[in] point2 角の座標
		//!@param[in] point3 角の座標
		CollisionShape* CreateTriangleHullShape(const K_Math::Vector3& point1, const K_Math::Vector3& point2, const K_Math::Vector3& point3);
		//!@brief btTriangleMeshからメッシュ形状を作成
		//!@param[in] mesh CreateTriangleMesh()が返す形状のポインタ
		CollisionShape* CreateTriangleMeshShape(btTriangleMesh* mesh);
		//!@brief 球の形状を作成
		//!@param[in] radius 球の半径
		CollisionShape* CreateSphereShape(float radius);
		//!@brief カプセルの形状を作成
		//!@param[in] radius 球の半径
		//!@param[in] height カプセルの高さ
		CollisionShape* CreateCapsuleShape(float radius, float height);
		//!@brief 直方体の形状を作成
		//!@param[in] halfWidth 中心からの直方体の幅
		//!@param[in] halfHeight 中心からの直方体の高さ
		//!@param[in] halfDepth 中心からの直方体の奥行き
		CollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);
		//!@brief ポリゴンの集合形状情報を作成
		//!@param[in] vectice 三角形の角座標の配列
		//!@param[in] numFace 三角形の数
		btTriangleMesh* CreateTriangleMesh(K_Math::Vector3* vectice, int numFace);


		//!@brief 剛体オブジェクトを作成し、ポインタを返す
		//!@param[in] shape 剛体の形状へのポインタ
		//!@param[in] mass 剛体の質量
		//!@param[in] ghost コリジョンが剛体と衝突するかのフラグ（trueで剛体とは衝突しない）
		//!@param[in] mask 衝突フィルタに使うビットマスク
		//!@param[in] pos 剛体の初期位置（省略時はすべて０）
		//!@param[in] rot 剛体の回転（省略時はすべて０）
		RigidBodyData* CreateRigidBody(btCollisionShape* shape, btScalar mass, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));
		//!@brief コリジョンオブジェクトを作成し、ポインタを返す
		//!@param[in] shape コリジョンの形状へのポインタ
		//!@param[in] ghost コリジョンが剛体と衝突するかのフラグ（trueで剛体とは衝突しない）
		//!@param[in] mask 衝突フィルタに使うビットマスク
		//!@param[in] pos コリジョンの初期位置（省略時はすべて０）
		//!@param[in] rot コリジョンの回転（省略時はすべて０）
		CollisionData* CreateCollisionObject(btCollisionShape* shape, bool ghost, int myselfMask, int giveMask, const K_Math::Vector3& pos = K_Math::Vector3(0, 0, 0), const K_Math::Vector3& rot = K_Math::Vector3(0, 0, 0));

		//!@brief 明示的に世界に登録している剛体を世界から外してからポインタをdeleteする\nこのクラスのデストラクタにてこの関数によって全て開放している
		void RemoveCollision(CollisionData** rigidbody);

		//!@brief 明示的にリストに存在する形状情報をリストから外してdeleteする
		//!この関数を呼ばなくても、このクラスのデストラクタで全て開放している
		//!この関数の後は、形状へのポインタは使えないので注意
		void RemoveCollisionShape(CollisionShape** shape);

		//!@brief 操作性を意識したコリジョンの移動、壁判定も行う(重いが正確)
		//!@param[in] obj 移動するコリジョンオブジェクト
		//!@param[in] move 移動ベクトル
		//!@param[in] vLimitAngle 縦方向の壁ずりを起こさない角度(deg)、この角度以上の入射角度に対して壁ずりを行う
		//!@param[in] hLimitAngle 横方向の壁ずりを起こさない角度(deg)、この角度以上の入射角度に対して壁ずりを行う
		void MoveCharacter(CollisionData* obj, const K_Math::Vector3& move, float vLimitAngle = 40.0f, float hLimitAngle = 0.0f);

		//!@brief 離散的なコリジョンの移動、判定が MoveCharacter よりも大雑把(ただし軽い)
		//!@param[in] obj 移動するコリジョンオブジェクト
		//!@param[in] move 移動ベクトル
		//!@param[in] vLimitDirection 縦方向の押し返しを、moveの直線上に限定するかのフラグ
		//!@param[in] hLimitDirection 横方向の押し返しを、moveの直線上に限定するかのフラグ
		void MoveCharacterDiscrete(CollisionData* obj, const K_Math::Vector3& move, bool vLimitDirection = true, bool hLimitDirection = false);

		//!@brief 現在の物理世界での特定のオブジェクトに対する衝突のチェック
		//!@param[in] 衝突をチェックしたいオブジェクト
		//!@return 衝突が起こったオブジェクトのタグ情報
		std::vector<CollisionTag*>& FindConfrictionObjects(CollisionData* myself);

		//!@brief 重力とは反対方向を指す単位ベクトルを設定、キャラクターの移動に利用する
		//!@param[in] vector 重力と反対方向の単位ベクトル
		void SetSkyVector(const K_Math::Vector3& vector);
	private:
		//コリジョンを削除
		void RemoveCollisionObject(btCollisionObject* rigidbody);
		//コリジョンを移動
		void MoveCollisionObject(btCollisionObject* obj, const btVector3& moveVector);
		//指定方向に移動（離散判定）
		void MoveDiscrete(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection);
		//指定方向に移動
		void MoveSmooth(btCollisionObject* obj, const btVector3& moveVector, float limitAngle, bool limitDirection);
		//移動部分をまとめ、allowDistanceはめり込み許容値、isCalcurateがtrueの時は法線を返す
		btVector3 MoveBySweep(btCollisionObject* obj, const btVector3& moveVector, bool limitDirection, float allowDistance = 0.2f);
	private:
		//衝突結果格納用
		std::vector<CollisionTag*> confrictResult;
		//空への単位ベクトル、あたり判定で使用
		btVector3 toSkyVector;

		btDiscreteDynamicsWorld*                bulletWorld;
		btDefaultCollisionConfiguration*        config;
		btCollisionDispatcher*                  dispatcher;
		btConstraintSolver*                     solver;
		btBroadphaseInterface*                  broadphase;
		bulletDebugDraw                         debugDrawer;
		btAlignedObjectArray<CollisionShape*>   shapeArray;
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
		CollectCollisionCallBack(std::vector<CollisionTag*>& tagList);
		virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

	public:
		std::vector<CollisionTag*>& result;
		bool isHit;
	};

}