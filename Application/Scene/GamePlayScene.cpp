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

	//当たり判定処理の設定
	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();

#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG


	uvTexture = textureManager_->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	monsterBall = textureManager_->LoadTexture("Resources/DefaultAssets/monsterBall.png");
	Doll = textureManager_->LoadTexture("Resources/DefaultAssets/Doll.png");
	circle = textureManager_->LoadTexture("Resources/DefaultAssets/circle.png");

	camera = new Camera;
	camera->Initialize();

	
	//triangle.reset(Triangle::Create(uvTexture));
	//triangle->SetisInvisible(true);
	//

	sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });

	//sprite->SetisInvisible(true);


	//sprite.reset(Sprite::Create(Doll));
	//sprite->SetSize({ 64.0f, 64.0f });
	//sprite->SetTextureLeftTop({ 0,0 });

	//sphere.reset(Sphere::Create(monsterBall));
	//sphere->worldTransform_->translation_.y = -1.0f;

	//model.reset(Model::Create("Resources/DefaultAssets/plane.gltf"));
	//model->worldTransform_->rotation_.y = 3.14f;

	

	//particle.reset(ParticleSystem::Create(circle));
	//particle->emitter_->count = 100;
	////particle->SetisInvisible(true);

	player = std::make_unique<Player>();
	player->Initialize();

	playerWeapon_ = std::make_unique<PlayerWeapon>();
	playerWeapon_->Initialize();

	sampleEnemy = std::make_unique<PlayerWeapon>();
	sampleEnemy->Initialize();

	player->SetWeapon(playerWeapon_.get());

	//colliderManager_->UpdateWorldTransform();
	
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


	CheckAllCollisions();


	//triangle->Update();
	//triangle->worldTransform_->rotation_.y += 0.03f;

	//sprite->worldTransform_->translation_ = { 700.0f };
	//sprite->Update();
	//sprite->Debug("Doll");

	//sphere->Update();
	//sphere->worldTransform_->rotation_.y += 0.01f;

	/*model->ModelDebug("plane");
	model->Update();
	model->worldTransform_->translation_.x = 3.0f;*/

	//particle->Debug("circleParticle");
	//particle->Update();
	
	player->Update();

	Vector3 weaponPos = player->GetPosition();

	weaponPos.z = weaponPos.z + 5.0f;

	playerWeapon_->SetPosition(weaponPos);


	//sampleEnemy->Update();
	playerWeapon_->Update();
	sampleEnemy->Update();

}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//sphere->Draw(camera);
	//model->Draw(camera);
	//sprite->Draw(camera);
	//particle->Draw(camera);


	player->Draw(camera);

	if (player->GetIsUnderAttack())
	{
		playerWeapon_->Draw(camera);
	}

	sampleEnemy->Draw(camera);

	//colliderManager_->Draw(camera);
}

void GamePlayScene::CheckAllCollisions()
{
	//コライダーのリストをクリア
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player.get());
	colliderManager_->AddColliders(playerWeapon_.get());
	colliderManager_->AddColliders(sampleEnemy.get());

	//当たり判定
	colliderManager_->ChackAllCollisions();
}
