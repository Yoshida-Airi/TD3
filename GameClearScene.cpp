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

	ClearSceneTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/gameclear.png");
	clearSprite.reset(Sprite::Create(ClearSceneTex));
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

	if (input->IsLeftMouseTrigger())
	{
		sceneManager_->ChangeScene("TITLE");
	}

	clearSprite->Update();

}

void GameClearScene::Draw()
{
	clearSprite->Draw(camera);
}



