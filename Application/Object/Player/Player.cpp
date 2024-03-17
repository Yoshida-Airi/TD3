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

	Skill();

	Direction();

}

void Player::Draw()
{
	model_->Draw(camera_);

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
		HP -= 1;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet))
	{
		HP -= 1;
	}
}

void Player::Move()
{
	const float threshold = 0.7f;
	Vector3 move = { 0.0f,0.0f,0.0f };
	bool isMoveing = false;
	

	if (input_->PushKey(DIK_W))
	{
		move.z = 1.0f;
	}
	if (input_->PushKey(DIK_S))
	{
		move.z = -1.0f;
	}
	if (input_->PushKey(DIK_A))
	{
		move.x = -1.0f;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = 1.0f;
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

		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(camera_->transform.rotate.y);

		move = TransformNormal(move, rotateMatrix);

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}


	// Y軸周り角度(θy)	歩いている方向に顔を向ける
	model_->worldTransform_->rotation_.y = LerpShortAngle(model_->worldTransform_->rotation_.y, angle_, 0.1f);

	model_->worldTransform_->translation_.x += move.x;
	model_->worldTransform_->translation_.z += move.z;

	ImGui::Text("angle : %f", angle_);

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

void Player::Skill()
{
	if (input_->PushKey(DIK_LSHIFT)) {
		isSkill = true;
	}
	else
	{
		isSkill = false;
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