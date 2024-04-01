#include "Player.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"

#include"numbers"

void Player::Initialize(Camera* camera)
{
	input_ = Input::GetInstance();
	camera_ = camera;

	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	model_->worldTransform_->scale_ = { 0.5f,0.5f,0.5f };
	//model_->worldTransform_->translation_.y = 0.5f;
	SetRadius(model_->worldTransform_->scale_);


	bodyModel_.reset(Model::Create("Resources/PlayerModel/body.obj"));
	headModel_.reset(Model::Create("Resources/PlayerModel/head.obj"));
	LeftArmModel_.reset(Model::Create("Resources/PlayerModel/leftArm.obj"));
	RightArmModel_.reset(Model::Create("Resources/PlayerModel/rightArm.obj"));
	LeftFootModel_.reset(Model::Create("Resources/PlayerModel/leftFoot.obj"));
	RightFootModel_.reset(Model::Create("Resources/PlayerModel/rightFoot.obj"));

	bodyModel_->Parent(model_.get());
	headModel_->Parent(bodyModel_.get());
	LeftArmModel_->Parent(bodyModel_.get());
	RightArmModel_->Parent(bodyModel_.get());
	LeftFootModel_->Parent(bodyModel_.get());
	RightFootModel_->Parent(bodyModel_.get());

}

void Player::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();

	bodyModel_->Update();
	headModel_->Update();
	LeftArmModel_->Update();
	RightArmModel_->Update();
	LeftFootModel_->Update();
	RightFootModel_->Update();


#ifdef _DEBUG
	model_->ModelDebug("player");
	ImGui::Begin("Status");
	ImGui::Text("HP : %d", HP);
	ImGui::Text("Power : %d", AttackPower);
	ImGui::End();

#endif // _DEBUG


	//移動
	Move();
	//攻撃
	Attack();
	//スキル
	Skill();
	//方向
	Direction();
	//ヒット時のクールダウン
	CoolDown();

}

void Player::Draw()
{
	model_->Draw(camera_);

	bodyModel_->Draw(camera_);
	headModel_->Draw(camera_);
	LeftArmModel_->Draw(camera_);
	RightArmModel_->Draw(camera_);
	LeftFootModel_->Draw(camera_);
	RightFootModel_->Draw(camera_);

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


void Player::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		HP -= 100;
		isCoolDown = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet))
	{
		HP -= 200;
		isCoolDown = true;
	}
}

void Player::Move()
{
	XINPUT_STATE joyState;
	const float threshold = 0.7f;
	Vector3 move = { 0.0f,0.0f,0.0f };
	bool isMoveing = false;

	//移動
	if (input_->PushKey(DIK_W))
	{
		move.z = PlayerSpeed;
	}
	if (input_->PushKey(DIK_S))
	{
		move.z = -PlayerSpeed;
	}
	if (input_->PushKey(DIK_A))
	{
		move.x = -PlayerSpeed;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = PlayerSpeed;
	}

	if (input_->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 1.0f;
		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 1.0f;
	}

	if (Length(move) > threshold)
	{
		isMoveing = true;

	}
	if (isMoveing == true)
	{
		move.x *= Speed;
		move.y = 0.0f;
		move.z *= Speed;

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}


	// Y軸周り角度(θy)	歩いている方向に顔を向ける
	model_->worldTransform_->rotation_.y = LerpShortAngle(model_->worldTransform_->rotation_.y, angle_, 0.1f);
	model_->worldTransform_->translation_.x += move.x;
	model_->worldTransform_->translation_.z += move.z;

}

void Player::Attack()
{
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			isUnderAttack = true;
		}
		else
		{
			isUnderAttack = false;
		}
	}


	if (input_->IsLeftMouseClicked())
	{
		isUnderAttack = true;
	}
	else
	{
		isUnderAttack = true;
	}


}

void Player::Skill()
{
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			isSkill = true;
		}
	}


}

void Player::PLevelUp()
{
	HP += HPIncreasePerLevel;
	AttackPower += AttackPowerIncreasePerLevel;

}

void Player::CoolDown() {
	if (isCoolDown == true) {
		coolDownTimer++;
	}

	if (coolDownTimer == 120) {
		isCoolDown = false;
		coolDownTimer = 0;
	}

}

void Player::Direction()
{


}




float Player::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}

// 最短角度補間
float Player::LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < 2 * (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= 2 * std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= (float)std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a, diff, t);
}

float Player::LerpShortTranslate(float a, float b, float t) {
	return a + t * (b - a);
}