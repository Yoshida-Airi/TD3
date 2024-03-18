#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{
	delete camera;
	delete playerlevel;

	for (Enemy* enemys : enemy_) {
		delete enemys;
	}

	for (EnemyBullet* enemyBullets : enemyBullet_) {
		delete enemyBullets;
	}

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
	demo_stage.reset(Model::Create("Resources/DefaultAssets/demo_stage.obj"));

	camera = new Camera;
	camera->Initialize();

	timer.Initialize();


	

	playerlevel = new Playerlevel;
	playerlevel->Initialize();


	sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });

	

	player = std::make_unique<Player>();
	player->Initialize(camera);

	sword = std::make_unique<Sword>();
	sword->Initialize();

	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

	//sampleEnemy = std::make_unique<PlayerWeapon>();
	//sampleEnemy->Initialize();

	player->SetWeapon(sword.get());
	
	//colliderManager_->UpdateWorldTransform();

}

void GamePlayScene::Update()
{
	if (input->PushKey(DIK_W))
	{
		camera->transform.translate.z += 0.03f;
	}
	if (input->PushKey(DIK_S))
	{
		camera->transform.translate.z -= 0.03f;
	}
	if (input->PushKey(DIK_A))
	{
		camera->transform.translate.x -= 0.03f;
	}
	if (input->PushKey(DIK_D))
	{
		camera->transform.translate.x += 0.03f;
	}

	camera->UpdateMatrix();

	if (playerlevel->nowlevel == playerlevel->count) {
		player->PLevelUp();
		playerlevel->count += 1;
	}
	

	if (timer.GetNowSecond() != 120)
	{
		//sprite->worldTransform_->translation_ =
		//{
		//	sprite->worldTransform_->translation_.x - 20.0f,sprite->worldTransform_->translation_.y,sprite->worldTransform_->translation_.z
		//};
		timer.AddNowFrame();
		timer.AddNowWaveFrame();

		//敵の処理
		EnemyAttack();

		if (isEnemySpawn == true) {
			if (enemyCount <= MaxEnemySpawn) {
				EnemySpawn();
				enemyCount++;
			}
			else {
				isEnemySpawn = false;
			}
		}

		for (Enemy* enemys : enemy_) {
			enemys->Update();
			if (enemys->GetIsDead()) {
				//貰える経験値
				playerlevel->Experiencepoint += 55.0f;
				enemyDeathCount++;
			}
		}

		enemy_.remove_if([](Enemy* enemys) {
			if (enemys->GetIsDead()) {
				delete enemys;
				return true;
			}
			return false;
			});
		//ここから敵の弾の処理
		for (EnemyBullet* enemyBullets : enemyBullet_) {
			enemyBullets->Update();
		}

		enemyBullet_.remove_if([](EnemyBullet* enemyBullets) {
			if (enemyBullets->GetIsDead()) {
				delete enemyBullets;
				return true;
			}
			return false;
			});

		CheckAllCollisions();

		if (timer.GetNowFrame() == 60)
		{
			timer.AddNowSecond();
			timer.ResetNowFrame();
		}
		if (timer.GetNowWaveFrame() == 60)
		{
			timer.AddNowWaveSecond();
			timer.ResetNowWaveFrame();
		}
		if (timer.GetNowWaveSecond() == 20 || enemyDeathCount == MaxEnemySpawn)
		{
			timer.ResetNowWaveSecond();
			timer.ResetNowWaveFrame();

			isEnemySpawn = true;
			enemyCount = 1;
			enemyDeathCount = 0;

			//sprite->worldTransform_->translation_ =
			//{
			//	1280.0f,300.0f,0.0f
			//};
		}
	}
	else if (timer.GetNowSecond() >= 10)
	{
		timer.AddBossFrame();

		enemy_.remove_if([](Enemy* enemys) {
			delete enemys;
			return true;
			});

		enemyBullet_.remove_if([](EnemyBullet* enemyBullets) {
			delete enemyBullets;
			return true;
			});

		boss_->Update();

		if (boss_->GetIsDead() == false) {
			isBossSpawn = true;
		}
		else {
			isBossSpawn = false;
		}

		BossSceneAllCollisions();

		if (timer.GetBossFrame() == 60)
		{
			timer.AddBossSecond();
			timer.ResetBossFrame();
		}
	}
	ImGui::Begin("Frame&Seconds");
	ImGui::Text("nowFrame : %u", timer.GetNowFrame());
	ImGui::Text("nowWaveFrame : %u", timer.GetNowWaveFrame());
	ImGui::Text("nowSecond : %u", timer.GetNowSecond());
	ImGui::Text("nowWaveSecond : %u", timer.GetNowWaveSecond());
	ImGui::Text("bossFrame : %u", timer.GetBossFrame());
	ImGui::Text("bossSecond : %u", timer.GetBossSecond());
	ImGui::Text("boss : %d", boss_->GetIsDead());
	ImGui::Text("hp: %d", boss_->GetHP());
	ImGui::End();

#ifdef _DEBUG

	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}


	CheckAllCollisions();


	demo_stage->Update();
	demo_stage->ModelDebug("demo_stage");
	playerlevel->Update();
	player->Update();
	sword->Update();

	Vector3 weaponPos = player->GetPosition();
	weaponPos.z = weaponPos.z + 5.0f;

	sword->GetWorldTransform()->parent_ = player->GetWorldTransform();


}

void GamePlayScene::Draw()
{
	demo_stage->Draw(camera);
	player->Draw();
	sword->Draw(camera);

	if (player->GetIsUnderAttack())
	{
	}

	if (player->GetIsSkill())
	{
		if (playerlevel->nowskilllevel == 1) {

			float directionAngle = player->model_->worldTransform_->rotation_.y;

			float dashSpeed = 0.5f;

			float dashX = std::sin(directionAngle) * dashSpeed;
			float dashZ = std::cos(directionAngle) * dashSpeed;

			player->model_->worldTransform_->translation_.x += dashX;
			player->model_->worldTransform_->translation_.z += dashZ;

		
			//カメラ直書き
			camera->transform.translate.x += dashX;
			camera->transform.translate.z += dashZ;
		}
	}
	
	//ここから敵の弾の処理
	for (EnemyBullet* enemyBullets : enemyBullet_) {
		enemyBullets->Draw(camera);
	}

	//ここから敵を出す処理
	for (Enemy* enemys : enemy_) {
		enemys->Draw(camera);
	}

	if (isBossSpawn == true) {
		boss_->Draw(camera);
	}

	playerlevel->Draw();



	//colliderManager_->Draw(camera);
}

void GamePlayScene::CheckAllCollisions()
{
	for (EnemyBullet* enemyBullets : enemyBullet_) {
		for (Enemy* enemys : enemy_) {
			//コライダーのリストをクリア
			colliderManager_->ListClear();

			//コライダーにオブジェクトを登録
			colliderManager_->AddColliders(player.get());
			if (player->GetIsUnderAttack() == true) {
				colliderManager_->AddColliders(sword.get());
			}
			colliderManager_->AddColliders(enemyBullets);
			colliderManager_->AddColliders(enemys);

			//当たり判定
			colliderManager_->ChackAllCollisions();
		}
	}
}

void GamePlayScene::BossSceneAllCollisions() {
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player.get());
	if (player->GetIsUnderAttack() == true) {
		colliderManager_->AddColliders(sword.get());
	}
	if (boss_->GetIsCoolDown() == false) {
		colliderManager_->AddColliders(boss_.get());
	}
	//colliderManager_->AddColliders(sampleEnemy.get());

	//当たり判定
	colliderManager_->ChackAllCollisions();
}

void GamePlayScene::EnemySpawn() {

	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize();

	std::mt19937 random(generator());

	newEnemy->SetTranslate(random, player->GetPosition());

	enemy_.push_back(newEnemy);

}

void GamePlayScene::EnemyAttack() {

	for (Enemy* enemy : enemy_) {
		if (isEnemyAttack == true) {

			isEnemyAttack = false;
			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize();
			newBullet->SetTranslate(enemy->GetTranslate());

			const float kBulletSpeed = 0.05f;
			Vector3 playerPos = player->GetPosition();
			Vector3 enemyPos = enemy->GetTranslate();
			Vector3 speed;

			speed.x = playerPos.x - enemyPos.x;
			speed.y = playerPos.y - enemyPos.y;
			speed.z = playerPos.z - enemyPos.z;

			speed = Normalize(speed);

			speed.x *= kBulletSpeed;
			speed.y *= kBulletSpeed;
			speed.z *= kBulletSpeed;

			speed = TransformNormal(speed, enemy->GetMatWorld());

			newBullet->SetSpeed(speed);

			enemyBullet_.push_back(newBullet);
		}
		else if (isEnemyAttack == false) {
			enemyAttackCoolDown++;
			if (enemyAttackCoolDown >= 60) {
				enemyAttackCoolDown = 0;
				isEnemyAttack = true;
			}
		}
	}

}

