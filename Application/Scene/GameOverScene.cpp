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


	UI_MouseTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_MouseLeftClick.png");
	UI_Mouse.reset(Sprite::Create(UI_MouseTex));
	UI_Mouse->SetPosition({ 1100.0f,450.0f });

	UI_GamePadABottonTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_ABotton.png");
	UI_GamePadABotton.reset(Sprite::Create(UI_GamePadABottonTex));
	UI_GamePadABotton->SetPosition({ 1100.0f,470.0f });
	UI_GamePadABotton->worldTransform_->scale_ = { 0.1f,0.1f };

	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,fadeOutAlpha });

	GameoverBGM = Audio::GetInstance()->SoundLoadWave("Resources/Sound/ClearBGM.wav");
	Audio::GetInstance()->SoundPlayWave(GameoverBGM, false);
	Audio::GetInstance()->SoundVolume(GameoverBGM, 0.01f);

	fadeOutAlpha = 1.0f;
	fadeInAlpha = 0.0f;

	StartFadeOut();
}

void GameOverScene::Update()
{
	camera->CameraDebug();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		UI_GamePadABotton->SetisInvisible(false);
		UI_Mouse->SetisInvisible(true);
	}
	else
	{
		UI_GamePadABotton->SetisInvisible(true);
		UI_Mouse->SetisInvisible(false);
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			StartFadeIn();
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

	overSprite->Update();
	fadeSprite->Update();
	UI_GamePadABotton->Update();
	UI_Mouse->Update();

}

void GameOverScene::Draw()
{
	overSprite->Draw(camera);
	UI_GamePadABotton->Draw(camera);
	UI_Mouse->Draw(camera);

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


void GameOverScene::StartFadeIn()
{
	isFadeIn = true;
	fadeSprite->SetisInvisible(false);
}

void GameOverScene::UpdateFadeIn()
{
	fadeInAlpha+=0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeInAlpha });

	if (fadeInAlpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadeIn = false;
		sceneManager_->ChangeScene("TITLE");
		Audio::GetInstance()->SoundStopWave(GameoverBGM);

	}
}