#include "PlayerWeapon.h"
#include"CollisionConfig.h"

void PlayerWeapon::Initialize()
{
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));

	model.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	model->worldTransform_->scale_.x = 0.5f;
	model->worldTransform_->scale_.y = 0.5f;
	model->worldTransform_->scale_.z = 0.5f;

	SetRadius(model->worldTransform_->scale_.x);

}

void PlayerWeapon::Update()
{
	Collider::UpdateWorldTransform();
	model->Update();
}

void PlayerWeapon::Draw(Camera* camera)
{
	model->Draw(camera);
}

void PlayerWeapon::SetPosition(Vector3 position)
{
	model->worldTransform_->translation_ = position;
}

Vector3 PlayerWeapon::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

Vector3 PlayerWeapon::GetCenterPosition() const
{
	const Vector3 offset = { 0.0f,0.25f,0.0f };
	//ワールド座標に変換
	Vector3 worldPos = TransformNormal(offset, model->worldTransform_->matWorld_);
	return worldPos;
}

void PlayerWeapon::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
	
	}
}
