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

		//2D�p��Trandform�\����
		struct Transform {
		public:
			Transform();

		public:
			K_Math::Vector3 position;
			float rotation;
			K_Math::Vector2 scale;
		};

		//�A�j���[�V��������i�����e�N�X�`���ł̃A�j���[�V�����j
		//Box2D�Ő؂���ʒu��A�ԂŔz��Ɏ��߂ēo�^����
		class TextureAnimation {
		public:
			//�A�j���[�V����1���̏��A�E�F�C�g�ƃe�N�X�`���؂���ʒu������
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

			//�A�j���[�V������z������Ē�`
			void SetDataArray(const std::vector<Data>& dataArray);
			//�A�j���[�V��������R�}�ǉ�
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

		//�Z�b�^�[

		//�X�N���[���ʒu
		void SetPosition(const K_Math::Vector3& position);
		void SetPosition(const K_Math::Vector2& position, float posZ);
		void OffsetPosition(const K_Math::Vector2& vector);
		//���v��肪���̉�]
		void SetRotation(float rotation);
		//�g�k
		void SetScale(const K_Math::Vector2& scale);
		void SetScale(int width, int height);
		void Scaling(const K_Math::Vector2& scale);
		//��]�Ɗg�k�̒��S
		void SetControlPoint(const K_Math::Vector2& controlPoint);
		//�e�N�X�`���̐؂���ʒu
		void SetTexturePosition(const K_Math::Box2D& src);

		//�A�j���[�V������ǉ�
		void SetAnimation(const std::string& name, const TextureAnimation& animData);

		//��`�ς݂̃A�j���[�V�����ɐ؂�ւ�
		void ChangeAnimation(const std::string& name, bool isStop);

		//�摜��ݒ�
		void SetTexture(K_Graphics::Texture* texture);
		//�摜�̐F
		void SetColor(const K_Math::Vector4& color);
		//�`�悷�邩
		void SetDrawFrag(bool isDraw);

		void SetMaxFillAngle(float degree);
		void SetFillRatio(float ratio);

		//�Q�b�^�[

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
		//�Ǘ��ԍ��i�ύX�s�j
		const SpriteID id;
		//�ʒu
		Transform trans;
		//��]��g�k�̒��S�n�_�̍��W(1�{�ł̐��l)
		K_Math::Vector2 controlPoint;
		//�e�N�X�`���̍��̐؂���ʒu
		K_Math::Box2D src;
		//�e�N�X�`���inull�̏ꍇ������j
		K_Graphics::Texture* textureImage;
		//�e�N�X�`���J���[
		K_Math::Vector4 color;

		std::string animName;
		//�e�N�X�`���̃A�j���[�V�������
		std::unordered_map<std::string, TextureAnimation> animData;

		//�~�Q�[�W�Ƃ��Ŏg����A�\���p�x�i�O�x�̕����͂܂��w��ł��Ȃ��j
		float fillMaxAngle;
		float fillRatio;

		//���������`�悷�邩�ǂ����̐ݒ�
		bool isDraw;
	};


	//2D�`��Ǘ��N���X�A�p�����ăX�v���C�g����ǂ��`�悷�邩���`���Ďg��
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
		//�X�v���C�g��̕`��
		virtual void DrawStart() = 0;
		virtual void DrawUnit(SpriteData* spriteData) = 0;
		virtual void DrawEnd() = 0;


	private:
		//�`����s���X�v���C�g
		K_Graphics::SpriteObject* sprite;
		//�X�v���C�g�̃f�[�^�̔z��
		std::vector<SpriteData*> spriteArray;
	};

}