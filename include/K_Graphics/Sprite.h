#pragma once

#include"K_Graphics/DrawableObject.h"
#include"K_Math/MyMathFanctions.h"
#include"K_Graphics/MeshModel.h"
#include<vector>
#include<algorithm>

namespace K_Graphics {

	class SpriteData {
	public:
		typedef int SpriteID;

		//2D用のTrandform構造体
		struct Transform {
		public:
			Transform();

		public:
			K_Math::Vector3 position;
			float rotation;
			K_Math::Vector2 scale;
		};

		//アニメーション一つ分（同じテクスチャでのアニメーション）
		//Box2Dで切り取る位置を連番で配列に収めて登録する
		class TextureAnimation {
		public:
			//アニメーション1枚の情報、ウェイトとテクスチャ切り取り位置を持つ
			struct Data {
				Data(const K_Math::Box2D& texturePosition, float waitFrame);

				K_Math::Box2D texturePosition;
				float waitFrame;
			};
		public:
			TextureAnimation();

			bool IsPlay();
			void Play();
			void Stop();
			void Pause();
			void Run();
			const Data& GetData();

			//アニメーションを配列を入れて定義
			void SetDataArray(const std::vector<Data>& dataArray);
			//アニメーションを一コマ追加
			void AddData(const Data& data);

		private:
			int numAnimData;
			int currentPosition;
			float frameCount;
			bool isLoop;
			bool isPlay;
			float speed;
			std::vector<Data> animData;
		};

	public:
		SpriteData(SpriteID id, K_Graphics::Texture* textureImage);
		~SpriteData();

		void UpdateAnimation();
		void PlayAnimation();
		void StopAnimation();
		void PauseAnimation();

		//セッター

		//スクリーン位置
		void SetPosition(const K_Math::Vector3& position);
		void SetPosition(const K_Math::Vector2& position, float posZ);
		void OffsetPosition(const K_Math::Vector2& vector);
		//時計回りが正の回転
		void SetRotation(float rotation);
		//拡縮
		void SetScale(const K_Math::Vector2& scale);
		void SetScale(int width, int height);
		void Scaling(const K_Math::Vector2& scale);
		//回転と拡縮の中心
		void SetControlPoint(const K_Math::Vector2& controlPoint);
		//テクスチャの切り取り位置
		void SetTexturePosition(const K_Math::Box2D& src);

		//アニメーションを追加
		void SetAnimation(const std::string& name, const TextureAnimation& animData);

		//定義済みのアニメーションに切り替え
		void ChangeAnimation(const std::string& name, bool isStop);

		//画像を設定
		void SetTexture(K_Graphics::Texture* texture);
		//画像の色
		void SetColor(const K_Math::Vector4& color);
		//描画するか
		void SetDrawFrag(bool isDraw);

		void SetMaxFillAngle(float degree);
		void SetFillRatio(float ratio);

		//ゲッター

		const Transform& GetTrans() const;
		const K_Math::Vector2& GetControlPoint() const;
		const K_Math::Box2D& GetTexturePosition() const;
		K_Graphics::Texture* GetTexture() const;
		SpriteID GetID();
		const K_Math::Vector4& GetColor();
		float GetWidth();
		float GetHeight();
		float GetImageWidth();
		float GetImageHeight();
		float GetImageAspectHW();
		bool IsDraw();

		float GetMaxFillAngle();
		float GetFillRatio();

	private:
		//管理番号（変更不可）
		const SpriteID id;
		//位置
		Transform trans;
		//回転や拡縮の中心地点の座標(1倍での数値)
		K_Math::Vector2 controlPoint;
		//テクスチャの今の切り取り位置
		K_Math::Box2D src;
		//テクスチャ（nullの場合もある）
		K_Graphics::Texture* textureImage;
		//テクスチャカラー
		K_Math::Vector4 color;

		std::string animName;
		//テクスチャのアニメーション情報
		std::unordered_map<std::string, TextureAnimation> animData;

		//円ゲージとかで使える、表示角度（０度の方向はまだ指定できない）
		float fillMaxAngle;
		float fillRatio;

		//そもそも描画するかどうかの設定
		bool isDraw;
	};


	//2D描画管理クラス、継承してスプライト一つをどう描画するかを定義して使う
	class SpriteRenderer : public DrawableObject {
	public:
		SpriteRenderer(DrawPassManager* drawPassManager);
		~SpriteRenderer();

		virtual void Run() final;
		virtual void Draw() final;
		virtual void DrawSprite(SpriteData* spriteData);

		virtual SpriteData* GetSprite(SpriteData::SpriteID id) final;
		virtual SpriteData* CreateSprite(K_Graphics::Texture* textureImage) final;
		virtual void DeleteSprite(SpriteData* target) final;


	private:
		//スプライト一つの描画
		virtual void DrawStart() = 0;
		virtual void DrawUnit(SpriteData* spriteData) = 0;
		virtual void DrawEnd() = 0;


	private:
		//描画を行うスプライト
		K_Graphics::SpriteObject* sprite;
		//スプライトのデータの配列
		std::vector<SpriteData*> spriteArray;
	};

}