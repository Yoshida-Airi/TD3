#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;

	for (Enemy* enemys : enemy_) {
		delete enemys;
	}
	
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

	
	//triangle.reset(Triangle::Create(uvTexture));
	//triangle->SetisInvisible(true);
	//

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

}

void GamePlayScene::Update()
{
	input->TriggerKey(DIK_0);

#ifdef _DEBUG
	
	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	//triangle->Update();
	//triangle->worldTransform_->rotation_.y += 0.03f;

	//sprite->worldTransform_->translation_ = { 700.0f };
	//sprite->Update();
	//sprite->Debug("Doll");

	//sphere->Update();
	//sphere->worldTransform_->rotation_.y += 0.01f;

	//model->ModelDebug("plane");
	//model->Update();
	//model->worldTransform_->translation_.x = 3.0f;

	//particle->Debug("circleParticle");
	//particle->Update();
	
	player->Update();

	for (Enemy* enemys : enemy_) {
		enemys->Update();
	}

	EnemySporn();

	enemy_.remove_if([](Enemy* enemys) {
		if (enemys->GetIsDead()) {
			delete enemys;
			return true;
		}
		return false;
	});

}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//sphere->Draw(camera);
	//model->Draw(camera);
	//sprite->Draw(camera);
	//particle->Draw(camera);


	player->Draw(camera);

	for (Enemy* enemys : enemy_) {
		enemys->Draw(camera);
	}

}

void GamePlayScene::EnemySporn() {

	if (enemyCount <= 10) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize();

		std::mt19937 random(generator());

		newEnemy->SetTranslate(random);

		enemy_.push_back(newEnemy);
		enemyCount++;
	}
	else if (enemyCount > 10) {
		enemySpornTimer++;
		if (enemySpornTimer >= 180) {
			enemyCount = 0;
			enemySpornTimer = 0;
		}
	}
}
