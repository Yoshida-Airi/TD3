#include "Sword.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"


void Sword::Initialize(Player* player)
{
	input_ = Input::GetInstance();

	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));

	model_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));

	SetRadius({ 3.0f,1.0f,1.0f });
	player_ = player;
}

void Sword::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	model_->ModelDebug("Sword");

	//コントローラーチェンジ
	if (input_->PushKey(DIK_1)) {
		gamePad = true;
		keyBoard = false;
	}
	if (input_->PushKey(DIK_2)) {
		gamePad = false;
		keyBoard = true;
	}


	Attack();
}

void Sword::Draw(Camera* camera)
{
	model_->Draw(camera);
}

void Sword::SetPosition(Vector3 translation)
{
	model_->worldTransform_->translation_ = translation;
};
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

Vector3 Sword::GetRotate()
{
	return model_->worldTransform_->rotation_;
}


void Sword::OnCollision([[maybe_unused]] Collider* other)
{
}

void Sword::Attack()
{
	XINPUT_STATE joyState;

	if (gamePad == true) {
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
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
	else if (keyBoard == true) {
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
}
void Sword::Skill()
{
	XINPUT_STATE joyState;

	if (gamePad == true) {
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			isSkill = true;
		}
		else
		{
			isSkill = false;
		}
	}
	else if (keyBoard == true) {
		if (input_->IsLeftMouseClicked()) {
			isSkill = true;
		}
		else
		{
			isSkill = false;
		}
	}
}