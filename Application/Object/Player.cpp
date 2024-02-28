#include "Player.h"

void Player::Initialize()
{
	input_ = Input::GetInstance();

	playerModel_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	playerSpeed = 0.03f;
}

void Player::Update()
{
	playerModel_->Update();
	playerModel_->ModelDebug("player");
	Move();

}

void Player::Draw(Camera* camera)
{
	playerModel_->Draw(camera);
}

void Player::Move()
{
	if (input_->PushKey(DIK_W))
	{
		playerModel_->worldTransform_->translation_.z += playerSpeed;
	}
	if (input_->PushKey(DIK_S))
	{
		playerModel_->worldTransform_->translation_.z -= playerSpeed;
	}
	if (input_->PushKey(DIK_A))
	{
		playerModel_->worldTransform_->translation_.x -= playerSpeed;
	}
	if (input_->PushKey(DIK_D))
	{
		playerModel_->worldTransform_->translation_.x += playerSpeed;
	}
}
