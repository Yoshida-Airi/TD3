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
	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");
	
	clearSprite.reset(Sprite::Create(ClearSceneTex));

	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,fadeOutAlpha });

	fadeOutAlpha = 1.0f;
	fadeInAlpha = 0.0f;

	StartFadeOut();
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
		StartFadeIn();
	}

	if (isFadeIn == true)
	{
		UpdateFadeIn();
	}

	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}

	clearSprite->Update();
	fadeSprite->Update();

}

void GameClearScene::Draw()
{
	clearSprite->Draw(camera);
	fadeSprite->Draw(camera);
}



void GameClearScene::StartFadeOut()
{
	isFadeOut = true;
	fadeSprite->SetisInvisible(false);
}

void GameClearScene::UpdateFadeOut()
{
	fadeOutAlpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeOutAlpha });

	if (fadeOutAlpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}


void GameClearScene::StartFadeIn()
{
	isFadeIn = true;
	fadeSprite->SetisInvisible(false);
}

void GameClearScene::UpdateFadeIn()
{
	fadeInAlpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeInAlpha });

	if (fadeInAlpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadeIn = false;
		sceneManager_->ChangeScene("TITLE");
	}
}
