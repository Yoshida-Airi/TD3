#include "GameClearScene.h"

GameClearScene::~GameClearScene()
{
	
}

void GameClearScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

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
			sceneManager_->ChangeScene("GAMEPLAY");
		}
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	effect->Update();

}

void GameClearScene::Draw()
{
	effect->Draw();
}

