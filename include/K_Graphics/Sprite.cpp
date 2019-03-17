#include"Sprite.h"

namespace K_Graphics {
	//比較関数
	bool cmp(SpriteData* left, SpriteData* right) {
		return left->GetTrans().position.z > right->GetTrans().position.z;
	}


	SpriteData::Transform::Transform() {
		//位置、拡縮はコンストラクタで初期値が定義されている
		this->rotation = 0.0f;
	}


	SpriteData::SpriteData(SpriteID id, K_Graphics::Texture* textureImage) :
		id(id)
	{
		this->textureImage = textureImage;
		this->trans.position = K_Math::Vector3(0.0f, 0.0f, 0.0f);
		this->trans.rotation = 0.0f;
		this->trans.scale = K_Math::Vector2(1.0f, 1.0f);
		this->controlPoint = K_Math::Vector2(0.0f, 0.0f);
		this->color = K_Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		this->isDraw = true;
		if (textureImage != nullptr) {
			this->src = K_Math::Box2D(0, 0, textureImage->GetWidth(), textureImage->GetHeight());
		}
		this->fillMaxAngle = 360.0f;
		this->fillRatio = 1.0f;
	}
	SpriteData::~SpriteData() {
	}

	void SpriteData::UpdateAnimation() {
		if (this->animData[this->animName].IsPlay()) {
			this->animData[this->animName].Run();
			this->src = this->animData[this->animName].GetData().texturePosition;
		}
	}

	void SpriteData::PlayAnimation() {
		this->animData[this->animName].Play();
	}

	void SpriteData::StopAnimation() {
		this->animData[this->animName].Stop();
	}

	void SpriteData::PauseAnimation() {
		this->animData[this->animName].Pause();
	}

	//セッタ
	void SpriteData::SetPosition(const K_Math::Vector3& position) {
		this->trans.position = position;
	}
	void SpriteData::SetPosition(const K_Math::Vector2& position, float posZ) {
		this->trans.position = K_Math::Vector3(position.x, position.y, posZ);
	}
	void SpriteData::OffsetPosition(const K_Math::Vector2& vector) {
		this->trans.position += K_Math::Vector3(vector.x, vector.y, 0.0f);
	}


	void SpriteData::SetRotation(float rotation) {
		this->trans.rotation = rotation;
	}

	void SpriteData::SetScale(const K_Math::Vector2& scale) {
		this->trans.scale = scale;
	}

	void SpriteData::SetScale(int width, int height) {
		this->trans.scale.x = this->src.w ? (float)width / this->src.w : 0.0f;
		this->trans.scale.y = this->src.h ? (float)height / this->src.h : 0.0f;
	}

	void SpriteData::Scaling(const K_Math::Vector2& scale) {
		this->trans.scale *= scale;
	}

	void SpriteData::SetControlPoint(const K_Math::Vector2& controlPoint) {
		this->controlPoint = controlPoint;
	}

	void SpriteData::SetTexturePosition(const K_Math::Box2D& src) {
		this->src = src;
	}

	void SpriteData::SetAnimation(const std::string& name, const TextureAnimation& animData) {
		//名前が被ってても上書きする
		this->animData[name] = animData;
	}

	void SpriteData::ChangeAnimation(const std::string& name, bool isStop) {
		//アニメーション切替
		this->animName = name;
		if (isStop) {
			StopAnimation();
		}
	}

	void SpriteData::SetTexture(K_Graphics::Texture* texture) {
		this->textureImage = texture;
		if (textureImage != nullptr) {
			this->src = K_Math::Box2D(0, 0, textureImage->GetWidth(), textureImage->GetHeight());
		}
	}

	void SpriteData::SetColor(const K_Math::Vector4& color) {
		this->color = color;
	}

	void SpriteData::SetDrawFrag(bool isDraw) {
		this->isDraw = isDraw;
	}


	void SpriteData::SetMaxFillAngle(float degree) {
		this->fillMaxAngle = degree;
	}
	void SpriteData::SetFillRatio(float ratio) {
		this->fillRatio = ratio;
	}

	//ゲッタ
	const SpriteData::Transform& SpriteData::GetTrans() const {
		return this->trans;
	}

	const K_Math::Vector2& SpriteData::GetControlPoint() const {
		return this->controlPoint;
	}

	const K_Math::Box2D& SpriteData::GetTexturePosition() const {
		return this->src;
	}

	K_Graphics::Texture* SpriteData::GetTexture() const {
		return this->textureImage;
	}

	SpriteData::SpriteID SpriteData::GetID() {
		return this->id;
	}

	const K_Math::Vector4& SpriteData::GetColor() {
		return this->color;
	}

	float SpriteData::GetImageWidth() {
		if (this->textureImage == nullptr) {
			return 0;
		}
		return (float)this->textureImage->GetWidth();
	}
	float SpriteData::GetImageHeight() {
		if (this->textureImage == nullptr) {
			return 0;
		}
		return (float)this->textureImage->GetHeight();
	}
	float SpriteData::GetImageAspectHW() {
		if (this->textureImage == nullptr) {
			return 0;
		}
		return (float)this->textureImage->GetHeight() / this->textureImage->GetWidth();
	}

	float SpriteData::GetWidth() {
		return (float)src.w;
	}
	float SpriteData::GetHeight() {
		return (float)src.h;
	}

	bool SpriteData::IsDraw() {
		return this->isDraw;
	}

	float SpriteData::GetMaxFillAngle() {
		return this->fillMaxAngle;
	}
	float SpriteData::GetFillRatio() {
		return this->fillRatio;
	}


	////////
	//SpriteManager
	////
	SpriteRenderer::SpriteRenderer(DrawPassManager* drawPassManager){
		SetDrawPassManager(drawPassManager);
		this->sprite = new K_Graphics::SpriteObject(nullptr);
	}


	SpriteRenderer::~SpriteRenderer() {
		//描画用のスプライトを削除
		if (this->sprite != nullptr) {
			delete this->sprite;
		}
		//登録されたスプライトを開放
		for (auto spriteData : this->spriteArray) {
			if (spriteData == nullptr) {
				continue;
			}
			delete spriteData;
		}
	}

	void SpriteRenderer::Run() {
		for (auto& sprite : this->spriteArray) {
			sprite->UpdateAnimation();
		}
	}

	void SpriteRenderer::Draw() {
		//前準備の設定
		DrawStart();
		
		//Z座標に応じて描画順番を調整
		std::vector<SpriteData*> drawList;
		drawList.reserve(this->spriteArray.size());

		//存在するスプライトだけ登録
		for (auto spriteData : this->spriteArray) {
			if (spriteData == nullptr) {
				continue;
			}
			drawList.push_back(spriteData);
		}

		//ソート
		std::sort(drawList.begin(), drawList.end(), cmp);

		//奥から描画
		for (auto spriteData : drawList) {
			//描画フラグが下りているなら描画しない
			if (!spriteData->IsDraw()) {
				continue;
			}
			DrawUnit(spriteData);
		}

		//後処理の設定
		DrawEnd();
	}

	void SpriteRenderer::DrawSprite(SpriteData* spriteData) {
		DrawUnit(spriteData);
	}

	SpriteData* SpriteRenderer::GetSprite(SpriteData::SpriteID id) {
		if (this->spriteArray.size() <= id || id < 0) {
			//範囲外はnullを返す
			return nullptr;
		}
		return this->spriteArray[id];
	}

	SpriteData* SpriteRenderer::CreateSprite(K_Graphics::Texture * textureImage) {
		int position = -1;
		for (int i = 0; i < this->spriteArray.size(); ++i) {
			//すでに存在しているのはスキップ
			if (this->spriteArray[i] != nullptr) {
				continue;
			}
			//開いている場所を見つけたら番号を記録してループ終了
			position = i;
			break;
		}
		SpriteData* newSprite;
		if (position == -1) {
			//場所が見つからなかった場合はリスト新規追加
			position = (int)this->spriteArray.size();
			newSprite = new SpriteData(position, textureImage);
			this->spriteArray.push_back(newSprite);
		}
		else {
			//場所があったらそこに登録
			newSprite = new SpriteData(position, textureImage);
			this->spriteArray[position] = newSprite;
		}
		//新規スプライトを返す
		return newSprite;
	}

	void SpriteRenderer::DeleteSprite(SpriteData* target) {
		if (this->spriteArray.size() <= target->GetID()) {
			//範囲外は何もしない
			return;
		}
		int id = target->GetID();
		delete this->spriteArray[id];
		this->spriteArray[id] = nullptr;
	}

	SpriteData::TextureAnimation::Data::Data(const K_Math::Box2D& texturePosition, float waitFrame) {
		this->texturePosition = texturePosition;
		this->waitFrame = waitFrame;
	}
	SpriteData::TextureAnimation::TextureAnimation() {
		this->frameCount = 0.0f;
		this->isLoop = false;
		this->numAnimData = 0;
		this->isPlay = false;
		this->speed = 1.0f;
		this->currentPosition = 0;
	}
	bool SpriteData::TextureAnimation::IsPlay() {
		return this->isPlay;
	}
	void SpriteData::TextureAnimation::Play() {
		if (this->numAnimData == 0) {
			return;
		}
		this->isPlay = true;
	}
	void SpriteData::TextureAnimation::Stop() {
		this->isPlay = false;
		this->currentPosition = 0;
	}
	void SpriteData::TextureAnimation::Pause() {
		this->isPlay = false;
	}
	void SpriteData::TextureAnimation::Run() {
		if (this->numAnimData == 0) {
			return;
		}

		this->frameCount += this->speed;

		if (this->frameCount < GetData().waitFrame) {
			return;
		}
		this->frameCount = 0.0f;
		this->currentPosition += 1;

		if (this->currentPosition >= this->numAnimData) {
			if (this->isLoop) {
				this->currentPosition = 0;
			}
			else {
				this->currentPosition = 0;
				this->isPlay = false;
			}
		}
	}
	const SpriteData::TextureAnimation::Data& SpriteData::TextureAnimation::GetData() {
		return this->animData[(int)this->currentPosition];
	}
	void SpriteData::TextureAnimation::SetDataArray(const std::vector<Data>& dataArray) {
		this->animData = dataArray;
		this->numAnimData = (int)this->animData.size();
	}
	void SpriteData::TextureAnimation::AddData(const Data& data) {
		this->animData.push_back(data);
		this->numAnimData += 1;
	}
}