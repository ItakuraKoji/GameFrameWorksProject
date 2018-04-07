#pragma once
#include"ModelData.h"
#include"ModelDataFactory.h"
#include"FbxModelLoader.h"
#include"CameraClass.h"
#include"ShaderClass.h"

namespace K_Graphics {

	//モデルクラス。ファクトリーから生産されたモデルデータを受け取って初期化
	//「形状」という扱いなので、描画時はモデルデータ以外の必要な情報を外から渡すものとする
	class MeshModel {
	public:
		MeshModel(ModelDatas* data);
		~MeshModel();

		bool Initialize(ModelDatas* data);
		void Finalize();
		void SetAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation, int interpolationFrames);
		void SetTexture(Texture* texture, int arrayIndex, int materialIndex);
		void SetSpeed(float speed);

		void UpdateAnimation();
		void Draw(ShaderClass* shader);
		void InstanceDraw(int numInstance, ShaderClass* shader);

	private:
		void SetBone(int arrayIndex, ShaderClass* shader);
		void DrawBuffers(int arrayIndex, ShaderClass* shader);

	private:
		ModelDatas * data;
		Texture*  boneTexture;
		bool isBoneProcessed;
	};

	//3Dモデルの描画周りをまとめたクラス
	class MeshObject {
	public:
		MeshObject(MeshModel* model);
		~MeshObject();

		bool Initialize(MeshModel* model);
		void Finalize();

		void SetBoneAnimation(const std::string& animationName, bool playOnce, bool isLoop, bool isInterporation = true, int interpolationFrames = 5);
		void SetSpeed(float speed);

		void UpdateAnimation();

		void Draw(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		void InstanceDraw(CameraClass* camera, ShaderClass* shader, int numDraw, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

	protected:
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);
		MeshModel* drawModel;
	};

	//2Dスプライト描画周りをまとめたクラス
	//1辺の長さ1.0fの板ポリゴンをスプライトっぽく拡縮、UV座標を指定することができる

	class SpriteObject {
	public:
		SpriteObject(Texture* texture, float controlPointX = 0.0f, float controlPointY = 0.0f);
		~SpriteObject();

		bool Initialize(Texture* texture, float controlPointX, float controlPointY);
		void Finalize();

		bool SetTexture(Texture* texture);
		void Draw2D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Box2D& draw, float rotation);
		void Draw3D(CameraClass* camera, ShaderClass* shader, const K_Math::Box2D& src, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale);

	public:
		K_Math::Vector2 controlPoint;
	protected:
		MeshModel* drawModel;
		void SetMatrix(CameraClass* camera, ShaderClass* shader, const K_Math::Vector3& position, const K_Math::Vector3& rotation, const K_Math::Vector3& scale, bool billBoard);
		Texture* cullentTexture;
	};

}
