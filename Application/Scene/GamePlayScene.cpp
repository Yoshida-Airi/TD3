#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	
}

void GamePlayScene::Initialize()
{
	textureManager_ = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG


	uvTexture = textureManager_->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	monsterBall = textureManager_->LoadTexture("Resources/DefaultAssets/monsterBall.png");
	Doll = textureManager_->LoadTexture("Resources/DefaultAssets/Doll.png");
	circle = textureManager_->LoadTexture("Resources/DefaultAssets/circle.png");

	camera = new Camera;
	camera->Initialize();

	


	triangle.reset(Triangle::Create(uvTexture));
	triangle2.reset(Triangle::Create(monsterBall));
	
	triangle->SetisInvisible(true);
	triangle2->SetisInvisible(true);

	//
	sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });
	
	//sprite->SetisInvisible(true);


	sprite2.reset(Sprite::Create(uvTexture));
	sprite2->SetSize({ 64.0f, 64.0f });
	sprite2->SetTextureLeftTop({ 0,0 });
	//sprite2->SetisInvisible(true);

	sphere.reset(Sphere::Create(monsterBall));
	sphere->worldTransform_->translation_.y = -1.0f;
	//sphere->SetisInvisible(true);

	model.reset(Model::Create("Resources/DefaultAssets/plane.gltf"));
	model2.reset(Model::Create("Resources/DefaultAssets/terrain.obj"));

	model->worldTransform_->rotation_.y = 3.14f;
	model2->worldTransform_->rotation_.y = 3.14f;

	model2->worldTransform_->translation_ =
	{
		0.0f,-1.5,0.0f
	};

	//model->SetisInvisible(true);
	//model2->SetisInvisible(true);


	particle.reset(ParticleSystem::Create(circle));
	particle->emitter_->count = 100;
	//particle->SetisInvisible(true);

	particle2.reset(ParticleSystem::Create(uvTexture));
	//particle2->SetisInvisible(true);
}

void GamePlayScene::Update()
{
	input->TriggerKey(DIK_0);

	if(nowSecond!=120)
	{
		nowFrame++;
		nowWaveFrame++;
		if (nowFrame == 60) 
		{
			nowSecond++;
			nowFrame = 0;
		}
		if (nowWaveFrame == 60) 
		{
			nowWaveSecond++;
			nowWaveFrame = 0;
		}
		if (nowWaveSecond == 20 || input->TriggerKey(DIK_SPACE))//TriggerKey->敵の数を参照して０になったらリセットに変更
		{
			nowWaveSecond = 0;
			nowWaveFrame = 0;
		}
	}
	else if(nowSecond >= 120)
	{
		bossFrame++;
		if (bossFrame == 60)
		{
			bossSecond++;
			bossFrame = 0;
		}
	}
	ImGui::Begin("Frame&Seconds");
	ImGui::DragInt("nowFrame", (int*)&nowFrame);
	ImGui::DragInt("nowWaveFrame", (int*)&nowWaveFrame);
	ImGui::DragInt("nowSecond", (int*)&nowSecond);
	ImGui::DragInt("nowWaveSecond", (int*)&nowWaveSecond);
	ImGui::DragInt("bossFrame", (int*)&bossFrame);
	ImGui::DragInt("bossSecond", (int*)&bossSecond);
	ImGui::End();

#ifdef _DEBUG
	
	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	triangle->Update();
	triangle->worldTransform_->rotation_.y += 0.03f;

	triangle2->Update();
	triangle2->worldTransform_->scale_.y = 0.5f;
	triangle2->worldTransform_->rotation_.y += 0.02f;

	sprite->worldTransform_->translation_ = { 700.0f };

	sprite->Update();
	sprite2->Update();

	sprite->Debug("Doll");
	sprite2->Debug("uv");

	sphere->Update();
	sphere->worldTransform_->rotation_.y += 0.01f;

	model->ModelDebug("plane");
	model2->ModelDebug("plane2");

	model->Update();
	model2->Update();
	model->worldTransform_->translation_.x = 3.0f;

	particle->Debug("circleParticle");
	particle2->Debug("uvTextureParticle");

	particle->Update();
	particle2->Update();
}

void GamePlayScene::Draw()
{

	triangle->Draw(camera);
	triangle2->Draw(camera);

	

	sphere->Draw(camera);


	model->Draw(camera);
	model2->Draw(camera);


	sprite->Draw(camera);
	sprite2->Draw(camera);

	particle->Draw(camera);
	particle2->Draw(camera);

}
