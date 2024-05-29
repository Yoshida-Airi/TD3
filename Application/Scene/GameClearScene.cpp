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

	camera->transform.translate.y = 11.0f;
	camera->transform.rotate.x = 0.7f;

	SetPlayerModel();

	swoad_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));
	swoad_->worldTransform_->scale_ = { 0.4f,0.4f,0.4f };
	swoad_->worldTransform_->translation_ = { -1.3f,3.0f,-2.3f };
	swoad_->worldTransform_->rotation_ = { -0.1f,0.9f,-0.6f };
	//swoad_->SetLightDirection({ -5.2f,0.1f,1.0f });

	ui_gameOver_.reset(Model::Create("Resources/UI/UI_gameclear.obj"));
	ui_gameOver_->worldTransform_->translation_ = { -0.03f,4.3f,0.0f };
	ui_gameOver_->worldTransform_->rotation_ = { 0.3f,3.15f,0.0f };
	ui_gameOver_->worldTransform_->scale_ = { 1.5f,1.2f,2.4f };
	ui_gameOver_->SetLightDirection({ -5.2f ,lightDirectionX,1.0f });

	ClearSceneTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/gameclear.png");
	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");
	
	clearSprite.reset(Sprite::Create(ClearSceneTex));

	UI_MouseTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_MouseLeftClick.png");
	UI_Mouse.reset(Sprite::Create(UI_MouseTex));
	UI_Mouse->SetPosition({ 1100.0f,450.0f });

	UI_GamePadABottonTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_ABotton.png");
	UI_GamePadABotton.reset(Sprite::Create(UI_GamePadABottonTex));
	UI_GamePadABotton->SetPosition({ 1100.0f,470.0f });
	UI_GamePadABotton->worldTransform_->scale_ = { 0.1f,0.1f };

	ClearBGM = Audio::GetInstance()->SoundLoadWave("Resources/Sound/ClearBGM.wav");
	Audio::GetInstance()->SoundPlayWave(ClearBGM, false);
	Audio::GetInstance()->SoundVolume(ClearBGM, 0.01f);

	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,fadeOutAlpha });

	demo_stage.reset(Model::Create("Resources/DefaultAssets/stage.obj"));
	demo_stage->SetLightDirection({ -5.2f,0.1f,1.0f });

	fadeOutAlpha = 1.0f;
	fadeInAlpha = 0.0f;

	StartFadeOut();
	for (int i = 0; i < 3; i++)
	{
		numberTex[i] = TextureManager::GetInstance()->LoadTexture(texNum);
		numberSprite[i].reset(Sprite::Create(numberTex[i]));
		numberSprite[i]->SetSize({ 36.0f,72.0f });
	}
	scoreTex = TextureManager::GetInstance()->LoadTexture(texScore);
	scoreSprite.reset(Sprite::Create(scoreTex));

}


void GameClearScene::Update()
{
	camera->CameraDebug();
	camera->UpdateMatrix();

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

	clearSprite->Update();
	fadeSprite->Update();
	UI_GamePadABotton->Update();
	UI_Mouse->Update();

	for (int i = 0; i < 3; i++)
	{
		numberSprite[i]->Update();
	}

	

	numberSprite[0]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 568),500.0f };
	numberSprite[1]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 536),500.0f };
	numberSprite[2]->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 500),500.0f };

	uint32_t texNum100 = SceneManager::bossTimeData / 100;
	uint32_t texNum10 = SceneManager::bossTimeData % 100 / 10;
	uint32_t texNum1 = SceneManager::bossTimeData % 100 % 10;
	numberSprite[0]->SetTextureLeftTop({ 36.0f * texNum100,0.0f });
	numberSprite[1]->SetTextureLeftTop({ 36.0f * texNum10,0.0f });
	numberSprite[2]->SetTextureLeftTop({ 36.0f * texNum1,0.0f });

	scoreSprite->Update();
	scoreSprite->worldTransform_->translation_ = { float(winApp_->kCilentWidth / 2 - 468),512.0f };

	demo_stage->Update();

	bodyModel_->Update();
	headModel_->Update();
	LeftArmModel_->Update();
	RightArmModel_->Update();
	LeftFootModel_->Update();
	RightFootModel_->Update();
	swoad_->Update();
	ui_gameOver_->Update();
	ui_gameOver_->SetLightDirection({ -5.2f, lightDirectionX, 1.0f });


	bodyModel_->ModelDebug("body");
	headModel_->ModelDebug("head");
	LeftArmModel_->ModelDebug("leftArm");
	RightArmModel_->ModelDebug("rightArm");
	LeftFootModel_->ModelDebug("leftFoot");
	RightFootModel_->ModelDebug("rightFoot");

	swoad_->ModelDebug("swoad");

	ui_gameOver_->ModelDebug("gameover");

	GameClearAction();
}

void GameClearScene::Draw()
{
	demo_stage->Draw(camera);
	bodyModel_->Draw(camera);
	headModel_->Draw(camera);
	LeftArmModel_->Draw(camera);
	RightArmModel_->Draw(camera);
	LeftFootModel_->Draw(camera);
	RightFootModel_->Draw(camera);
	swoad_->Draw(camera);

	ui_gameOver_->Draw(camera);

	UI_GamePadABotton->Draw(camera);
	UI_Mouse->Draw(camera);
	for (int i = 0; i < 3; i++)
	{
		numberSprite[i]->Draw(camera);
	}
	scoreSprite->Draw(camera);

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
		Audio::GetInstance()->SoundStopWave(ClearBGM);
	}
}


void GameClearScene::SetPlayerModel()
{
	playerTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/base.png");
	headTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/head_smile.png");
	bodyTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/body.png");
	leftFootTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/leftFoot.png");
	rightFootTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/rightFoot.png");

	bodyModel_.reset(Model::Create("Resources/PlayerModel/body.obj"));
	headModel_.reset(Model::Create("Resources/PlayerModel/head.obj"));
	LeftArmModel_.reset(Model::Create("Resources/PlayerModel/leftArm.obj"));
	RightArmModel_.reset(Model::Create("Resources/PlayerModel/RightArm.obj"));
	LeftFootModel_.reset(Model::Create("Resources/PlayerModel/LeftFoot.obj"));
	RightFootModel_.reset(Model::Create("Resources/PlayerModel/RightFoot.obj"));



	bodyModel_->SetTexture(bodyTex);
	headModel_->SetTexture(headTex);
	LeftArmModel_->SetTexture(playerTex);
	RightArmModel_->SetTexture(playerTex);
	LeftFootModel_->SetTexture(leftFootTex);
	RightFootModel_->SetTexture(rightFootTex);

	headModel_->Parent(bodyModel_.get());
	LeftArmModel_->Parent(bodyModel_.get());
	RightArmModel_->Parent(bodyModel_.get());
	LeftFootModel_->Parent(bodyModel_.get());
	RightFootModel_->Parent(bodyModel_.get());

	headModel_->SetLightDirection({ -5.2f,0.1f,1.0f });
	bodyModel_->SetLightDirection({ -5.2f,0.1f,1.0f });
	LeftArmModel_->SetLightDirection({ -5.2f,0.1f,1.0f });
	RightArmModel_->SetLightDirection({ -5.2f,0.1f,1.0f });
	LeftFootModel_->SetLightDirection({ -5.2f,0.1f,1.0f });
	RightFootModel_->SetLightDirection({ -5.2f,0.1f,1.0f });

	bodyModel_->worldTransform_->translation_ = { 0.0f,0.05f,-3.8f };
	bodyModel_->worldTransform_->rotation_ = { 0.5f,3.11f,0.0f };

	headModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	headModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	LeftArmModel_->worldTransform_->translation_ = { 0.28f,2.38f,0.0f };
	LeftArmModel_->worldTransform_->rotation_ = { 0.0f,3.2f,-0.72f };

	RightArmModel_->worldTransform_->translation_ = { -0.28f,2.38f,0.0f };
	RightArmModel_->worldTransform_->rotation_ = { 0.0f,-3.2f,0.72f };

	LeftFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	LeftFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	RightFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	RightFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };
}


void GameClearScene::GameClearAction()
{

	MotionTimer++;

	if (motionCount == 0)
	{
		lightDirectionX -= 0.7f;

		if (MotionTimer >= 40)
		{
			motionCount = 1;
		}
	}

	if (motionCount == 1)
	{
		lightDirectionX += 0.7f;

		if (MotionTimer >= 80)
		{
			motionCount = 2;
		}
	}


	if (motionCount == 2)
	{
		MotionTimer = 0.0f;
		motionCount = 0;
	}

}