#include "Sword.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"


void Sword::Initialize()
{
	input_ = Input::GetInstance();

	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));

	model_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));

	SetRadius({3.0f,1.0f,1.0f});

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
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->IsLeftMouseClicked() || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
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
	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->PushKey(DIK_LSHIFT) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		isSkill = true;
	}
	else
	{
		isSkill = false;
	}
}