#include "Sword.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"


void Sword::Initialize()
{
	input_ = Input::GetInstance();

	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

	model_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));

	SetRadius(model_->worldTransform_->scale_.x);

}

void Sword::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	model_->ModelDebug("Sword");

	Attack();
}

void Sword::Draw(Camera* camera)
{
	model_->Draw(camera);
}

Vector3 Sword::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}


void Sword::OnCollision([[maybe_unused]] Collider* other)
{
}


void Sword::Attack()
{
	if (input_->IsLeftMouseClicked())
	{
		if (model_->worldTransform_->rotation_.y <= rotationmax) {
			model_->worldTransform_->rotation_.y += rotationspeed;
		}
		if (model_->worldTransform_->rotation_.y >= rotationmax) {
			model_->worldTransform_->rotation_.y = rotationmax;
		}
	}
	else
	{
		if (model_->worldTransform_->rotation_.y >= rotationmin) {
			model_->worldTransform_->rotation_.y -= rotationspeed;
		}
		if (model_->worldTransform_->rotation_.y <= rotationmin) {
			model_->worldTransform_->rotation_.y = rotationmin;
		}
	}
}
void Sword::Skill()
{
	if (input_->PushKey(DIK_LSHIFT)) {
		isSkill = true;
	}
	else
	{
		isSkill = false;
	}
}