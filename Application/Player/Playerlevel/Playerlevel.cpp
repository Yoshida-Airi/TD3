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
}
void Playerlevel::Update() {
#ifdef _DEBUG
	camera->CameraDebug();
#endif // _DEBUG
	sprite1->worldTransform_->scale_.x = Experiencepoint;
	if (sprite1->worldTransform_->scale_.x >= 106.0f) {
		//経験値がMAXになったらレベルを上げてメーターを０にする
		Levelup();
		Experiencepoint = 0.0f;
	}
	Experiencepoint += 1.0f;//デバック用経験値

	sprite1->Update();
	sprite0->Update();
}
void Playerlevel::Draw() {

	sprite1->Draw(camera);
	sprite0->Draw(camera);

}

void Playerlevel::Levelup() {
	//playerのHPやATTACKを増加するコードをここに書く

}