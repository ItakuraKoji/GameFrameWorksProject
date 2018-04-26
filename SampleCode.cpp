#include"SystemClass.h"
#include"K_Physics\BulletPhysics.h"
#include"K_Graphics\MeshModel.h"
#include"K_Graphics\ShaderList.h"
#include"K_Graphics\CameraList.h"
#include"K_Graphics\LightList.h"
#include"K_Graphics\FrameBufferList.h"

//2018/4/26 現在のサンプルコード


//サンプルコード１：初期化
void Sample1() {
	//フルスクリーンの場合は、ウィンドウサイズが解像度になるので、モニタに合う比率でないと何も映らない
	int windowWidth = 1280;
	int windowHeight = 720;
	bool isFullScreen = false;
	K_System::SystemClass* system = new K_System::SystemClass(windowWidth, windowHeight, isFullScreen);

	//メインループ
	while (!system->IsSystemEnd()) {
		system->ProcessSystem();
		system->SwapBuffer();
	}

	//この関数で Frame Per Second を取得できる
	system->GetFPS();

	delete system;
}


//サンプルコード２：描画
void Sample2() {
	//必要なのは「フレームバッファ」「シェーダー」「カメラ」「テクスチャ」「描画するモデルオブジェクト」

	K_Graphics::ShaderList* shader = new K_Graphics::ShaderList;
	K_Graphics::CameraList* camera = new K_Graphics::CameraList;
	K_Graphics::TextureList* texture = new K_Graphics::TextureList;
	K_Graphics::FrameBufferList* frame = new K_Graphics::FrameBufferList(texture);

	//頂点シェーダー・フラグメントシェーダーの二つを用意
	shader->LoadVertexShader("fileName.vs");
	shader->LoadFragmentShader("fileName.fs");
	//読み込み済みのシェーダーのパスを引数に渡す
	shader->CreateShaderProgram("shaderHogeHoge", "fileName.vs", "fileName.ps");

	//カメラのscreenサイズは、描画の画面サイズのイメージでいい
	K_Math::Vector3 position = K_Math::Vector3(0.0f, 0.0f, -10.0f);
	K_Math::Vector3 terget = K_Math::Vector3(0.0f, 0.0f, 0.0f);
	int screenWidth = 1000;
	int screenHeight = 1000;
	float clipNear = 0.1f;
	float clipFar = 1000.0f;
	float fieldOfView = 45.0f;
	//カメラは平行投影と射影投影の２種類ある
	camera->AddPerspectiveCamera("cameraHogeHoge1", position, terget, screenWidth, screenHeight, clipNear, clipFar, fieldOfView);
	camera->AddOrthoCamera("cameraHogeHoge2", position, terget, screenWidth, screenHeight, clipNear, clipFar);

	//ユーザー側でテクスチャを読み込む場合はこう書く
	texture->LoadTexture("HogeHogeTexture", "fileName.tga");

	//描画するものを作るのはModelDataFactoryクラス
	//テクスチャを扱うためにTextureListクラスが生成に必要となる
	K_Graphics::ModelDataFactory factory;
	K_Graphics::MeshModel* model = new K_Graphics::MeshModel(factory.LoadFBXModel("fileName.fbx", texture));
	K_Graphics::MeshObject* obj = new K_Graphics::MeshObject(model);

	{
		//オレンジ色にクリアして描画
		int viewPortWidth = 1000;
		int viewPortHeight = 1000;
		float colorR = 1.0f;
		float colorG = 0.5f;
		float colorB = 0.0f;
		frame->BeginDraw(viewPortWidth, viewPortHeight, colorR, colorG, colorB);

		//カメラのDraw()は行列を更新する処理
		K_Graphics::CameraClass* usedCamera = camera->GetCamera("cameraHogeHoge1");
		K_Graphics::ShaderClass* usedShader = shader->GetShader("shaderHogeHoge");

		usedCamera->Draw();
		usedShader->UseShader();

		K_Math::Vector3 position = K_Math::Vector3(0.0f, 0.0f, 0.0f);
		K_Math::Vector3 rotation = K_Math::Vector3(0.0f, 0.0f, 0.0f);
		K_Math::Vector3 scale = K_Math::Vector3(1.0f, 1.0f, 1.0f);
		obj->Draw(usedCamera, usedShader, position, rotation, scale);
	}


	//ちなみにライトはこう使う
	K_Graphics::LightList* light = new K_Graphics::LightList;
	light->AddAmbient("ambientLight1", 1.0f, K_Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	light->AddDirectional("directionalLight1", 1.0f, K_Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f), K_Math::Vector3(1.0f, 0.0f, 1.0f));

	//この関数でシェーダーにセット、どんな名前のUniform変数に情報が行くかは「シェーダークラスのルール」を参照
	light->SetAmbient("ambientLight1", shader->GetShader("shaderHogeHoge"));
	light->SetDirectional("directionalLight1", shader->GetShader("shaderHogeHoge"));

	delete shader;
	delete camera;
	delete frame;
	delete texture;
	delete light;
}

//サンプルコード３：物理衝突シミュレーション
void Sample3() {
	//物理を扱うクラスはBulletPhysics
	K_Physics::BulletPhysics* physics = new K_Physics::BulletPhysics;

	//幅4.0f 高さ4.0f 奥行4.0f の直方体コリジョンの「形」を生成
	btCollisionShape* shape = physics->CreateBoxShape(2.0f, 2.0f, 2.0f);
	//生成した「形」でコリジョンや剛体をつくる(剛体には質量の情報が必要)
	K_Physics::RigidBodyData* rigid     = physics->CreateRigidBody(shape, 1.0f, false, 1);
	K_Physics::CollisionData* collision = physics->CreateCollisionObject(shape, false, 1);

	//コリジョンには情報を持たせることができる
	K_Math::Vector3 data = K_Math::Vector3(1.0f, 0.0f, 1.0f);
	rigid->tag.tagIndex = 10;
	rigid->tag.tagName = "hogehoge";
	rigid->tag.userData = &data;

	//剛体には力を加えることができる
	rigid->AddForce(K_Math::Vector3(100.0f, 0.0f, 0.0f));

	//キャラクターの移動(MoveCharacterDiscrete は MoveCharacter よりも処理が軽い)
	//壁に当たるとそのまま
	physics->MoveCharacter(collision, K_Math::Vector3(0.0f, 0.0f, 1.0f));
	physics->MoveCharacterDiscrete(rigid, K_Math::Vector3(0.0f, 0.0f, 1.0f));

	//衝突しているオブジェクトを取得する
	std::vector<K_Physics::CollisionTag*> tags = physics->FindConfrictionObjects(collision);

	//この例ではrigidと衝突しているので、rigid付属の情報を得られる
	tags[0]->tagIndex;                  //10
	tags[0]->tagName;                   //"hogehoge"
	(K_Math::Vector3*)tags[0]->userData;//K_Maeh::Vector3 data のアドレス値


	//Run()によって物理シミュレーションを進める（物理によって剛体が動く）
	physics->Run();

	delete physics;
}