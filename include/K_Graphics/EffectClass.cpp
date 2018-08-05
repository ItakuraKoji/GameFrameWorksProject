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

	void EffectClass::Run(float timeSpeed) {
		this->manager->Update(timeSpeed);
	}
	void EffectClass::Draw() {
		if (this->viewCamera == nullptr) {
			return;
		}

		Effekseer::Matrix44 view;
		Effekseer::Matrix44 projection;

		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				view.Values[y][x] = this->viewCamera->GetViewMatrix()[y][x];
				projection.Values[y][x] = this->viewCamera->GetProjectionMatrix()[y][x];
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
			throw std::runtime_error("EffectName has already existed ： " + effectName);
		}

		EFK_CHAR path[64];
		Effekseer::ConvertUtf8ToUtf16((int16_t*)path, 64, (const int8_t*)filePath);
		Effekseer::Effect* efk = Effekseer::Effect::Create(this->manager, path);
		//失敗も許さない
		if (efk == nullptr) {
			throw std::runtime_error("Effect Load Failed ： " + std::string(filePath));
		}
		//許されたものはリストへ登録
		this->effect[effectName] = efk;
		return true;
	}

	void EffectClass::DeleteEffectSource(const std::string& effectName) {
		//リストにその名前があってインスタンスもあるものが対象
		if (this->effect.find(effectName) == this->effect.end() || !this->effect[effectName]) {
			throw std::runtime_error("EffectName is not exists ： " + effectName);
		}
		this->effect[effectName]->Release();
		this->effect.erase(effectName);
	}

	EffectHandle EffectClass::Play(const std::string& name, const K_Math::Vector3& position, float speed) {
		//リストにその名前があってインスタンスもあるものが対象
		if (this->effect.find(name) == this->effect.end()) {
			throw std::runtime_error("EffectName is not exists ： " + name);
		}
		EffectHandle handle = this->manager->Play(this->effect[name], position.x, position.y, position.z);
		this->manager->SetSpeed(handle, speed);
		return handle;
	}

	void EffectClass::Stop(EffectHandle handle) {
		this->manager->StopEffect(handle);
	}

	void EffectClass::StopRoot(EffectHandle handle) {
		this->manager->StopRoot(handle);
	}

	void EffectClass::SetPosition(EffectHandle handle, const K_Math::Vector3& position) {
		this->manager->SetLocation(handle, position.x, position.y, position.z);
	}
	void EffectClass::SetRotation(EffectHandle handle, float rotation, const K_Math::Vector3& axis) {
		this->manager->SetRotation(handle, Effekseer::Vector3D(axis.x, axis.y, axis.z), rotation);
	}
	void EffectClass::SetScale(EffectHandle handle, const K_Math::Vector3& scale) {
		this->manager->SetScale(handle, scale.x, scale.y, scale.z);
	}
	void EffectClass::SetSpeed(EffectHandle handle, float speed) {
		this->manager->SetSpeed(handle, speed);
	}

}
