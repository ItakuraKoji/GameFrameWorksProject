#pragma once
#include"ModelData.h"
#include"ModelDataFactory.h"
#include"CameraClass.h"
#include"ShaderClass.h"

namespace K_Graphics {

	//!@brief 3Dモデルクラス\nファクトリーから生産されたモデルデータを受け取って初期化する\n
	//!「形状」という扱いで、描画時には「K_Graphics::ShaderClass」のポインタが必須
	class MeshModel {
	public:
		//!@brief Initialize()を呼ぶ
		MeshModel(ModelDatas* data);
		//!@brief Finalize()を呼ぶ
		~MeshModel();

		//!@brief モデルデータによって3Dモデルを初期化
		//!@param[in] ModelDataFactoryクラスによって生成されたモデルデータへのポインタ
		//!@return 成功するとtrue
		bool Initialize(ModelDatas* data);
		//!@brief 終了処理、モデルデータのポインタをdeleteしている
		void Finalize();
		//!@brief スキンメッシュアニメーションを選択、再生する
		//!@param[in] animationName アニメーションの名前（モデリングソフトで作成したときのアニメーション名）
		//!@param[in] playOnce 連続して同じアニメーションを再生したときに初めから再生しなおすか
		//!@param[in] isLoop アニメーションをループ再生するか
		//!@param[in] interpolationFrames フレーム単位の補間時間（０で補完しない）
		void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames);
		//!@brief テクスチャを変更する、基本は使わなくてもいい
		//!@param[in] texture 使用するテクスチャへのポインタ
		//!@param[in] arrayIndex 対象となるメッシュ階層のインデックス
		//!@param[in] materialIndex 対象となるメッシュ階層のなかのマテリアルインデックス
		void SetTexture(Texture* texture, int hierarchyIndex, int materialIndex);
		//!@brief アニメーションスピードを変更する
		//!@param[in] 1を通常の速度としたアニメーションの速度
		void SetSpeed(float speed);

		//!@brief アニメーションを更新
		void UpdateAnimation();
		//!@brief 描画を行う\nシェーダーには必要な行列がすでに入っているとする
		//!@param[in] shader 使用するシェーダー
		void Draw(ShaderClass* shader);
		//!@brief (未完成)インスタンス描画を行う\nシェーダーとの連携がまだできていないので未実装
		void InstanceDraw(int numInstance, ShaderClass* shader);

	private:
		void SetBone(int hierarchyIndex, ShaderClass* shader);
		void DrawBuffers(int hierarchyIndex, ShaderClass* shader);

	private:
		ModelDatas * data;
		Texture*  boneTexture;
		bool isBoneProcessed;
	};

	//!@brief K_Graphics::MeshModelの描画を"position""rotation""scale"の３つで行えるようにしたもの\n
	//!描画には「K_Graphics::CameraClass」「K_Graphics::ShaderClass」の二つが必要
	class MeshObject {
	public:
		//!@brief Initialize()を呼ぶ
		MeshObject(MeshModel* model);
		//!@brief Finalize()を呼ぶ
		~MeshObject();

		//!@brief MeshModelへのポインタを渡して初期化
		//!@param[in] model MeshModelへのポインタ
		//!@return 成功するとtrue
		bool Initialize(MeshModel* model);
		//!@brief Initialize()で渡したMeshModelへのポインタを開放している
		void Finalize();

		//!@brief スキンメッシュアニメーションを選択、再生する
		//!@param[in] animationName アニメーションの名前（モデリングソフトで作成したときのアニメーション名）
		//!@param[in] playOnce 連続して同じアニメーションを再生したときに初めから再生しなおすか
		//!@param[in] isLoop アニメーションをループ再生するか
		//!@param[in] inInterpolation 前のアニメーションと線形補間をするか(省略時は０になって補完しない)
		void SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, int interpolationFrames = 0);
		//!@brief アニメーションスピードを変更する
		//!@param[in] 1を通常の速度としたアニメーションの速度
		void SetSpeed(float speed);

		//!@brief アニメーションを更新
		void UpdateAnimation();

		//!@brief 各種行列を作り、シェーダーに渡して描画を行う
		//!@param[in] camera 使用するカメラクラスへのポインタ
		//!@param[in] shader 使用するシェーダーへのポインタ
		//!@param[in] position 3D空間上の位置座標
		//!@param[in] rotation XYZそれぞれの軸に関する回転角度（Y→X→Zの順で回転する）
		//!@param[in] scale スケーリング
		void Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		void Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);
		//!@brief インスタンス描画は未実装
		void InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

		//!@brief SRT姿勢からワールド変換行列を作成する
		//!@param[in] position 3D空間上の位置座標
		//!@param[in] rotation XYZそれぞれの軸に関する回転角度（Y→X→Zの順で回転する）
		//!@param[in] scale スケーリング
		//!@return ワールド変換行列
		K_Math::Matrix4x4 CreateWorldMatrix(const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		K_Math::Matrix4x4 CreateWorldMatrix(const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);

	protected:
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Quaternion& rotation, const K_Math::Vector3& scale);
		MeshModel* drawModel;
	};

	//!@brief 長さ1.0fの板ポリゴンのModelClassを内部で生成し、2D画像の描画に利用できる機能を付け加えたクラス\n
	//!描画には「K_Graphics::CameraClass」「K_Graphics::ShaderClass」の二つが必要
	class SpriteObject {
	public:
		
		//!@brief Initialize()を呼ぶ
		SpriteObject(Texture* texture, float controlPointX = 0.0f, float controlPointY = 0.0f);
		//!@brief Finalize()を呼ぶ
		~SpriteObject();

		//!@brief 初期化、テクスチャを設定し、変形のコントロールポイント座標を指定することもできる
		//!@param[in] texture 使用するテクスチャ、nullptrを指定することもできる
		//!@param[in] controlPointX コントロールポイントX座標\n板ポリゴンを左上原点で変形の影響を受けない
		//!@param[in] controlPointY コントロールポイントY座標\n板ポリゴンを左上原点で変形の影響を受けない
		bool Initialize(Texture* texture, float controlPointX, float controlPointY);
		//!@brief 終了処理
		void Finalize();

		//!@brief 表示するテクスチャを変更する
		//!@param[in] texture 使用するテクスチャのポインタ
		bool SetTexture(Texture* texture);
		//!@brief 2D描画を行う
		//!「スクリーンのサイズをカメラに設定している」「平行投影のカメラである」「Z軸方向を向きとZ軸と平行」
		//!という３つの条件を満たしたカメラを設定するとスクリーン座標での描画ができる
		//!@param[in] camera 使用するカメラ（上記の条件に注意）
		//!@param[in] shader 使用するシェーダー
		//!@param[in] src テクスチャの切り取り情報\n「テクスチャのピクセル座標XY」「矩形の幅と高さ」をそれぞれ"x""y""w""h"に設定する
		//!@param[in] draw 実際に描画する場所と大きさを指定する\n「位置座標XY」「矩形の幅と高さ」をそれぞれ"x""y""w""h"に設定する
		//!@param[in] rotation 右回転の角度（度数法）
		void Draw2D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Box2D& draw, float rotation);
		//!@brief 3D空間上に描画する、ただし必ずカメラの方を向くビルボードになる
		//!@param[in] camera 使用するカメラクラスへのポインタ
		//!@param[in] shader 使用するシェーダーへのポインタ
		//!@param[in] src テクスチャの切り取り情報\n「テクスチャのピクセル座標XY」「矩形の幅と高さ」をそれぞれ"x""y""w""h"に設定する
		//!@param[in] position 3D空間上の位置座標
		//!@param[in] rotation XYZそれぞれの軸に関する回転角度（Y→X→Zの順で回転する）
		//!@param[in] scale スケーリング
		void Draw3D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

		//!@brief SRT姿勢からワールド変換行列を作成する
		//!@param[in] camera 使用するカメラクラスへのポインタ、ビルボード回転に必要
		//!@param[in] position 3D空間上の位置座標
		//!@param[in] rotation XYZそれぞれの軸に関する回転角度（Y→X→Zの順で回転する）
		//!@param[in] scale スケーリング
		//!@param[in] billBoard ビルボードとしてスプライトを扱うかのフラグ
		//!@return ワールド変換行列
		K_Math::Matrix4x4 CreateWorldMatrix(CameraClass* camera, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale, bool billBoard);

	public:
		//!@brief コントロールポイントの座標
		K_Math::Vector2 controlPoint;
	protected:
		MeshModel* drawModel;
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale, bool billBoard);
		Texture* cullentTexture;
	};

}
