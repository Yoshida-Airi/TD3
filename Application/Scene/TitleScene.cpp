#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete efect;

	for (Enemy* enemys : enemy_) {
		delete enemys;
	}

}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();



	fence_.reset(Model::Create("Resources/DefaultAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	fence_->worldTransform_->rotation_.y = 3.1f;
}

void TitleScene::Update()
{
	camera->CameraDebug();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
			Audio::GetInstance()->SoundStopWave(soundData);
		}
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
		Audio::GetInstance()->SoundStopWave(soundData);
	}

	fence_->Update();
	cube_->Update();


	
	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

	for (Enemy* enemys : enemy_) {
		enemys->Update();
	}

	EnemySporn();

}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

	for (Enemy* enemys : enemy_) {
		enemys->Draw(camera);
	}

}

void TitleScene::EnemySporn() {

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
