#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{
	delete camera;

	for (Enemy* enemys : enemy_) {
		delete enemys;
	}

	for (EnemyBullet* enemyBullets : enemyBullet_) {
		delete enemyBullets;
	}

	for (DeathEffect* deathEffects : deathEffect_) {
		delete deathEffects;
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


	sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });


	player = std::make_unique<Player>();
	player->Initialize(camera);

	sword = std::make_unique<Sword>();
	sword->Initialize(player.get());

	bossBullet_ = std::make_unique<BossBullet>();
	bossBullet_->Initialize();

	boss_ = std::make_unique<Boss>();
	boss_->Initialize(player.get(), bossBullet_.get());

	player->SetWeapon(sword.get());

	//colliderManager_->UpdateWorldTransform();

	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");

	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	fadeAlpha = 0.0f;
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,fadeAlpha });

	isFadeOut = false;
	isFadeIn = false;

	StartFadeOut();

	throughTimer = 0;

}

void GamePlayScene::Update()
{

	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}

	// フェードイン中の処理
	if (isFadeIn == true)
	{
		//fadeAlpha += 0.01f;
		UpdateFadeIn();

	}

	if (timer.GetNowSecond() != kFullWaveTime)

	{
		sprite->worldTransform_->translation_ =
		{
			sprite->worldTransform_->translation_.x - 20.0f,sprite->worldTransform_->translation_.y,sprite->worldTransform_->translation_.z
		};
		timer.AddNowFrame();
		timer.AddNowWaveFrame();
		if (player->GetIsHit() != true)
		{

			//敵の処理
			EnemyAttack();
			boss_->SetTranslate(player->GetPosition());

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
					player->GetPlayerLevel()->Experiencepoint += 240.0f;
					enemyDeathCount++;
					CreateDeathEffect({ enemys->GetTranslate() });
				}
			}

			enemy_.remove_if([](Enemy* enemys) {
				if (enemys->GetIsDead()) {
					delete enemys;
					return true;
				}
				return false;
				});

			deathEffect_.remove_if([](DeathEffect* hitEffects) {
				if (hitEffects->IsDead())
				{
					//実行時間をすぎたらメモリ削除
					delete hitEffects;
					return true;
				}
				return false;
				});

			for (DeathEffect* deathEffects : deathEffect_) {
				deathEffects->Update();
			}

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
		}
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

			sprite->worldTransform_->translation_ =
			{
				1280.0f,300.0f,0.0f
			};
		}

	}
	else if (timer.GetNowSecond() >= kFullWaveTime)
	{
		timer.AddBossFrame();
		if (player->GetIsHit() != true)
		{

			enemy_.remove_if([](Enemy* enemys) {
				delete enemys;
				return true;
				});

			deathEffect_.remove_if([](DeathEffect* hitEffects) {
				delete hitEffects;
				return true;
				});

			enemyBullet_.remove_if([](EnemyBullet* enemyBullets) {
				delete enemyBullets;
				return true;
				});

			boss_->Update(sceneManager_);

			if (boss_->GetIsDead() == false) {
				isBossSpawn = true;
			}
			else {
				isBossSpawn = false;
			}

			BossSceneAllCollisions();
		}
		if (timer.GetBossFrame() == 60)
		{
			timer.AddBossSecond();
			timer.ResetBossFrame();
		}

	}

#ifdef _DEBUG
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

	camera->CameraDebug();
	camera->UpdateMatrix();

	ImGui::Text("fadeAlpha:%f", fadeAlpha);

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	if (player->GetHP() <= 0)
	{
		//sceneManager_->ChangeScene("GAMEOVER");
		StartFadeIn();
	}

	

	if (player->GetIsHit() != true)
	{
		CheckAllCollisions();


		demo_stage->Update();
		demo_stage->ModelDebug("demo_stage");

		player->Update();
		sword->Update();

		if (boss_->GetTranslate().y < 0.0f && isCameraShake == false && cameraShakeTime < 50)
		{
			isCameraShake = true;
		}
		if (isCameraShake && cameraShakeTime < 51)
		{
			if (cameraShakeTime == 50)
			{
				isCameraShake = false;
			}
			cameraShakeTime++;
			camera->ShakeCamera(cameraShakeTime);
		}


		camera->transform.translate.x = player->LerpShortTranslate(camera->transform.translate.x, player->model_->worldTransform_->translation_.x, 0.04f);
		camera->transform.translate.z = player->LerpShortTranslate(camera->transform.translate.z, player->model_->worldTransform_->translation_.z - 10.0f, 0.04f);
		camera->UpdateMatrix();

		fadeSprite->Update();

	}

	Hitstop();
}

void GamePlayScene::Draw()
{
	demo_stage->Draw(camera);
	player->Draw();
	sword->Draw(camera);

	

	//ここから敵の弾の処理
	for (EnemyBullet* enemyBullets : enemyBullet_) {
		enemyBullets->Draw(camera);
	}

	//ここから敵を出す処理
	for (Enemy* enemys : enemy_) {
		enemys->Draw(camera);
	}

	for (DeathEffect* deathEffects : deathEffect_) {
		deathEffects->Draw();
	}


	if (isBossSpawn == true) {
		boss_->Draw(camera);
	}

	fadeSprite->Draw(camera);

	player->TextureDraw();

	//colliderManager_->Draw(camera);
}

void GamePlayScene::CheckAllCollisions()
{
	for (EnemyBullet* enemyBullets : enemyBullet_) {
		for (Enemy* enemys : enemy_) {
			//コライダーのリストをクリア
			colliderManager_->ListClear();

			//コライダーにオブジェクトを登録
			if (player->GetIsCoolDown() == false) {
				colliderManager_->AddColliders(player.get());
			}
			if (player->GetIsUnderAttack() == true || player->GetIsSkillAttack() == true) {
				colliderManager_->AddColliders(sword.get());
			}
			if (enemyBullets->GetIsDead() == false) {
				colliderManager_->AddColliders(enemyBullets);
			}
			colliderManager_->AddColliders(enemys);

			//当たり判定
			colliderManager_->ChackAllCollisions();
		}
	}
}

void GamePlayScene::BossSceneAllCollisions() {
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	if (player->GetIsCoolDown() == false) {
		colliderManager_->AddColliders(player.get());
	}
	if (player->GetIsUnderAttack() == true || player->GetIsSkillAttack() == true) {
		colliderManager_->AddColliders(sword.get());
	}
	if (boss_->GetIsCoolDown() == false) {
		colliderManager_->AddColliders(boss_.get());
	}
	colliderManager_->AddColliders(bossBullet_.get());
	//colliderManager_->AddColliders(sampleEnemy.get());

	//当たり判定
	colliderManager_->ChackAllCollisions();
}

void GamePlayScene::EnemySpawn() {

	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(player.get());

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

			const float kBulletSpeed = 0.15f;
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
			if (enemyAttackCoolDown >= 180) {
				enemyAttackCoolDown = 0;
				isEnemyAttack = true;
			}
		}
	}

}


void GamePlayScene::CreateDeathEffect(Vector3 position)
{
	DeathEffect* newDeathEffect = new DeathEffect();
	newDeathEffect->Initialize(camera);
	newDeathEffect->SetFlag(true);

	newDeathEffect->SetPosition(position);

	deathEffect_.push_back(newDeathEffect);
}

void GamePlayScene::StartFadeOut()
{
	isFadeOut = true;
	fadeAlpha = 1.0f;
	fadeSprite->SetisInvisible(false);
}

void GamePlayScene::UpdateFadeOut()
{
	fadeAlpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeAlpha });

	if (fadeAlpha <= 0.0f)
	{
		fadeSprite->SetisInvisible(true);
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}

void GamePlayScene::StartFadeIn()
{
	isFadeIn = true;
	fadeAlpha = 0.0f;
	fadeSprite->SetisInvisible(false);
}

void GamePlayScene::UpdateFadeIn()
{
	fadeAlpha++; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeAlpha });

	if (fadeAlpha >= 1.0f)
	{
		// フェードイン完了時の処理
		//isFadeIn = false;
		sceneManager_->ChangeScene("GAMEOVER");
	}
}


void GamePlayScene::Hitstop()
{
	if (player->GetIsHit() == true && throughTimer == 0)
	{
		hitstopTimer++;
	}
	if (hitstopTimer == 15)
	{
		player->SetIsHit(false);
		throughTimer++;
	}
	if (throughTimer == 30)
	{
		hitstopTimer = 0;
		throughTimer = 0;
	}
}
