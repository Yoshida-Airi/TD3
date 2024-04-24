#include "GameOverScene.h"

GameOverScene::~GameOverScene()
{
	delete camera;
}

void GameOverScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera();
	camera->Initialize();

	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");
	overSceneTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/gameover.png");
	overSprite.reset(Sprite::Create(overSceneTex));


	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	fadeOutAlpha = 1.0f;
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,fadeOutAlpha });

	StartFadeOut();
}

void GameOverScene::Update()
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

	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}

	overSprite->Update();
	fadeSprite->Update();

}

void GameOverScene::Draw()
{
	overSprite->Draw(camera);
	fadeSprite->Draw(camera);
}


void GameOverScene::StartFadeOut()
{
	isFadeOut = true;
	fadeSprite->SetisInvisible(false);
}

void GameOverScene::UpdateFadeOut()
{
	fadeOutAlpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeOutAlpha });

	if (fadeOutAlpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}
