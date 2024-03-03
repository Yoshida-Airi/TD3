#include "PlayerWeapon.h"

void playerWeapon::Initialize()
{
	weapon.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	weapon->worldTransform_->scale_.x = 0.5f;
	weapon->worldTransform_->scale_.y = 0.5f;
	weapon->worldTransform_->scale_.z = 0.5f;

}

void playerWeapon::Update()
{
	weapon->Update();
}

void playerWeapon::Draw(Camera* camera)
{
	weapon->Draw(camera);
}

void playerWeapon::SetPosition(Vector3 position)
{
	weapon->worldTransform_->translation_ = position;
}
