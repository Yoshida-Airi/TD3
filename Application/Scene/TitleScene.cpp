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

	//soundData = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();

	fence_.reset(Model::Create("Resources/DefaultAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	fence_->worldTransform_->rotation_.y = 3.1f;

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

	fence_->Update();
	cube_->Update();

	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

	effect->Update();

	slashingEffect->Update();

	titleSprite->Update();
	fadeSprite->Update();

}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

	effect->Draw();
	slashingEffect->Draw();

	titleSprite->Draw(camera);
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
		//Audio::GetInstance()->SoundStopWave(soundData);
	}
}
