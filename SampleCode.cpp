#include"SystemClass.h"
#include"K_Physics\BulletPhysics.h"
#include"K_Graphics\MeshModel.h"
#include"K_Graphics\ShaderList.h"
#include"K_Graphics\CameraList.h"
#include"K_Graphics\LightList.h"
#include"K_Graphics\FrameBufferList.h"

//2018/5/3 現在のサンプルコード


//サンプルコード１：初期化
void Sample1() {
	//フルスクリーンの場合は、ウィンドウサイズが解像度になるので、モニタに合う比率でないと何も映らない
	int windowWidth = 1280;
	int windowHeight = 720;
	bool isFullScreen = false;
	K_System::SystemClass* system = new K_System::SystemClass("hogehogeWindowName", windowWidth, windowHeight, isFullScreen);

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
	//引数の後ろの方にある二つの数字は「ビットマスク」という衝突する対象をビット演算でフィルタリングするもの
	//
	//例：
	//Collision(A)「myselfMask:(0010) giveMask:(1100)」
	//Collision(B)「myselfMask:(0100) giveMask:(0000)」
	//
	//(A)と(B)の衝突は
	//
	//(A)は(B)に衝突しているか　→　「(A)myselfMask(0010) & (B)giveMask(0000) = (0000)」　なので「衝突していない」
	//(B)は(A)に衝突しているか　→　「(B)myselfMask(0100) & (A)giveMask(1100) = (0100)」　なので「衝突している」

	K_Physics::RigidBodyData* rigid = physics->CreateRigidBody(shape, 1.0f, false, 1, 1);
	K_Physics::CollisionData* collision = physics->CreateCollisionObject(shape, false, 1, 1);

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

	//デストラクタですべて解放されるが、このように明示的にコリジョンを消すこともできる
	//当然ながら消した後は使わないこと
	physics->RemoveCollision(&collision);
	//形状情報も同様
	physics->RemoveCollisionShape(&shape);

	delete physics;
}

//サンプルコード４：入力関連
void Sample4() {
	//まずはSystemClassを作る
	int windowWidth = 1280;
	int windowHeight = 720;
	bool isFullScreen = false;
	K_System::SystemClass* system = new K_System::SystemClass("hogehogeWindowName", windowWidth, windowHeight, isFullScreen);

	//システムから入力クラスを受け取る
	K_Input::InputClass* input = system->GetInput();

	//コントローラーを作る（複数可）
	input->AddGamePad(K_Input::VpadIndex::Pad0);

	//キーコンフィグをすることで入力を受けることができるようになる
	//Pad0 の Aボタン に 
	//・ジョイスティックのボタン0番目 
	//・キーボード割り当てなし 
	//を設定
	input->GetPad(K_Input::VpadIndex::Pad0)->SetButtonConfig(K_Input::VpadButton::A, K_Input::JoyButton::Button0, K_Input::Key::Empty);

	//Pad0 の ０番目のアナログ軸 に 
	//・ジョイスティックの0番目のアナログ軸 
	//・正の方向にキーボードのA 
	//・負の方向にキーボードのD 
	//を設定
	input->GetPad(K_Input::VpadIndex::Pad1)->SetAxisConfig(K_Input::VpadAxis::Axis0, K_Input::JoyAxis::Axis0, K_Input::Key::A, K_Input::Key::D);

	//仮想ゲームパッドの軸二つを割り当ててスティックを作る。GetStickPower()やGetStickRotation()を受け取るのに必要
	//初期設定では
	//SetStickConfig(VpadStick::L, VpadAxis::Axis0, VpadAxis::Axis1);
	//SetStickConfig(VpadStick::R, VpadAxis::Axis2, VpadAxis::Axis3);
	//ここでは Pad0 の 左スティック に
	//・x軸 として 仮想ゲームパッドの２番目の軸
	//・y軸 として 仮想ゲームパッドの３番目の軸
	//を設定
	input->GetPad(K_Input::VpadIndex::Pad0)->SetStickConfig(K_Input::VpadStick::L, K_Input::VpadAxis::Axis2, K_Input::VpadAxis::Axis3);

	//上から、Aボタンが「押された瞬間」「押している間」「離された瞬間」という意味。ちなみにbool型
	input->GetPad(K_Input::VpadIndex::Pad0)->IsPressButton(K_Input::VpadButton::A);
	input->GetPad(K_Input::VpadIndex::Pad0)->IsStayButton(K_Input::VpadButton::A);
	input->GetPad(K_Input::VpadIndex::Pad0)->IsReaveButton(K_Input::VpadButton::A);

	//上から、左スティックの「倒されている深さ」「右方向を０度とした回転度」を取得できる
	input->GetPad(K_Input::VpadIndex::Pad0)->GetStickPower(K_Input::VpadStick::L);
	input->GetPad(K_Input::VpadIndex::Pad0)->GetStickRotation(K_Input::VpadStick::L);

	//個別の軸についてはこれ(-1.0f - 1.0f の範囲)
	input->GetPad(K_Input::VpadIndex::Pad0)->GetAxisPosition(K_Input::VpadAxis::Axis0);

	//マウスに関しては複数作れないのでGetPad()は必要ない
	input->GetMousePosition();
	//使い方は大体ボタンと同じ
	input->IsPressMouse(K_Input::VMouse::Left);
	input->IsStayMouse(K_Input::VMouse::Left);
	input->IsReaveMouse(K_Input::VMouse::Left);
}

//サンプルコード５：描画メッシュモデル作成（FBX読み込み）(サンプルコード１でもFBXは読み込んでいる)
void Sample5() {
	//メッシュモデルクラスの初期化にはtextureListクラスが必要
	K_Graphics::TextureList* textureList = new TextureList;
	
	
	//手順一つ目、モデルのメッシュやマテリアルの情報を指すModelDataクラスの作成
	K_Graphics::ModelDatas* modelData;
	//ファクトリに作ってもらう
	K_Graphics::ModelDataFactory factory;
	modelData = factory.LoadFBXModel("filePath", textureList);
	
	//手順二つ目、データをもとにモデルクラスを初期化
	K_Graphics::MeshModel* model = new K_Graphics::MeshModel(modelData);
	
	//手順三つ目、MeshModelをより扱いやすくするMeshObjectクラスを初期化
	K_Graphics::MeshObject* object = new K_Graphics::MeshObject(model);
	
	
	//ちなみに描画はこんな感じ（疑似コード）
	//object->Draw(camera, shader, position, rotation, scale);
	
	
	
	//解放はobjectのみでいい
	delete object;
	delete textureList;
	
	
	////////
	//おまけ
	////
	
	//こうも書ける、面倒なら関数化するのも手
	K_Graphics::ModelDataFactory factory;
	K_Graphics::MeshObject* obj = new K_Graphics::MeshObject(new K_Graphics::MeshModel(factory.LoadFBXModel("filePath", texture)));
	delete obj;
	
	//実を言わなくても描画だけならMeshModelクラスのみで可能、ただし行列とシェーダーを扱う必要あり
	model = new K_Graphics::MeshModel(factory.LoadFBXModel("filePath", texture));
	
	//こんな風にシェーダークラスから行列を渡す（疑似コード）
	//shader->SetMatrix(matrixViewProjectionWorld);
	//shader->SetWorldMatrix(matrixWorld);
	
	model->Draw();
	
	//MeshObjectを扱わない場合はMeshModelの解放が必要（ModelDatasクラスの解放は必要ない）
	delete model;
}

//サンプルコード６：FBXから物理衝突用のコリジョンを作成する
void Sample6(){
	////////
	//初期化は省略
	////
	K_Physics::BulletPhysics* physics = new K_Physics::BulletPhysics;
	
	
	//メッシュの作成にはMapPolygonクラスを使用する
	K_Physics::MapPolygon* collision;
	
	//コンストラクタで作成、ここで作られるのはコリジョンではなく剛体（特に気にする必要はない）
	collision = new K_Physics::MapPolygon("filePath", physics, myselfMask, giveMask);
	
	//作成した剛体はGetRigidBody()でアクセスできる、扱いはほかのコリジョンと同じ
	collision->GetRigidBody();
	
	//FBXは軸の向きが自由なので見た目に合わないことがある
	//そんな時は回転するといい
	collision->GetRigidBody()->SetCollisionRotation(K_Math::Vector3(K_Math::DegToRad(-90.0f), 0.0f, 0.0f));
	//MapPolygonクラスに限り拡縮も可能
	collision->SetScaling(scale);
	
	//ほかのコリジョンと違ってこのクラスには解放処理が必要であることに注意
	delete collision;
}
