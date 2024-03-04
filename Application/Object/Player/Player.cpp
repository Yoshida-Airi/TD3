#include "Player.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"


void Player::Initialize()
{
	input_ = Input::GetInstance();

	Collider::Initialize();
	
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	SetRadius(model_->worldTransform_->scale_.x);

}

void Player::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	model_->ModelDebug("player");

#ifdef _DEBUG
	
	ImGui::Begin("HitCheack");
	ImGui::Text("HP : %d", HP);
	ImGui::End();

#endif // _DEBUG


	Move();

	Attack();
}

void Player::Draw(Camera* camera)
{
	model_->Draw(camera);
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

Vector3 Player::GetCenterPosition() const
{
	const Vector3 offset = { 0.0f,1.0f,0.0f };
	//ワールド座標に変換
	Vector3 worldPos = TransformNormal(offset, model_->worldTransform_->matWorld_);
	return worldPos;
}

void Player::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		HP -= 1;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		HP -= 1;
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
