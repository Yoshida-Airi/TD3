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

	
	slashingEffect = std::make_unique<SlashingEffect>();
	slashingEffect->Initialize(camera);
	



	bodyModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	bodyModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	headModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	headModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	LeftArmModel_->worldTransform_->translation_ = { 0.28f,2.38f,0.0f };
	LeftArmModel_->worldTransform_->rotation_ = { 0.0f,1.6f,0.72f };

	RightArmModel_->worldTransform_->translation_ = { -0.28f,2.38f,0.0f };
	RightArmModel_->worldTransform_->rotation_ = { 0.0f,-1.6f,-0.72f };

	LeftFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	LeftFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	RightFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	RightFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };


	

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

	// 剣の先端の座標
	Vector3 tipPosition = weapon_->GetWorldPosition();

	// 剣の基点（柄など）の座標
	Vector3 basePosition =
	{
		weapon_->GetWorldPosition().x + 0.5f,
		weapon_->GetWorldPosition().y - 0.5f,
		weapon_->GetWorldPosition().z
	};

	// 剣の方向ベクトルを計算
	Vector3 swordDirection = Subtract(tipPosition, basePosition);

	// 剣の長さ（方向ベクトルの長さ）を計算
	float swordLength = Length(swordDirection);

	swordDirection = Normalize(swordDirection);

	Vector3 particleOffsetDistance = { 0.5f,0.5f,0.0f };

	// パーティクルの発生位置を計算（例: 剣の先端から一定距離離れた位置）
	Vector3 particleStartPosition = Add(tipPosition, swordDirection);
	
	slashingEffect->SetPosition(particleStartPosition);
	slashingEffect->Update();

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
		ImGui::Text("behavior : Root");
		break;
	case  Player::Animation::kAttack:
		AttackUpdate();
		ImGui::Text("behavior : Attack");
		break;
	case  Player::Animation::kSkill1:
		Skill1Update();
		ImGui::Text("behavior : Skill1");
		break;
	case  Player::Animation::kSkill2:
		Skill2Update();
		ImGui::Text("behavior : Skill2");
		break;
	case  Player::Animation::kSkill3:
		ImGui::Text("behavior : Skill3");
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

	slashingEffect->Draw();
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
	isMove = false;
	//移動
	if (keyBoard == true) {
		if (input_->PushKey(DIK_W))
		{
			move.z = 2.0f;
			isMove = true;
		}
		if (input_->PushKey(DIK_S))
		{
			move.z = -2.0f;
			isMove = true;
		}
		if (input_->PushKey(DIK_A))
		{
			move.x = -2.0f;
			isMove = true;
		}
		if (input_->PushKey(DIK_D))
		{
			move.x = 2.0f;
			isMove = true;
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
		slashingEffect->SetFlag(true);
		slashingEffect->StartParticle();
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
	
	//移動
	//Move();

	MotionTimer_++;

	if (MotionCount_ == 0)
	{
		if (MotionTimer_ == 10)
		{
			MotionCount_ = 1;
		}
	
		weapon_->GetWorldTransform()->rotation_.y += 1.6f / 10;

		
		LeftFootModel_->worldTransform_->rotation_.x += 0.1f/10;
		LeftFootModel_->worldTransform_->rotation_.z -= 0.18f/10;
	
		RightFootModel_->worldTransform_->rotation_.x -= 0.1f / 10;
		RightFootModel_->worldTransform_->rotation_.z += 0.18f / 10;

		bodyModel_->worldTransform_->translation_.y -= 0.1f/10;
		bodyModel_->worldTransform_->rotation_.y += 1.0f/10;

	}

	if (MotionCount_ == 1)
	{
		if (MotionTimer_ == 20)
		{
			MotionCount_ = 2;
		}


		weapon_->GetWorldTransform()->rotation_.y -= 1.6f / 10;


		LeftFootModel_->worldTransform_->rotation_.x -= 0.1f / 10;
		LeftFootModel_->worldTransform_->rotation_.z += 0.18f / 10;

		RightFootModel_->worldTransform_->rotation_.x += 0.1f / 10;
		RightFootModel_->worldTransform_->rotation_.z -= 0.18f / 10;


		bodyModel_->worldTransform_->translation_.y += 0.1f / 10;
		bodyModel_->worldTransform_->rotation_.y -= 0.7f / 10;
		
	}


	if (MotionCount_ == 2)
	{
		behaviorRequest_ = Animation::kRoot;
		isUnderAttack = false;
		slashingEffect->StopMakeParticle();
	}
}



void Player::RootUpdate()
{
	//スキルのフラグ
	if (input_->PushKey(DIK_LSHIFT))
	{
		isSkill = true;
	}

	//スキルと攻撃の併用を禁止
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

	//スキルフラグとクールタイムが終わっていたら
	if (isSkill == true && isSkillCooldown_ == false)
	{
		if (playerLevel->nowskilllevel == 1)
		{
			behaviorRequest_ = Animation::kSkill1;
		}
		else if (playerLevel->nowskilllevel == 2)
		{
			behaviorRequest_ = Animation::kSkill2;
		}
		else if (playerLevel->nowskilllevel == 3)
		{
			behaviorRequest_ = Animation::kSkill3;
		}
		else
		{
			//該当するスキルなし
			isSkill = false;
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

	//歩行アニメーション
	MotionTimer_++;

	if (isMove == true)
	{
		if (MotionCount_ == 0)
		{
			
			MotionCount_ = 1;
		}
	}
	else
	{
		MotionTimer_ = 0;
		MotionCount_ = 0;

		LeftFootModel_->worldTransform_->rotation_.x = 0.0f;
		RightFootModel_->worldTransform_->rotation_.x = 0.0f;

		bodyModel_->worldTransform_->translation_.y = 0.0f;

	}
	
	

	if (MotionCount_ == 1)
	{
		if (MotionTimer_ == 20)
		{
			MotionCount_ = 2;
		}

		
		LeftFootModel_->worldTransform_->rotation_.x -= 0.2f / 10;
		RightFootModel_->worldTransform_->rotation_.x += 0.2f / 10;

		bodyModel_->worldTransform_->translation_.y += 0.02f;

	}

	if (MotionCount_ == 2)
	{
		if (MotionTimer_ == 40)
		{
			MotionCount_ = 3;
		}

		LeftFootModel_->worldTransform_->rotation_.x += 0.2f / 10;
		RightFootModel_->worldTransform_->rotation_.x -= 0.2f / 10;

		bodyModel_->worldTransform_->translation_.y -= 0.01f;

	}

	if (MotionCount_ == 3)
	{
		if (MotionTimer_ == 60)
		{
			MotionCount_ = 4;
		}

		LeftFootModel_->worldTransform_->rotation_.x += 0.2f / 10;
		RightFootModel_->worldTransform_->rotation_.x -= 0.2f / 10;

		bodyModel_->worldTransform_->translation_.y += 0.01f;

	}

	if (MotionCount_ == 4)
	{
		if (MotionTimer_ == 80)
		{
			MotionCount_ = 0;
			MotionTimer_ = 0;
		}

		LeftFootModel_->worldTransform_->rotation_.x -= 0.2f / 10;
		RightFootModel_->worldTransform_->rotation_.x += 0.2f / 10;

		bodyModel_->worldTransform_->translation_.y -= 0.01f;

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

	isSkill = false;
	isUnderAttack = false;

	weapon_->GetWorldTransform()->rotation_ = { 0.0f,0.0f,0.0f };


	bodyModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	bodyModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	headModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	headModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	LeftArmModel_->worldTransform_->translation_ = { 0.28f,2.38f,0.0f };
	LeftArmModel_->worldTransform_->rotation_ = { 0.0f,1.6f,0.72f };

	RightArmModel_->worldTransform_->translation_ = { -0.28f,2.38f,0.0f };
	RightArmModel_->worldTransform_->rotation_ = { 0.0f,-1.6f,-0.72f };

	LeftFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	LeftFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	RightFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	RightFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

}

void Player::AttackInitialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;


	
	weapon_->GetWorldTransform()->rotation_ = { 0.0f,0.0f,0.0f };


	bodyModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	bodyModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	headModel_->worldTransform_->translation_ = { 0.0f,0.0f,0.0f };
	headModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	LeftArmModel_->worldTransform_->translation_ = { 0.28f,2.38f,0.0f };
	LeftArmModel_->worldTransform_->rotation_ = { 0.0f,1.6f,0.72f };

	RightArmModel_->worldTransform_->translation_ = { -0.28f,2.38f,0.0f };
	RightArmModel_->worldTransform_->rotation_ = { 0.0f,-1.6f,-0.72f };

	LeftFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	LeftFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };

	RightFootModel_->worldTransform_->translation_ = { 0.0f,1.57f,0.0f };
	RightFootModel_->worldTransform_->rotation_ = { 0.0f,0.0f,0.0f };



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