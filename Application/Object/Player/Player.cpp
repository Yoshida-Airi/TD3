#include "Player.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"

void Player::Initialize()
{
	input_ = Input::GetInstance();

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象の設定
	SetCollisionMask(kCollisionAttributeEnemy);
	

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

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

void Player::OnCollision()
{

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
