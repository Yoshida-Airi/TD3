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

	currentTime = int(time(nullptr));
	srand(currentTime);

	uvTexture = textureManager_->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	monsterBall = textureManager_->LoadTexture("Resources/DefaultAssets/monsterBall.png");
	Doll = textureManager_->LoadTexture("Resources/DefaultAssets/Doll.png");
	circle = textureManager_->LoadTexture("Resources/DefaultAssets/circle.png");
	//demo_stage.reset(Model::Create("Resources/DefaultAssets/demo_stage.obj"));
	demo_stage.reset(Model::Create("Resources/DefaultAssets/stage.obj"));

	GameBGM = Audio::GetInstance()->SoundLoadWave("Resources/Sound/GameBGM.wav");
	Audio::GetInstance()->SoundPlayWave(GameBGM, false);
	Audio::GetInstance()->SoundVolume(GameBGM, 0.01f);


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
	sceneClear = false;
	sceneOver = false;

	StartFadeOut();

	for(int i=0;i<3;i++)
	{
		numberTex[i] = TextureManager::GetInstance()->LoadTexture(texNum);
		numberSprite[i].reset(Sprite::Create(numberTex[i]));
		numberSprite[i]->SetSize({ 36.0f,72.0f });
	}

	throughTimer = 0;
	kRestWaveTime = kFullWaveTime;
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
		if (sceneClear == true)
		{
			UpdateFadeIn("GAMECLEAR");
		}
		if (sceneOver == true)
		{
			UpdateFadeIn("GAMEOVER");
		}
	}

	if (timer.GetNowSecond() != kFullWaveTime)
	{
		sprite->worldTransform_->translation_ =
		{
			sprite->worldTransform_->translation_.x - 20.0f,sprite->worldTransform_->translation_.y,sprite->worldTransform_->translation_.z
		};
		timer.AddNowFrame();
		timer.AddNowWaveFrame();
		if (isHitstop != true)
		{

			//敵の処理
			EnemyAttack();
			boss_->SetTranslate(player->GetPosition());

			if (isEnemySpawn == true) {
				if (enemyCount <= MaxEnemySpawn) {
					EnemySpawn();
				}
				else {
					isEnemySpawn = false;
				}
			}

			for (Enemy* enemys : enemy_) {
				enemys->Update();
				if (enemys->GetIsDead()) {
					//貰える経験値
					player->GetPlayerLevel()->Experiencepoint += 16.0f;
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

			EnemyNumberOfOccurrences();

			sprite->worldTransform_->translation_ =
			{
				1280.0f,300.0f,0.0f
			};
		}
		kRestWaveTime = kFullWaveTime - timer.GetNowSecond();
	}
	else if (timer.GetNowSecond() >= kFullWaveTime)
	{
		timer.AddBossFrame();
		if (isHitstop != true)
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
		sceneManager_->ChangeScene("GAMECLEAR");
	}

	if (player->GetHP() <= 0)
	{
		//sceneManager_->ChangeScene("GAMEOVER");
		StartFadeIn();
		sceneOver = true;
	}

	if (boss_->GetIsDead())
	{
		StartFadeIn();
		sceneClear = true;
	}
	

	if (isHitstop != true)
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
		if (timer.GetNowSecond() != kFullWaveTime)
		{
			for (int i = 0; i < 3; i++)
			{
				numberSprite[i]->Update();
			}
			uint32_t texNum100 = kRestWaveTime / 100;
			uint32_t texNum10 = kRestWaveTime % 100 / 10;
			uint32_t texNum1 = kRestWaveTime % 100 % 10;
			numberSprite[0]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 54),0.0f };
			numberSprite[1]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 18),0.0f };
			numberSprite[2]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 + 18),0.0f };
			numberSprite[0]->SetTextureLeftTop({ 36.0f * texNum100,0.0f });
			numberSprite[1]->SetTextureLeftTop({ 36.0f * texNum10,0.0f });
			numberSprite[2]->SetTextureLeftTop({ 36.0f * texNum1,0.0f });
		}
		else if (timer.GetNowSecond() >= kFullWaveTime)
		{
			for (int i = 0; i < 3; i++)
			{
				numberSprite[i]->Update();
			}
			uint32_t texNum100 = timer.GetBossSecond() / 100;
			uint32_t texNum10 = timer.GetBossSecond() % 100 / 10;
			uint32_t texNum1 = timer.GetBossSecond() % 100 % 10;
			numberSprite[0]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 54),0.0f };
			numberSprite[1]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 18),0.0f };
			numberSprite[2]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 + 18),0.0f };
			numberSprite[0]->SetTextureLeftTop({ 36.0f * texNum100,0.0f });
			numberSprite[1]->SetTextureLeftTop({ 36.0f * texNum10,0.0f });
			numberSprite[2]->SetTextureLeftTop({ 36.0f * texNum1,0.0f });
		}
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
	for (int i = 0; i < 3; i++)
	{
		numberSprite[i]->Draw(camera);
	}

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
			if (enemys->GetIsCoolDown() == false) {
				colliderManager_->AddColliders(enemys);
			}
			//colliderManager_->AddColliders(enemys);

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
	if (nomber_ <= 2) {
		nomber_++;
	}
	else {
		nomber_ = 0;
	}
	std::mt19937 random(generator());

	newEnemy->Initialize(player.get(), nomber_);

	newEnemy->SetTranslate(random, player->GetPosition());

	if (newEnemy->GetIsRelottery() == false) {
		enemy_.push_back(newEnemy);
		enemyCount++;
	}

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
	fadeSprite->SetisInvisible(false);
}

void GamePlayScene::UpdateFadeIn(const std::string& sceneName)
{
	fadeInAlpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeInAlpha });

	if (fadeInAlpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadeIn = false;
		Audio::GetInstance()->SoundStopWave(GameBGM);
		sceneManager_->ChangeScene(sceneName);
	}
}


void GamePlayScene::Hitstop()
{

	for (Enemy* enemys : enemy_)
	{
		if (enemys->GetIsDead() == true && isHitstop == false)
		{
			isHitstop = true;
			continue;
		}
	}
	if (isHitstop == true && isKillBoss == false)
	{
		hitstopTimer++;
	}
	if (hitstopTimer == 8 && isKillBoss == false)
	{
		hitstopTimer = 0;
		isHitstop = false;
	}

	if (boss_->GetIsDead() == true && isHitstop == false)
	{
		isHitstop = true;
		isKillBoss = true;
	}
	if (isHitstop == true && isKillBoss == true)
	{
		hitstopTimer++;
	}
	if (hitstopTimer == 3 && isKillBoss == true)
	{
		hitstopTimer = 0;
		isHitstop = false;
		isKillBoss = false;
	}
}

void GamePlayScene::EnemyNumberOfOccurrences() {
	enemyNumberOfOccurrences = rand() % 15 + 5;
}
