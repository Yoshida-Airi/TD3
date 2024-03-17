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

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

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

	playerWeapon_ = std::make_unique<PlayerWeapon>();
	playerWeapon_->Initialize();

	sword = std::make_unique<Sword>();
	sword->Initialize();


	player->SetWeapon(playerWeapon_.get());
	player->SetCamera(camera);

	//colliderManager_->UpdateWorldTransform();

	//// カメラの初期位置からプレイヤーの位置へのベクトルを計算
	//offset = Subtract(camera->transform.translate, player->GetWorldPosition());


	//

	followCamera_->SetTarget(player->GetWorldTransform());

}

void GamePlayScene::Update()
{



	if (playerlevel->nowlevel == playerlevel->count) {
		player->PLevelUp();
		playerlevel->count += 1;
	}

	//camera->transform.translate = Add(player->GetWorldPosition(), offset);
	camera->UpdateMatrix();

	followCamera_->Update();
	camera->matView = followCamera_->GetCamera()->matView;
	camera->matProjection = followCamera_->GetCamera()->matProjection;
	camera->TransferMatrix();
	//camera->UpdateMatrix();

	if (timer.GetNowSecond() != 120)
	{
		sprite->worldTransform_->translation_ =
		{
			sprite->worldTransform_->translation_.x - 20.0f,sprite->worldTransform_->translation_.y,sprite->worldTransform_->translation_.z
		};
		timer.AddNowFrame();
		timer.AddNowWaveFrame();

		//敵の処理
		EnemyAttack();

		if (isEnemySpawn == true) {
			if (enemyCount <= 4) {
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
		if (timer.GetNowWaveSecond() == 20 || input->TriggerKey(DIK_SPACE))//TriggerKey->敵の数を参照して０になったらリセットに変更
		{
			timer.AddNowWaveSecond();
			timer.ResetNowWaveFrame();

			isEnemySpawn = true;
			enemyCount = 0;

			sprite->worldTransform_->translation_ =
			{
				1280.0f,300.0f,0.0f
			};
		}
	}
	else if (timer.GetNowSecond() >= 120)
	{
		timer.AddBossFrame();
		if (timer.GetBossFrame() == 60)
		{
			timer.AddBossSecond();
			timer.ResetBossFrame();
		}
	}
	//ImGui::Begin("Frame&Seconds");
	//ImGui::DragInt("nowFrame", (int*)timer.GetNowFrame());
	//ImGui::DragInt("nowWaveFrame", (int*)timer.GetNowWaveFrame());
	//ImGui::DragInt("nowSecond", (int*)timer.GetNowSecond());
	//ImGui::DragInt("nowWaveSecond", (int*)timer.GetNowWaveSecond());
	//ImGui::DragInt("bossFrame", (int*)timer.GetBossFrame());
	//ImGui::DragInt("bossSecond", (int*)timer.GetBossSecond());
	//ImGui::End();

#ifdef _DEBUG

	camera->CameraDebug();
	followCamera_->CameraDebug();

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

	playerWeapon_->SetPosition(weaponPos);
	playerWeapon_->Update();
	

	sword->GetWorldTransform()->parent_ = player->GetWorldTransform();


	//sampleEnemy->Update();
	playerWeapon_->Update();
	//sampleEnemy->Update();

	
}

void GamePlayScene::Draw()
{
	demo_stage->Draw(camera);
	player->Draw();
	sword->Draw(camera);

	if (player->GetIsUnderAttack())
	{
		playerWeapon_->Draw(camera);
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

			////向いている方向にダッシュに変更予定。今はｚにダッシュのみ
			//player->model_->worldTransform_->translation_.z += 0.5f;
			//sword->model_->worldTransform_->translation_.z += 0.5f;
			////camera->transform.translate.z += 0.5f;
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
				colliderManager_->AddColliders(playerWeapon_.get());
			}
			colliderManager_->AddColliders(enemyBullets);
			colliderManager_->AddColliders(enemys);
			//colliderManager_->AddColliders(sampleEnemy.get());

			//当たり判定
			colliderManager_->ChackAllCollisions();
		}
	}
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

