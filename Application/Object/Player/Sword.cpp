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

	ImGui::Begin("combo");
	ImGui::Text("isAttack : %d\ncombo1 : %d\ncombo2 : %d\ncount : %d", isAttack, combo1, combo2,count);
	ImGui::End();

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

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			isAttack = true;
		}
	}
	if (input_->IsLeftMouseClicked() && combo1 == false && combo2 == false)
	{
		isAttack = true;
	}
	//三連続攻撃とか作りたい
	if (isAttack == true) {
		if (model_->worldTransform_->rotation_.x >= -0.83f) {
			model_->worldTransform_->rotation_.x -= 0.10f;
		}
		if (model_->worldTransform_->rotation_.y <= 2.48f) {
			model_->worldTransform_->rotation_.y += 0.16f;
		}
		if (model_->worldTransform_->rotation_.z >= -0.26f) {
			model_->worldTransform_->rotation_.z -= 0.12f;
		}
		if (model_->worldTransform_->translation_.x <= 1.0f) {
			model_->worldTransform_->translation_.x += 0.1f;
		}
		if (model_->worldTransform_->rotation_.x <= -0.83f && model_->worldTransform_->rotation_.y >= 2.48f && model_->worldTransform_->rotation_.z <= -0.26f && model_->worldTransform_->translation_.x >= 1.0f) {
			count++;
			if (count <= 120) {
				if (input_->IsLeftMouseClicked()) {
					count = 0;
					combo1 = true;
					isAttack = false;
				}
				if (Input::GetInstance()->GetJoystickState(0, joyState)) {
					if (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_SHOULDER)
					{
						count = 0;
						combo1 = true;
						isAttack = false;
					}
				}
				if (count >= 120) {
					count = 0;
					isAttack = false;
				}
			}
		}
	}
	if (combo1 == true) {
		model_->worldTransform_->rotation_.x += 0.10f;
		count++;
		if (count >= 20 && count <= 80) {
			if (input_->IsLeftMouseClicked()) {
				count = 0;
				combo2 = true;
				combo1 = false;
			}
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_SHOULDER)
				{
					count = 0;
					combo2 = true;
					combo1 = false;
				}
			}
			if (count >= 80) {
				count = 0;
				combo1 = false;
			}
		}
	}
	if (combo2 == true) {
		model_->worldTransform_->rotation_.y += 0.10f;
		player_->PlayerSpeed = 4.0f;
		count++;
		if (count >= 40) {
			count = 0;
			player_->PlayerSpeed = 2.0f;
			combo2 = false;
		}
	}
	if (isAttack == false &&  combo1 == false && combo2 == false) {
		model_->worldTransform_->rotation_.x = 0.79f;
		model_->worldTransform_->rotation_.y = -0.31f;
		model_->worldTransform_->rotation_.z = 1.39f;
		model_->worldTransform_->translation_.x = -1.0f;
	}
}
