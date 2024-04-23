#include "GameClearScene.h"

GameClearScene::~GameClearScene()
{
	delete camera;
}

void GameClearScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera();
	camera->Initialize();

	effect = std::make_unique<DeathEffect>();
	effect->Initialize(camera);

	effect->SetFlag(true);
}

void GameClearScene::Update()
{
	camera->CameraDebug();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("TITLE");
		}
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	effect->Update();

}

void GameClearScene::Draw()
{
	effect->Draw();
}

