#pragma once
#include"TextureList.h"
#include"ShaderList.h"
#include"LightList.h"
#include"CameraList.h"
#include"InputGLFW.h"
#include"BulletPhysics.h"
#include"FontRenderer.h"

#include"K_Audio\SoundClass.h"
#include"EffectClass.h"


//ゲームシステムパラメーター
//入力や物理、テクスチャへのアクセスなどを行うクラスへのポインタを渡してくれる
class GameParameters {
public:
	GameParameters(GLFWwindow* window, int screenWidth, int screenHeight);
	~GameParameters();
	bool Initialize(GLFWwindow* window, int screenWidth, int screenHeight);
	void Finalize();
	void Run();

	void UseAmbient(const std::string& name);
	void UseDirectional(const std::string& name);
	void UseShader(const std::string& name);
	void UseCamera(const std::string& name);


	K_Input::InputGLFW*        GetInput();
	K_Physics::BulletPhysics*  GetPhysics();
	K_Graphics::TextureList*   GetTextureList();
	K_Graphics::ShaderList*    GetShaderList();
	K_Graphics::LightList*     GetLightList();
	K_Graphics::CameraList*    GetCameraList();
	K_Audio::SoundClass*       GetAudioList();
	K_Graphics::EffectClass*   GetEffects();
	K_Graphics::FontRenderer*  GetFontRenderer();

private:
	K_Input::InputGLFW *       input;
	K_Physics::BulletPhysics*  physicsSystem;
	K_Graphics::TextureList*   textureList;
	K_Graphics::ShaderList*    shaderList;
	K_Graphics::LightList*     lightList;
	K_Graphics::CameraList*    cameraList;
	K_Audio::SoundClass*       audioPlayer;
	K_Graphics::EffectClass*   effectSystem;
	K_Graphics::FontRenderer*  fontRenderer;

public:
	K_Graphics::CameraClass*   currentCamera;
	K_Graphics::ShaderClass*   currentShader;
	int windowWidth;
	int windowHeight;
};