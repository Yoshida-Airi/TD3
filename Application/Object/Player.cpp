#include "Player.h"
#include"PlayerWeapon.h"

void Player::Initialize()
{
	input_ = Input::GetInstance();

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	Speed = 0.03f;

}

void Player::Update()
{
	model_->Update();
	model_->ModelDebug("player");
	Move();

	Attack();

	Weapon_->Update();

}

void Player::Draw(Camera* camera)
{
	model_->Draw(camera);

	if (isUnderAttack == true)
	{
		Weapon_->Draw(camera);
	}
}

void Player::Move()
{
	if (input_->PushKey(DIK_W))
	{
		model_->worldTransform_->translation_.z += Speed;
	}
	if (input_->PushKey(DIK_S))
	{
		model_->worldTransform_->translation_.z -= Speed;
	}
	if (input_->PushKey(DIK_A))
	{
		model_->worldTransform_->translation_.x -= Speed;
	}
	if (input_->PushKey(DIK_D))
	{
		model_->worldTransform_->translation_.x += Speed;
	}



}

void Player::Attack()
{
	if (input_->IsLeftMouseClicked())
	{
		isUnderAttack = true;
	}
	else
	{
		isUnderAttack = false;
	}
}
