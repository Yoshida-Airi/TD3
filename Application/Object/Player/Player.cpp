#include "Player.h"
#include"CollisionConfig.h"
#include"numbers"
#include"Player/Sword.h"

void Player::Initialize(Camera* camera)
{
	input_ = Input::GetInstance();
	camera_ = camera;

	Collider::Initialize();
	//属性　：　プレイヤー
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));	

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	model_->worldTransform_->scale_ = { 0.5f,0.5f,0.5f };
	//model_->worldTransform_->translation_.y = 0.5f;
	model_->SetisInvisible(true);
	SetRadius(model_->worldTransform_->scale_);

	playerLevel = std::make_unique<Playerlevel>();
	playerLevel->Initialize();

	bodyModel_.reset(Model::Create("Resources/PlayerModel/body.obj"));
	headModel_.reset(Model::Create("Resources/PlayerModel/head.obj"));
	LeftArmModel_.reset(Model::Create("Resources/PlayerModel/leftArm.obj"));
	RightArmModel_.reset(Model::Create("Resources/PlayerModel/RightArm.obj"));
	LeftFootModel_.reset(Model::Create("Resources/PlayerModel/LeftFoot.obj"));
	RightFootModel_.reset(Model::Create("Resources/PlayerModel/RightFoot.obj"));

	LeftFootModel_->worldTransform_->translation_.y = 1.57f;
	RightFootModel_->worldTransform_->translation_.y = 1.57f;

	RightArmModel_->worldTransform_->translation_.x = 0.26f;
	RightArmModel_->worldTransform_->translation_.y = 2.38f;
	LeftArmModel_->worldTransform_->translation_.x = -0.26f;
	LeftArmModel_->worldTransform_->translation_.y = 2.38f;
	

	bodyModel_->Parent(model_.get());
	headModel_->Parent(bodyModel_.get());
	LeftArmModel_->Parent(bodyModel_.get());
	RightArmModel_->Parent(bodyModel_.get());
	LeftFootModel_->Parent(bodyModel_.get());
	RightFootModel_->Parent(bodyModel_.get());

	//bodyModel_->SetisInvisible(true);
	//headModel_->SetisInvisible(true);
	//LeftArmModel_->SetisInvisible(true);
	//RightArmModel_->SetisInvisible(true);
	//LeftFootModel_->SetisInvisible(true);
	//RightFootModel_->SetisInvisible(true);

}

void Player::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();	//移動用モデル

	bodyModel_->Update();
	headModel_->Update();
	LeftArmModel_->Update();
	RightArmModel_->Update();
	LeftFootModel_->Update();
	RightFootModel_->Update();


	playerLevel->sprite1->worldTransform_->translation_.x = 54.0f;
	playerLevel->sprite1->worldTransform_->translation_.y = 31.0f;
	playerLevel->sprite2->worldTransform_->translation_.x = 96.0f;
	playerLevel->sprite2->worldTransform_->translation_.y = 18.0f;
	playerLevel->sprite3->worldTransform_->translation_.x = -1008.0f;
	playerLevel->sprite3->worldTransform_->translation_.y = -49.0f;
	if (playerLevel->nowlevel >= 3) {
		playerLevel->sprite3->worldTransform_->translation_.x = 1008.0f;
		playerLevel->sprite3->worldTransform_->translation_.y = 49.0f;
	}

	if (playerLevel->nowlevel == playerLevel->count) {
		PLevelUp();
		playerLevel->count += 1;
	}

	playerLevel->Update();



#ifdef _DEBUG
	model_->ModelDebug("player");
	ImGui::Begin("Status");
	ImGui::Text("HP : %d", HP);
	ImGui::Text("Power : %d", AttackPower);
	ImGui::End();

	bodyModel_->ModelDebug("body");
	headModel_->ModelDebug("head");
	LeftArmModel_->ModelDebug("leftArm");
	RightArmModel_->ModelDebug("rightArm");
	LeftFootModel_->ModelDebug("leftFoot");
	RightFootModel_->ModelDebug("rightFoot");

#endif // _DEBUG


	if (behaviorRequest_)
	{
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_)
		{
		case Player::Animation::kRoot:
		default:
			//待機モーション
			RootInitialize();
			break;
		case  Player::Animation::kAttack:
			//攻撃
			AttackInitialize();
			break;
		case  Player::Animation::kSkill1:
			//スキル１
			Skill1Initialize();
			break;
		case  Player::Animation::kSkill2:
			//スキル２
			Skill2Initialize();
			break;
		case  Player::Animation::kSkill3:
			//スキル３
			Skill3Initialzie();
			break;

		}

		behaviorRequest_ = std::nullopt;
	}

	//スキルの更新処理
	switch (behavior_)
	{
	case  Player::Animation::kRoot:
	default:
		RootUpdate();
		break;
	case  Player::Animation::kAttack:
		AttackUpdate();
		break;
	case  Player::Animation::kSkill1:
		Skill1Update();
		break;
	case  Player::Animation::kSkill2:
		Skill2Update();
		break;
	case  Player::Animation::kSkill3:
		Skill3Update();
		break;
	}
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


	playerLevel->Draw();
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
		//敵にあたったら
		HP -= 100;
		isCoolDown = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet))
	{
		//敵の弾にあたったら
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
	if (keyBoard == true) {
		if (input_->PushKey(DIK_W))
		{
			move.z = 2.0f;
		}
		if (input_->PushKey(DIK_S))
		{
			move.z = -2.0f;
		}
		if (input_->PushKey(DIK_A))
		{
			move.x = -2.0f;
		}
		if (input_->PushKey(DIK_D))
		{
			move.x = 2.0f;
		}
	}
	//コントローラーチェンジ
	if (input_->PushKey(DIK_1)) {
		gamePad = true;
		keyBoard = false;
	}
	if (input_->PushKey(DIK_2)) {
		gamePad = false;
		keyBoard = true;
	}

	if (gamePad == true) {
		if (input_->GetJoystickState(0, joyState)) {
			move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 1.0f;
			move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 1.0f;
		}
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

	CoolDown();

}

void Player::Attack()
{
	XINPUT_STATE joyState;

	if (gamePad == true) {
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			isUnderAttack = true;
		}
	}
	else if (keyBoard == true) {
		if (input_->IsLeftMouseClicked()) {
			isUnderAttack = true;
		}
	}


	if (isUnderAttack == true )
	{
		behaviorRequest_ = Animation::kAttack;
	}
}

void Player::Skill()
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
	
	}
	else if (keyBoard == true) {
		if (input_->PushKey(DIK_LSHIFT))
		{
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

	//無敵時間
	if (isCoolDown == true) {
		coolDownTimer++;
	}

	if (coolDownTimer == 120) {
		isCoolDown = false;
		coolDownTimer = 0;
	}

}

void Player::AttackUpdate()
{
	//剣を振りかぶる

	if (isSkillCooldown_) {
		return;
	}

	//移動
	Move();

	MotionTimer_++;

	if (MotionCount_ == 0)
	{
		if (MotionTimer_ == 10)
		{
			MotionCount_ = 1;
		}
	
		if (weapon_->GetWorldTransform()->rotation_.y <= rotationmax) {
			weapon_->GetWorldTransform()->rotation_.y += rotationspeed;
		}
		if (weapon_->GetWorldTransform()->rotation_.y >= rotationmax) {
			weapon_->GetWorldTransform()->rotation_.y = rotationmax;
		}

	}

	if (MotionCount_ == 1)
	{
		if (MotionTimer_ == 20)
		{
			MotionCount_ = 2;
		}

		if (weapon_->GetWorldTransform()->rotation_.y >= rotationmin) {
			weapon_->GetWorldTransform()->rotation_.y -= rotationspeed;
		}
		if (weapon_->GetWorldTransform()->rotation_.y <= rotationmin) {
			weapon_->GetWorldTransform()->rotation_.y = rotationmin;
		}

	
	}


	if (MotionCount_ == 2)
	{
		if (MotionTimer_ == 30)
		{
			MotionCount_ = 3;
		}


		if (weapon_->GetWorldTransform()->rotation_.y >= rotationmin) {
			weapon_->GetWorldTransform()->rotation_.y -= rotationspeed;
		}
		if (weapon_->GetWorldTransform()->rotation_.y <= rotationmin) {
			weapon_->GetWorldTransform()->rotation_.y = rotationmin;
		}
	}

	if (MotionCount_ == 3)
	{
		behaviorRequest_ = Animation::kRoot;
		isUnderAttack = false;
	}
}



void Player::RootUpdate()
{
	//スキルのアニメーション
	if (input_->PushKey(DIK_LSHIFT))
	{
		isSkill = true;
	}

	if (isSkill == false)
	{
		//攻撃
		Attack();
	}
	
	//移動
	Move();

	//スキル
	Skill();
	//ヒット時のクールダウン
	CoolDown();

	if (isSkill == true && isSkillCooldown_ == false)
	{
		if (playerLevel->nowskilllevel == 1)
		{
			behaviorRequest_ = Animation::kSkill1;
		}
		if (playerLevel->nowskilllevel == 2)
		{
			behaviorRequest_ = Animation::kSkill2;
		}
		if (playerLevel->nowskilllevel == 3)
		{
			behaviorRequest_ = Animation::kSkill3;
		}
	}

	// スキルのクールダウンを減らす
	if (isSkillCooldown_) {
		skillCooldownTime_--;
		if (skillCooldownTime_ <= 0) {
			// クールダウンが終了したらフラグをリセットする
			isSkillCooldown_ = false;
			isSkill = false;

		}
	}
}

void Player::Skill1Update()
{
	//スキル　：　ダッシュ

	if (isSkillCooldown_) {
		return;
	}

	MotionTimer_++;

	if (MotionCount_ == 0)
	{
		if (MotionTimer_ == 20)
		{
			MotionCount_ = 1;
		}
		float directionAngle = model_->worldTransform_->rotation_.y;

		float dashSpeed = 0.7f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;


		//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;
	}

	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する

		isSkillCooldown_ = true;
		skillCooldownTime_ = 60;
	}
}

void Player::Skill2Update()
{
	//スキル　：　ダッシュ＋回転

	if (isSkillCooldown_) {
		return;
	}

	MotionTimer_++;

	if (MotionCount_ == 0)
	{
		if (MotionTimer_ == 30)
		{
			MotionCount_ = 1;
		}

		float directionAngle = model_->worldTransform_->rotation_.y;

		float dashSpeed = 0.5f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;
		weapon_->GetWorldTransform()->rotation_.y += 1.0f;
		if (weapon_->GetWorldTransform()->rotation_.y >= 6.28f) {
			weapon_->GetWorldTransform()->rotation_.y = 0.0f;
		}

		//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;

	}


	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する
		isSkillCooldown_ = true;
		skillCooldownTime_ = 60;

	}

}

void Player::Skill3Update()
{
	//スキル　：　範囲攻撃

	if (isSkillCooldown_) {
		return;
	}

	MotionTimer_++;

	if (MotionCount_ == 0)
	{


		if (MotionTimer_ == 30)
		{
			MotionCount_ = 1;
		}
		float directionAngle = model_->worldTransform_->rotation_.y;

		float dashSpeed = 0.5f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;
		weapon_->model_->worldTransform_->rotation_.y += 1.0f;
		if (weapon_->model_->worldTransform_->rotation_.y >= 6.28f) {
			weapon_->model_->worldTransform_->rotation_.y = 0.0f;
		}

		//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;

	}

	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する
		isSkillCooldown_ = true;
		skillCooldownTime_ = 60;

	}

}

void Player::RootInitialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;
}

void Player::AttackInitialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;
}

void Player::Skill1Initialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;
}


void Player::Skill2Initialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;
}

void Player::Skill3Initialzie()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;
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