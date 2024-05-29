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

	camera->transform.translate.y = 11.0f;
	camera->transform.rotate.x = 0.7f;

	//redTex=TextureManager::GetInstance()->LoadTexture("Resource/")
	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/black.png");
	overSceneTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/gameover.png");
	//overSprite.reset(Sprite::Create(overSceneTex));

	//enemyTexture[0] = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a.png");
	//enemyTexture[1] = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a3.png");
	//enemyTexture[2] = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a4.png");
	//enemyTexture[3] = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a5.png");

	demo_stage.reset(Model::Create("Resources/DefaultAssets/stage.obj"));
	demo_stage->SetLightDirection({ -5.2f,0.1f,1.0f });
	//demo_stage->SetTexture(fadeTex);
	SetPlayerModel();

	swoad_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));
	swoad_->worldTransform_->scale_ = { 0.5f,0.5f,0.5f };
	swoad_->worldTransform_->translation_ = { 1.6f,0.1f,4.5f };
	swoad_->worldTransform_->rotation_ = { 0.0f,1.8f,-0.02f };
	swoad_->SetLightDirection({ -5.2f,0.1f,1.0f });

	//enemy_.reset(Model::Create("Resources/Enemy/ene.obj"));
	//enemy_->SetTexture(enemyTexture[0]);
	//enemy_->worldTransform_->translation_ = { 4.6f,0.0f,6.9f };
	//enemy_->worldTransform_->rotation_ = { 0.0f,3.2f,0.0f };
	//enemy_->worldTransform_->scale_ = { 0.5f,0.5f,0.5f };
	//enemy_->SetLightDirection({ -5.2f,0.1f,1.0f });

	ui_gameOver_.reset(Model::Create("Resources/UI/UI_gameover.obj"));
	ui_gameOver_->worldTransform_->translation_ = { -2.1f,3.93f,0.0f };
	ui_gameOver_->worldTransform_->rotation_ = { 0.42f,3.18f,0.0f };
	ui_gameOver_->worldTransform_->scale_ = { 0.0f,0.0f,0.0f };
	ui_gameOver_->SetLightDirection({-5.2f ,lightDirectionX,1.0f });

	UI_MouseTex = TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_MouseLeftClick.png");
	UI_Mouse.reset(Sprite::Create(UI_MouseTex));
	UI_Mouse->SetPosition({ 1100.0f,500.0f });

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
	camera->UpdateMatrix();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;


	gameoverAnimation();
	EnemyMoveAction();



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

	//if (Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	//	{
	//		StartFadeIn();
	//	}
	//}

	//if (input->IsLeftMouseTrigger())
	//{
	//	StartFadeIn();
	//}

	if (isFadeIn == true)
	{
		UpdateFadeIn();
	}

	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}

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
	//enemy_->Update();

	bodyModel_->ModelDebug("body");
	headModel_->ModelDebug("head");
	LeftArmModel_->ModelDebug("leftArm");
	RightArmModel_->ModelDebug("rightArm");
	LeftFootModel_->ModelDebug("leftFoot");
	RightFootModel_->ModelDebug("rightFoot");

	swoad_->ModelDebug("swoad");
//	enemy_->ModelDebug("enemy1");

	ui_gameOver_->ModelDebug("gameover");

	//overSprite->Update();
	fadeSprite->Update();
	UI_GamePadABotton->Update();
	UI_Mouse->Update();

}

void GameOverScene::Draw()
{
	demo_stage->Draw(camera);

	bodyModel_->Draw(camera);
	headModel_->Draw(camera);
	LeftArmModel_->Draw(camera);
	RightArmModel_->Draw(camera);
	LeftFootModel_->Draw(camera);
	RightFootModel_->Draw(camera);
	swoad_->Draw(camera);

	//enemy_->Draw(camera);

	ui_gameOver_->Draw(camera);

	//overSprite->Draw(camera);
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
	fadeInAlpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, fadeInAlpha });

	if (fadeInAlpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadeIn = false;
		sceneManager_->ChangeScene("TITLE");
		Audio::GetInstance()->SoundStopWave(GameoverBGM);

	}
}

void GameOverScene::SetPlayerModel()
{
	playerTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/base.png");
	headTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/head_dead.png");
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

	//bodyModel_->Parent(model_.get());
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

	bodyModel_->worldTransform_->translation_ = { 1.8f,0.34f,0.0f };
	bodyModel_->worldTransform_->rotation_ = { 1.63f,3.15f,0.63f };

	headModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	headModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	LeftArmModel_->worldTransform_->translation_ = { 0.28f,2.38f,0.0f };
	LeftArmModel_->worldTransform_->rotation_ = { 0.0f,3.07f,0.0f };

	RightArmModel_->worldTransform_->translation_ = { -0.28f,2.38f,0.0f };
	RightArmModel_->worldTransform_->rotation_ = { 0.0f,-3.07f,-0.0f };

	LeftFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	LeftFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	RightFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	RightFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };
}

void GameOverScene::gameoverAnimation()
{

	addScale += 0.02f;

	ui_gameOver_->worldTransform_->scale_ = { addScale ,addScale ,addScale };


	if (addScale >= 1.0f)
	{
		ui_gameOver_->worldTransform_->scale_ = { 1.0f,1.0f,1.0f };
	}

}


void GameOverScene::EnemyMoveAction()
{

	MotionTimer++;

	if (motionCount == 0)
	{
		//enemy_->worldTransform_->translation_.x += 0.04f;
		lightDirectionX += 0.7f;
		//ui_gameOver_->SetLightDirection({ -5.2f,0.1f,1.0f });

		if (MotionTimer >= 40)
		{
			motionCount = 1;
		}
	}

	if (motionCount == 1)
	{
		lightDirectionX -= 0.7f;
		//enemy_->worldTransform_->translation_.x -= 0.04f;

		if (MotionTimer >= 80)
		{
			motionCount = 2;
		}
	}


	if (motionCount == 2)
	{
		//model_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		//isplayHitAction = false;
		MotionTimer = 0.0f;
		motionCount = 0;
	}

}