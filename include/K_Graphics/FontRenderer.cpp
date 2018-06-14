#include"FontRenderer.h"

namespace K_Graphics {

	////////
	//public
	////

	FontRenderer::FontRenderer() {
		//ロケールを設定し、フォント生成クラスを初期化
		try {
			setlocale(LC_ALL, "japanese");
			this->sprite = new K_Graphics::SpriteObject(nullptr);
			this->cullentFont = nullptr;
		}
		catch (std::exception& e) {
			throw e;
		}
	}
	FontRenderer::~FontRenderer() {
		for (int i = 0; i < this->drawData2D.size(); ++i) {
			delete[] this->drawData2D[i].buffer;
		}
		for (auto i : this->generators) {
			if (i.second != nullptr)
				delete i.second;
		}
		if (this->sprite != nullptr) {
			delete this->sprite;
			this->sprite = nullptr;
		}
	}

	bool FontRenderer::LoadFont(const char* fontName, const char* filePath) {
		//既にある名前では登録できない
		if (this->generators.find(fontName) != this->generators.end()) {
			throw("FontError name has already existed");
		}
		FontGenerator* font = new FontGenerator(filePath);
		this->generators[fontName] = font;

		SetCullentFont(fontName);
		return true;
	}
	bool FontRenderer::SetCullentFont(const char* fontName) {
		if (this->generators.find(fontName) == this->generators.end()) {
			throw("FontError name is not exists");
		}
		this->cullentFont = this->generators[fontName];
		return true;
	}

	void FontRenderer::Draw2D(CameraClass* camera, ShaderClass* shader) {
		if (this->cullentFont == nullptr) {
			for (int i = 0; i < this->drawData2D.size(); ++i) {
				delete this->drawData2D[i].buffer;
			}
			this->drawData2D.clear();
			return;
		}

		//2D
		for (int i = 0; i < this->drawData2D.size(); ++i) {
			//サイズに合ったテクスチャを選択
			Texture* texture = this->cullentFont->GetFontAtlas(this->drawData2D[i].fontSize);
			this->sprite->SetTexture(texture);

			//位置をずらしながら一文字づつ描画
			int position = 0;
			if (this->drawData2D[i].buffer == nullptr) {
				continue;
			}
			for (int j = 0; this->drawData2D[i].buffer[j] != L'\0'; ++j) {
				FontGenerator::FontData data;
				this->cullentFont->GetFontData(data, this->drawData2D[i].buffer[j], this->drawData2D[i].fontSize);
				this->sprite->Draw2D(camera, shader, data.locationUV, K_Math::Box2D((int)this->drawData2D[i].posX + position, (int)this->drawData2D[i].posY + data.fixPositionY, data.locationUV.w, data.locationUV.h), 0.0f);
				position += data.advance;
			}
			delete[] this->drawData2D[i].buffer;
		}
		this->drawData2D.clear();
	}

	void FontRenderer::Draw3D(CameraClass* camera, ShaderClass* shader) {
		if (this->cullentFont == nullptr) {
			for (int i = 0; i < this->drawData2D.size(); ++i) {
				delete this->drawData2D[i].buffer;
			}
			this->drawData2D.clear();
			return;
		}

		//3D
		//カメラから遠い順にソート
		std::multimap<float, FontDrawData> ordered;
		for (int i = 0; i < this->drawData3D.size(); ++i) {
			//カメラ座標系のZ座標にマイナスをつける
			float distance = -glm::dot(camera->GetAxisZ(), K_Math::Vector3(this->drawData3D[i].posX, this->drawData3D[i].posY, this->drawData3D[i].posZ));
			//同じ数字の場合は、後に呼んだ描画が優先
			ordered.insert(std::make_pair(distance, this->drawData3D[i]));
		}
		//ソートした順番で描画
		for (auto i : ordered) {
			DrawBuffers3D(i.second, camera, shader);
			//使い終わったら消去
			delete[] i.second.buffer;
		}
		this->drawData3D.clear();
	}

	void FontRenderer::DrawString2D(const std::string& text, int size, int posX, int posY) {
		if (this->cullentFont == nullptr) {
			return;
		}

		//ワイド文字列に変換
		wchar_t* buffer = new wchar_t[text.size() + 1];
		if (!CreateFontBuffer(buffer, text.data(), (int)text.size() + 1)) {
			delete[] buffer;
			return;
		}
		//必要なら新規テクスチャ作成
		for (int i = 0; buffer[i] != L'\0'; ++i) {
			this->cullentFont->SetFont(buffer[i], size);
		}

		//posZは2Dでは利用しない
		FontDrawData drawData;
		drawData.buffer = buffer;
		drawData.fontSize = size;
		drawData.posX = (float)posX;
		drawData.posY = (float)posY;

		this->drawData2D.push_back(drawData);
	}

	void FontRenderer::DrawString3D(const std::string& text, int size, float posX, float posY, float posZ) {
		if (this->cullentFont == nullptr) {
			return;
		}

		//ワイド文字列に変換
		wchar_t* buffer = new wchar_t[text.size() + 1];
		if (!CreateFontBuffer(buffer, text.data(), (int)text.size() + 1)) {
			delete[] buffer;
			return;
		}
		//必要なら新規テクスチャ作成
		for (int i = 0; buffer[i] != L'\0'; ++i) {
			this->cullentFont->SetFont(buffer[i], size);
		}
		FontDrawData drawData;
		drawData.buffer = buffer;
		drawData.fontSize = size;
		drawData.posX = posX;
		drawData.posY = posY;
		drawData.posZ = posZ;

		this->drawData3D.push_back(drawData);
	}


	void FontRenderer::ClearFontTexture(int size) {
		this->cullentFont->ClearFontAtlas(size);
	}

	void FontRenderer::ClearAllFontTexture() {
		this->cullentFont->ClearAllFontAtlas();
	}

	////////
	//private
	////

	bool FontRenderer::CreateFontBuffer(wchar_t* result, const char* src, int numChar) {
		size_t bufferSize;
		mbstowcs_s(&bufferSize, result, numChar, src, numChar);
		//不正なワイド文字による変換エラー
		if (bufferSize == -1) {
			//なんかエラー処理を書く
			return false;
		}
		return true;
	}

	void FontRenderer::DrawBuffers3D(FontDrawData& drawData, CameraClass* camera, ShaderClass* shader) {
		//サイズに合ったテクスチャを選択
		Texture* texture = this->cullentFont->GetFontAtlas(drawData.fontSize);
		this->sprite->SetTexture(texture);

		//位置をずらしながら一文字づつ描画
		int stringSize = 0;
		K_Math::Vector3 position(0.0f, 0.0f, 0.0f);
		K_Math::Vector3 advanceDir = camera->GetAxisX();
		if (drawData.buffer == nullptr) {
			return;
		}
		//文章全体の横の長さを取得
		for (int j = 0; drawData.buffer[j] != L'\0'; ++j) {
			FontGenerator::FontData data;
			this->cullentFont->GetFontData(data, drawData.buffer[j], drawData.fontSize);
			stringSize += data.advance;
		}

		//そのまま描画するとデカすぎるので補正をかける
		float size = 0.03f;
		//実際の描画
		for (int j = 0; drawData.buffer[j] != L'\0'; ++j) {
			FontGenerator::FontData data;
			this->cullentFont->GetFontData(data, drawData.buffer[j], drawData.fontSize);
			K_Math::Vector3 pos3D;
			pos3D.x = drawData.posX;
			pos3D.y = drawData.posY - data.fixPositionY * size;
			pos3D.z = drawData.posZ;
			//文字全体の半分だけ後ろにずらして描画することで、コントロールポイントを中心に
			pos3D += position - (advanceDir * (float)stringSize / 2.0f * size);

			this->sprite->Draw3D(camera, shader, data.locationUV, pos3D, K_Math::Vector3(0.0f, 0.0f, 0.0f), K_Math::Vector3(size, size, size));
			
			position += advanceDir * (float)data.advance * size;
		}
	}
}


