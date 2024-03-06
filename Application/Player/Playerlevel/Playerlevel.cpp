#include "Playerlevel.h"

Playerlevel ::~Playerlevel() {

	delete camera;
}

void Playerlevel::Initialize() {

	input = Input::GetInstance();
#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG
	levelbar = textureManager_->LoadTexture("Resources/DefaultAssets/Levelbar.png");
	camera = new Camera;
	camera->Initialize();

	sprite.reset(Sprite::Create(levelbar));
	sprite->SetSize({ 1280.0f, 720.0f });
	sprite->SetTextureLeftTop({ 0,0 });

}
void Playerlevel::Update() {
#ifdef _DEBUG
	camera->CameraDebug();
#endif // _DEBUG
	sprite->Update();
}
void Playerlevel::Draw() {

	sprite->Draw(camera);

}