#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/Sound/TitleBGM.wav");
	Audio::GetInstance()->SoundPlayWave(soundData, false);
	Audio::GetInstance()->SoundVolume(soundData, 0.01f);

	camera = new Camera;
	camera->Initialize();

	effect = std::make_unique<DeathEffect>();
	effect->Initialize(camera);

	effect->SetFlag(true);

	slashingEffect = std::make_unique<SlashingEffect>();
	slashingEffect->Initialize(camera);

	slashingEffect->SetFlag(true);

	titleTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/title.png");
	titleSprite.reset(Sprite::Create(titleTex));

	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");
	fadeSprite.reset(Sprite::Create(fadeTex));

	UI_MouseTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_MouseLeftClick.png");
	UI_Mouse.reset(Sprite::Create(UI_MouseTex));
	UI_Mouse->SetPosition({ 370.0f,450.0f });

	UI_GamePadABottonTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_ABotton.png");
	UI_GamePadABotton.reset(Sprite::Create(UI_GamePadABottonTex));
	UI_GamePadABotton->SetPosition({ 370.0f,470.0f });
	UI_GamePadABotton->worldTransform_->scale_ = { 0.1f,0.1f };

	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(true);
	alpha = 0;
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,alpha });
}

void TitleScene::Update()
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
			// フェードイン開始
			StartFadeIn();
		
		}
	}

	if (input->IsLeftMouseTrigger())
	{
		// フェードイン開始
		StartFadeIn();
	}

	// フェードイン中の処理
	if (isFadingIn)
	{
		UpdateFadeIn();
	}


	effect->Update();

	slashingEffect->Update();

	titleSprite->Update();
	fadeSprite->Update();
	UI_Mouse->Update();
	UI_GamePadABotton->Update();


}

void TitleScene::Draw()
{

	effect->Draw();
	slashingEffect->Draw();

	titleSprite->Draw(camera);
	UI_Mouse->Draw(camera);
	UI_GamePadABotton->Draw(camera);
	fadeSprite->Draw(camera);

}

void TitleScene::StartFadeIn()
{
	isFadingIn = true;
	fadeSprite->SetisInvisible(false);
}

void TitleScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, alpha });

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		sceneManager_->ChangeScene("GAMEPLAY");
		Audio::GetInstance()->SoundStopWave(soundData);
	}
}
