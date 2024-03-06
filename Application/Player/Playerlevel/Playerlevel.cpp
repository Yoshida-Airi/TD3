#include "Playerlevel.h"

Playerlevel ::~Playerlevel() {

	delete camera;
}

void Playerlevel::Initialize() {

	input = Input::GetInstance();
#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG
	textureManager_ = TextureManager::GetInstance();
	levelbar = textureManager_->LoadTexture("Resources/DefaultAssets/Levelbar.png");
	experiencepointbar = textureManager_->LoadTexture("Resources/DefaultAssets/Experiencepointbar.png");
	levelpointbar = textureManager_->LoadTexture("Resources/DefaultAssets/Levelpointbar.png");
	skillbar = textureManager_->LoadTexture("Resources/DefaultAssets/Skillbar.png");
	camera = new Camera;
	camera->Initialize();
	sprite0.reset(Sprite::Create(levelbar));
	sprite0->SetSize({ 1280.0f, 720.0f });
	sprite0->SetTextureLeftTop({ 0,0 });

	sprite1.reset(Sprite::Create(experiencepointbar));
	sprite1->SetSize({ 11.0f, 18.0f });
	sprite1->SetTextureLeftTop({ 0,0 });
	sprite1->worldTransform_->translation_.x = 54.0f;
	sprite1->worldTransform_->translation_.y = 31.0f;

	sprite2.reset(Sprite::Create(levelpointbar));
	sprite2->SetSize({ 64.0f, 13.0f });
	sprite2->SetTextureLeftTop({ 0,0 });
	sprite2->worldTransform_->translation_.x = 96.0f;
	sprite2->worldTransform_->translation_.y = 18.0f;

	sprite3.reset(Sprite::Create(skillbar));
	sprite3->SetSize({ 64.0f, 13.0f });
	sprite3->SetTextureLeftTop({ 0,0 });
	sprite3->worldTransform_->translation_.x = -1008.0f;
	sprite3->worldTransform_->translation_.y = -49.0f;
}
void Playerlevel::Update() {
#ifdef _DEBUG
	camera->CameraDebug();
#endif // _DEBUG
	sprite1->worldTransform_->scale_.x = Experiencepoint;
	if (sprite1->worldTransform_->scale_.x >= maxbar && nowlevel < maxlevel) {
		Experiencepoint = 0.0f;
		sprite1->worldTransform_->scale_.x = 0.0f;
		//経験値がMAXになったらレベルを上げてメーターを０にする
		Levelup();
		if (nowlevel == 3) {
			sprite3->worldTransform_->translation_.x = 1008.0f;
			sprite3->worldTransform_->translation_.y = 49.0f;
		}
		if (nowlevel == 6 || nowlevel == 10) {
			Skillup();
		}
	}
	
	if (Experiencepoint >= maxbar && nowlevel == maxlevel) {
		Experiencepoint = maxbar;
	}
	//Experiencepoint += 1.0f;//デバック用経験値

	sprite0->Update();
	sprite1->Update();
	sprite2->Update();
	sprite3->Update();

}
void Playerlevel::Draw() {

	sprite3->Draw(camera);
	sprite2->Draw(camera);
	sprite1->Draw(camera);
	sprite0->Draw(camera);

}

void Playerlevel::Levelup() {
	//playerのHPやATTACKを増加するコードをここに書く
	nowlevel += 1;
	sprite2->worldTransform_->scale_.x += 1.0f;
}
void Playerlevel::Skillup() {
	//Skill系
	nowskilllevel += 1;
	sprite3->worldTransform_->scale_.x += 1.0f;

}