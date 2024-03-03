#include "Player.h"
#include"PlayerWeapon.h"

void Player::Initialize()
{
	input_ = Input::GetInstance();

	Model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	Speed = 0.03f;

	
}

void Player::Update()
{
	Model_->Update();
	Model_->ModelDebug("player");
	Move();

	Attack();

	Weapon_->Update();

}

void Player::Draw(Camera* camera)
{
	Model_->Draw(camera);

	if (isUnderAttack == true)
	{
		Weapon_->Draw(camera);
	}
}

void Player::Move()
{
	if (input_->PushKey(DIK_W))
	{
		Model_->worldTransform_->translation_.z += Speed;
	}
	if (input_->PushKey(DIK_S))
	{
		Model_->worldTransform_->translation_.z -= Speed;
	}
	if (input_->PushKey(DIK_A))
	{
		Model_->worldTransform_->translation_.x -= Speed;
	}
	if (input_->PushKey(DIK_D))
	{
		Model_->worldTransform_->translation_.x += Speed;
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
