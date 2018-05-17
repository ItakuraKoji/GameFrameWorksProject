#include"EffectClass.h"

namespace K_Graphics {

	////////
	//public
	////
	EffectClass::EffectClass() {
		//とりあえず環境はOpenGL固定で考える
		this->manager = Effekseer::Manager::Create(2000);
		this->renderer = EffekseerRendererGL::Renderer::Create(2000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
		this->sound = EffekseerSound::Sound::Create(32);

		this->manager->SetModelRenderer(this->renderer->CreateModelRenderer());
		this->manager->SetRingRenderer(this->renderer->CreateRingRenderer());
		this->manager->SetSpriteRenderer(this->renderer->CreateSpriteRenderer());
		this->manager->SetRibbonRenderer(this->renderer->CreateRibbonRenderer());

		this->manager->SetModelLoader(this->renderer->CreateModelLoader());
		this->manager->SetTextureLoader(this->renderer->CreateTextureLoader());

		this->manager->SetSoundPlayer(this->sound->CreateSoundPlayer());
		this->manager->SetSoundLoader(this->sound->CreateSoundLoader());

		this->manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	}
	EffectClass::~EffectClass() {
		this->manager->StopAllEffects();
		for (auto i : this->effect) {
			if (i.second == nullptr) {
				continue;
			}
			i.second->Release();
			i.second = nullptr;
		}
		this->effect.clear();

		this->manager->Destroy();
		this->sound->Destroy();
		this->renderer->Destroy();
	}

	void EffectClass::SetMatrix(CameraClass* viewCamera) {
		this->viewCamera = viewCamera;
	}

	void EffectClass::Run() {
		this->manager->Update();
	}
	void EffectClass::Draw() {
		Effekseer::Matrix44 view;
		Effekseer::Matrix44 projection;

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				view.Values[y][x] = this->viewCamera->GetViewMatrix()(x, y);
				projection.Values[y][x] = this->viewCamera->GetProjectionMatrix()(x, y);
			}
		}
		this->renderer->SetCameraMatrix(view);
		this->renderer->SetProjectionMatrix(projection);

		glEnable(GL_DEPTH_TEST);
		this->renderer->BeginRendering();
		this->manager->Draw();
		this->renderer->EndRendering();
	}

	bool EffectClass::AddEffectSource(const std::string& effectName, const char* filePath) {
		//重複は許さない
		if (this->effect.find(effectName) != this->effect.end()) {
			printf("EffectName has existed：%s\n", filePath);
			return false;
		}

		EFK_CHAR path[64];
		Effekseer::ConvertUtf8ToUtf16((int16_t*)path, 64, (const int8_t*)filePath);
		Effekseer::Effect* efk = Effekseer::Effect::Create(this->manager, path);
		//失敗も許さない
		if (efk == nullptr) {
			printf("Effect Load Failed：%s\n", filePath);
			return false;
		}
		//許されたものはリストへ登録
		this->effect[effectName] = efk;
		return true;
	}

	void EffectClass::DeleteEffectSource(const std::string& effectName) {
		//リストにその名前があってインスタンスもあるものが対象
		if (this->effect.find(effectName) == this->effect.end() || !this->effect[effectName]) {
			return;
		}
		this->effect[effectName]->Release();
		this->effect.erase(effectName);
	}

	EffectHandle EffectClass::Play(const std::string& name, float posX, float posY, float posZ) {
		//リストにその名前があってインスタンスもあるものが対象
		if (this->effect.find(name) == this->effect.end() || !this->effect[name]) {
			return -1;
		}

		return this->manager->Play(this->effect[name], posX, posY, posZ);
	}

	void EffectClass::Stop(EffectHandle handle) {
		this->manager->StopEffect(handle);
	}

	void EffectClass::StopRoot(EffectHandle handle) {
		this->manager->StopRoot(handle);
	}
}
