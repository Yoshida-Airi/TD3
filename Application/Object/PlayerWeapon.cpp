#include "PlayerWeapon.h"

void PlayerWeapon::Initialize()
{
	weapon.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	weapon->worldTransform_->scale_.x = 0.5f;
	weapon->worldTransform_->scale_.y = 0.5f;
	weapon->worldTransform_->scale_.z = 0.5f;

}

void PlayerWeapon::Update()
{
	weapon->Update();
}

void PlayerWeapon::Draw(Camera* camera)
{
	weapon->Draw(camera);
}

void PlayerWeapon::SetPosition(Vector3 position)
{
	weapon->worldTransform_->translation_ = position;
}
