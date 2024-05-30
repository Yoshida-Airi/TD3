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

	TitleWall.reset(Model::Create("Resources/Scene/TitleWall.obj"));
	Title.reset(Model::Create("Resources/Scene/Title.obj"));
	TitleText.reset(Model::Create("Resources/Scene/TitleText.obj"));
	Sword.reset(Model::Create("Resources/Scene/sword.obj"));

	LoadModel();

	camera = new Camera;
	camera->Initialize();

	effect = std::make_unique<DeathEffect>();
	effect->Initialize(camera);

	effect->SetFlag(true);

	slashingEffect = std::make_unique<SlashingEffect>();
	slashingEffect->Initialize(camera);

	slashingEffect->SetFlag(true);

	/*titleTex = TextureManager::GetInstance()->LoadTexture("Resources/Scene/title.png");
	titleSprite.reset(Sprite::Create(titleTex));*/

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

	Title->worldTransform_->translation_ = { 0.0f,8.22f,-5.3f };
	Title->worldTransform_->rotation_ = { 1.0f,3.14f,0.0f };

	TitleText->worldTransform_->translation_ = { 0.5f, 6.74f,-7.61f };
	TitleText->worldTransform_->rotation_ = { 1.0f,3.14f,0.0f };

	Sword->worldTransform_->translation_ = { 0.0f,4.94f,-5.08f };
	Sword->worldTransform_->rotation_ = { -0.02f,1.57f,-0.73f };
	Sword->worldTransform_->scale_ = { 0.8f,0.8f,0.8f };

	TitleWall->worldTransform_->rotation_.x = { 1.7f };
	
}

void TitleScene::Update()
{
	speedframe += addframenum;
	
	if (speedframe >= 135) {
		addframenum *= -1;
		speed *= -1;
	}
	if (speedframe <= 45) {
		addframenum *= -1;
		speed *= -1;
	}

	lightDirectionX += speed * 20;

#ifdef _DEBUG
	camera->CameraDebug();
	ImGui::Begin("Num");
	ImGui::Text("spd : %f", speed);
	ImGui::Text("frame : %f", speedframe);
	ImGui::Checkbox("move", &isMoveSword);
	ImGui::Checkbox("reset", &ResetSword);
	ImGui::End();
#endif // _DEBUG
	if (isMoveSword == false) {
		Sword->worldTransform_->translation_.z += speed;
	}
	if (Title->worldTransform_->rotation_.y >= 4.710f) {
		Title->worldTransform_->rotation_.y = 1.570f;
	}
	Title->worldTransform_->rotation_.y += 0.001f;
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

	//if (input->IsLeftMouseTrigger())
	//{
	//	// フェードイン開始
	//	StartFadeIn();
	//}

	// フェードイン中の処理
	if (isFadingIn)
	{
		UpdateFadeIn();
	}
	if (isMoveSword)
	{
		Swordmove();
	}
	if (ResetSword)
	{
		Sword->worldTransform_->translation_ = { 0.0f,4.94f,-5.08f };
		Sword->worldTransform_->rotation_ = { -0.02f,1.57f,-0.73f };
		Sword->worldTransform_->scale_ = { 0.8f,0.8f,0.8f };
		ResetSword = false;
	}


	effect->Update();

	slashingEffect->Update();

	//titleSprite->Update();
	TitleWall->Update();
	Title->Update();
	TitleText->Update();
	Sword->Update();
	TitleWall->ModelDebug("wall");
	Title->ModelDebug("title");
	TitleText->ModelDebug("text");
	Sword->ModelDebug("sword");
	fadeSprite->Update();
	UI_Mouse->Update();
	UI_GamePadABotton->Update();
	TitleText->SetLightDirection({ lightDirectionX,lightDirectionY,lightDirectionZ });
	Title->SetLightDirection({ lightDirectionX,lightDirectionY,lightDirectionZ });
	Sword->SetLightDirection({ lightDirectionX,lightDirectionY,lightDirectionZ });
	TitleWall->SetLightDirection({ lightDirectionX,lightDirectionY,lightDirectionZ });

}

void TitleScene::Draw()
{

	effect->Draw();
	slashingEffect->Draw();
	TitleWall->Draw(camera);
	Title->Draw(camera);
	TitleText->Draw(camera);
	Sword->Draw(camera);
	//titleSprite->Draw(camera);
	UI_Mouse->Draw(camera);
	UI_GamePadABotton->Draw(camera);
	fadeSprite->Draw(camera);

}

void TitleScene::StartFadeIn()
{
	isFadingIn = true;
	isMoveSword = true;
	fadeSprite->SetisInvisible(false);
}

void TitleScene::Swordmove() {
	Sword->worldTransform_->translation_.y += 0.1f;
	Sword->worldTransform_->translation_.z -= 0.013f;
	if (Sword->worldTransform_->rotation_.x >= -1.1f) {
		Sword->worldTransform_->rotation_.x -= 0.1f;
	}
}

void TitleScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, alpha });

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		isMoveSword = false;
		sceneManager_->ChangeScene("GAMEPLAY");
		Audio::GetInstance()->SoundStopWave(soundData);
	}
}

void TitleScene::LoadModel()
{
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/base.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/head.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/body.png");
	TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/red.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/leftFoot.png");
	TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/rightFoot.png");

	TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_skill_pad.png");
	TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_skill_keyboard.png");

	TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_playerLevel.png");
	TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_Hp.png");
	TextureManager::GetInstance()->LoadTexture("Resources/UI/UI_skillLevel.png");

	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Attack.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Combo1.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Combo2.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/LevelUp.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Dash.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Skill.wav");
	Audio::GetInstance()->SoundLoadWave("Resources/Sound/Damage.wav");



}
