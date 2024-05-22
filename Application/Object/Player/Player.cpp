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

	playerTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/base.png");
	headTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/head.png");
	bodyTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/body.png");
	HpTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/red.png");
	leftFootTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/leftFoot.png");
	rightFootTex = TextureManager::GetInstance()->LoadTexture("Resources/PlayerModel/rightFoot.png");
	ui_skill_padTex = TextureManager::GetInstance()->LoadTexture("Resources/UI_skill_pad.png");
	ui_skill_keyboardTex = TextureManager::GetInstance()->LoadTexture("Resources/UI_skill_keyboard.png");
	ui_playerLevelTex = TextureManager::GetInstance()->LoadTexture("Resources/UI_playerLevel.png");
	ui_hpTex = TextureManager::GetInstance()->LoadTexture("Resources/UI_Hp.png");
	ui_skillLevelTex = TextureManager::GetInstance()->LoadTexture("Resources/UI_skillLevel.png");

	hpSprite_.reset(Sprite::Create(HpTex));
	hpSprite_->SetPosition({ 20.0f,650.0f });
	hpSprite_->worldTransform_->scale_ = { float(HP),0.7f };
	//hpSpriteScale = { 130.0f,3.0f };

	ui_skill_pad.reset(Sprite::Create(ui_skill_padTex));
	//ui_skill_pad->SetPosition({ 20.0f,650.0f });
	ui_skill_pad->worldTransform_->scale_ = { 0.1f,0.1f };
	ui_skill_pad->SetisInvisible(true);

	ui_skill_keyboard.reset(Sprite::Create(ui_skill_keyboardTex));
	ui_skill_keyboard->SetPosition({ 1050.0f,550.0f });
	ui_skill_keyboard->worldTransform_->scale_ = { 0.1f,0.1f };
	ui_skill_keyboard->SetMaterialData({ 1.0f,1.0f,1.0f,0.5f });

	ui_playerLevel.reset(Sprite::Create(ui_playerLevelTex));
	ui_playerLevel->SetPosition({ 40.0f,5.0f });
	ui_playerLevel->worldTransform_->scale_ = { 0.1f,0.1f };
	//ui_playerLevel->SetisInvisible(true);

	ui_hp.reset(Sprite::Create(ui_hpTex));
	ui_hp->SetPosition({ 20.0f,660.0f });
	ui_hp->worldTransform_->scale_ = { 0.1f,0.1f };

	ui_skillLevel.reset(Sprite::Create(ui_skillLevelTex));
	ui_skillLevel->SetPosition({ 950.0f,50.0f });
	ui_skillLevel->worldTransform_->scale_ = { 0.1f,0.1f };

	ExperienceBar.reset(Model::Create("Resources/DefaultAssets/ExperienceBar.obj"));
	ExperienceText.reset(Model::Create("Resources/DefaultAssets/ExperienceText.obj"));
	ExperiencePoint.reset(Model::Create("Resources/DefaultAssets/ExperiencePoint.obj"));
	LevelMax.reset(Model::Create("Resources/DefaultAssets/LevelMax.obj"));
	SkillUI.reset(Model::Create("Resources/DefaultAssets/SkillUI.obj"));
	Skill1.reset(Model::Create("Resources/DefaultAssets/Skill1.obj"));
	Skill2.reset(Model::Create("Resources/DefaultAssets/Skill2.obj"));
	Skill3.reset(Model::Create("Resources/DefaultAssets/Skill3.obj"));
	LevelUp.reset(Model::Create("Resources/DefaultAssets/LevelUp.obj"));




	bodyModel_.reset(Model::Create("Resources/PlayerModel/body.obj"));
	headModel_.reset(Model::Create("Resources/PlayerModel/head.obj"));
	LeftArmModel_.reset(Model::Create("Resources/PlayerModel/leftArm.obj"));
	RightArmModel_.reset(Model::Create("Resources/PlayerModel/RightArm.obj"));
	LeftFootModel_.reset(Model::Create("Resources/PlayerModel/LeftFoot.obj"));
	RightFootModel_.reset(Model::Create("Resources/PlayerModel/RightFoot.obj"));


	bodyModel_->SetTexture(bodyTex);
	headModel_->SetTexture(headTex);
	LeftArmModel_->SetTexture(playerTex);
	RightArmModel_->SetTexture(playerTex);
	LeftFootModel_->SetTexture(leftFootTex);
	RightFootModel_->SetTexture(rightFootTex);



	slashingEffect = std::make_unique<SlashingEffect>();
	slashingEffect->Initialize(camera);
	//slashingEffect->StopMakeParticle();



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

	ExperienceText->worldTransform_->rotation_.x = 0.8f;
	ExperienceText->worldTransform_->rotation_.y = 3.14159265348979f;
	ExperienceText->worldTransform_->rotation_.z = -0.1f;

	ExperienceBar->worldTransform_->rotation_.x = 0.8f;
	ExperienceBar->worldTransform_->rotation_.y = 0;
	ExperienceBar->worldTransform_->rotation_.z = 0.1f;

	ExperiencePoint->worldTransform_->rotation_.x = 0.8f;
	ExperiencePoint->worldTransform_->rotation_.y = 0;
	ExperiencePoint->worldTransform_->rotation_.z = 0.1f;

	LevelMax->worldTransform_->rotation_.x = 0.8f;
	LevelMax->worldTransform_->rotation_.y = 3.14159265348979f;
	LevelMax->worldTransform_->rotation_.z = -0.1f;

	SkillUI->worldTransform_->rotation_.x = 0.8f;
	SkillUI->worldTransform_->rotation_.y = 3.14159265348979f;
	SkillUI->worldTransform_->rotation_.z = 0.1f;

	Skill1->worldTransform_->rotation_.x = 0.8f;
	Skill1->worldTransform_->rotation_.y = 3.14159265348979f;
	Skill1->worldTransform_->rotation_.z = 0.1f;

	Skill2->worldTransform_->rotation_.x = 0.8f;
	Skill2->worldTransform_->rotation_.y = 3.14159265348979f;
	Skill2->worldTransform_->rotation_.z = 0.1f;

	Skill3->worldTransform_->rotation_.x = 0.8f;
	Skill3->worldTransform_->rotation_.y = 3.14159265348979f;
	Skill3->worldTransform_->rotation_.z = 0.1f;

	ExperienceBar->worldTransform_->scale_.x = 0.5f;
	ExperienceBar->worldTransform_->scale_.y = 0.5f;
	ExperienceText->worldTransform_->scale_.x = 0.5f;
	ExperienceText->worldTransform_->scale_.y = 0.5f;
	ExperiencePoint->worldTransform_->scale_.x = 0.5f;
	ExperiencePoint->worldTransform_->scale_.y = 0.5f;
	LevelMax->worldTransform_->scale_.x = 0.5f;
	LevelMax->worldTransform_->scale_.y = 0.5f;
	SkillUI->worldTransform_->scale_.x = 0.5f;
	SkillUI->worldTransform_->scale_.y = 0.5f;
	Skill1->worldTransform_->scale_.x = 0.5f;
	Skill1->worldTransform_->scale_.y = 0.5f;
	Skill2->worldTransform_->scale_.x = 0.5f;
	Skill2->worldTransform_->scale_.y = 0.5f;
	Skill3->worldTransform_->scale_.x = 0.5f;
	Skill3->worldTransform_->scale_.y = 0.5f;
	

	ExperienceBar->worldTransform_->translation_.x = -3.5f;
	ExperienceBar->worldTransform_->translation_.y = 5.0f;
	ExperienceText->worldTransform_->translation_.x = -3.5f;
	ExperienceText->worldTransform_->translation_.y = 5.0f;
	ExperiencePoint->worldTransform_->translation_.x = -3.5f;
	ExperiencePoint->worldTransform_->translation_.y = 5.0f;
	LevelMax->worldTransform_->translation_.x = -3.5f;
	LevelMax->worldTransform_->translation_.y = 5.0f;
	SkillUI->worldTransform_->translation_.x = 3.5f;
	SkillUI->worldTransform_->translation_.y = 6.0f;
	Skill1->worldTransform_->translation_.x = 3.5f;
	Skill1->worldTransform_->translation_.y = 6.0f;
	Skill2->worldTransform_->translation_.x = 3.5f;
	Skill2->worldTransform_->translation_.y = 6.0f;
	Skill3->worldTransform_->translation_.x = 3.5f;
	Skill3->worldTransform_->translation_.y = 6.0f;




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

	isHit = false;


}

void Player::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();	//移動用モデル

	ExperienceBar->Update();
	ExperienceText->Update();
	ExperiencePoint->Update();
	LevelMax->Update();
	SkillUI->Update();
	Skill1->Update();
	Skill2->Update();
	Skill3->Update();
	LevelUp->Update();

	bodyModel_->Update();
	headModel_->Update();
	LeftArmModel_->Update();
	RightArmModel_->Update();
	LeftFootModel_->Update();
	RightFootModel_->Update();

	ExperiencePoint->worldTransform_->scale_.x = playerLevel->Experiencepoint * 234.0f;

	if (ExperiencePoint->worldTransform_->scale_.x >= 24800.0f) {
		level += 1;
		ExperiencePoint->worldTransform_->scale_.x = 0.5f;
		LevelUp->worldTransform_->translation_.y = 2.00f;
		levelup = true;
	}
	if (levelup == true) {
		levelupcount++;
		LevelUp->worldTransform_->translation_.y += 0.05f;
		LevelUp->worldTransform_->rotation_.y -= 0.05f;
		if (levelupcount >= 180) {
			LevelUp->worldTransform_->translation_.y = 0.0f;
			LevelUp->worldTransform_->rotation_.y = 0.0f;
			levelupcount = 0;
			levelup = false;
		}
	}
	if (levelupcount == 0) {
		levelup = false;
	}

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
	ImGui::Text("MotionCount : %d", MotionCount_);
	ImGui::Text("MotionTimer : %d", MotionTimer_);
	ImGui::End();
	ImGui::Begin("levelupflag");
	ImGui::Text("levelupflag = %d", levelup);
	ImGui::Text("levelupcount = %d", levelupcount);
	ImGui::End();
	ExperienceBar->ModelDebug("bar");
	ExperienceText->ModelDebug("text");
	ExperiencePoint->ModelDebug("point");
	LevelMax->ModelDebug("max");
	SkillUI->ModelDebug("UI");
	Skill1->ModelDebug("1");
	Skill2->ModelDebug("2");
	Skill3->ModelDebug("3");
	LevelUp->ModelDebug("levelup");

	bodyModel_->ModelDebug("body");
	headModel_->ModelDebug("head");
	LeftArmModel_->ModelDebug("leftArm");
	RightArmModel_->ModelDebug("rightArm");
	LeftFootModel_->ModelDebug("leftFoot");
	RightFootModel_->ModelDebug("rightFoot");


	ImGui::Text("SkillFlag : %d", isSkill);

#endif // _DEBUG

	// 剣の先端の座標
	Vector3 tipPosition =
	{
		weapon_->GetWorldPosition().x,
		weapon_->GetWorldPosition().y,
		weapon_->GetWorldPosition().z,

	};
	// 剣の基点（柄など）の座標
	Vector3 basePosition = weapon_->GetWorldPosition();

	// 剣の方向ベクトルを計算
	Vector3 swordDirection = Subtract(tipPosition, basePosition);

	// 剣の長さ（方向ベクトルの長さ）を計算
	float swordLength = Length(swordDirection);

	Vector3 particleOffsetDistance = { 0.5f,0.0f,0.5f };

	// パーティクルの発生位置を計算（例: 剣の先端から一定距離離れた位置）
	Vector3 particleStartPosition = Subtract(Add(tipPosition, swordDirection), particleOffsetDistance);

	//slashingEffect->SetPosition(particleStartPosition);
	slashingEffect->Update();

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

	ui_skill_keyboard->Debug("UI_Keyboard_skill");
	ui_playerLevel->Debug("UI_playerlevel");
	ui_skillLevel->Debug("UI_skillLevel");
	ui_hp->Debug("UI_HP");

	hpSprite_->Update();
	ui_skill_pad->Update();
	ui_skill_keyboard->Update();
	ui_playerLevel->Update();
	ui_hp->Update();
	ui_skillLevel->Update();

	float scaleX = static_cast<float>(HP) / 1000.0f;
	if (scaleX >= 5.0f)
	{
		scaleX = 5.0f;
	}
	else if (scaleX <= 0.0f)
	{
		scaleX = 0.0f;
	}

	hpSprite_->worldTransform_->scale_ = { scaleX,0.7f };
}

void Player::Draw()
{
	model_->Draw(camera_);

	ExperienceBar->Draw(camera_);
	ExperienceText->Draw(camera_);
	ExperiencePoint->Draw(camera_);
	if (level >= 18) {
		LevelMax->Draw(camera_);
	}
	SkillUI->Draw(camera_);
	if (level >= 3) {
		Skill1->Draw(camera_);
	}
	if (level >= 6) {
		Skill2->Draw(camera_);
	}
	if (level >= 10) {
		Skill3->Draw(camera_);
	}
	LevelUp->Draw(camera_);
	bodyModel_->Draw(camera_);
	headModel_->Draw(camera_);
	LeftArmModel_->Draw(camera_);
	RightArmModel_->Draw(camera_);
	LeftFootModel_->Draw(camera_);
	RightFootModel_->Draw(camera_);


}

void Player::TextureDraw()
{

	playerLevel->Draw();

	slashingEffect->Draw();
	hpSprite_->Draw(camera_);

	ui_skill_pad->Draw(camera_);
	ui_skill_keyboard->Draw(camera_);
	//ui_playerLevel->Draw(camera_);
	ui_hp->Draw(camera_);
	/*ui_skillLevel->Draw(camera_);*/
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
		isHit = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet))
	{
		//敵の弾にあたったら
		HP -= 200;
		isCoolDown = true;
		isHit = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kBoss))
	{
		//敵の弾にあたったら
		HP -= 100;
		isCoolDown = true;
		isHit = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kBossBullet))
	{
		//敵の弾にあたったら
		HP -= 200;
		isCoolDown = true;
	}

}

void Player::Move()
{
	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	Vector3 move = { 0.0f,0.0f,0.0f };
	isPlayWalkAnimation = false;	//歩行アニメーション

	// 剣の基点（柄など）の座標
	Vector3 basePosition =
	{
		weapon_->GetWorldPosition().x + 0.7f,
		weapon_->GetWorldPosition().y ,
		weapon_->GetWorldPosition().z - 0.5f
	};

	// 剣の基点（柄など）の座標
	Vector3 backBasePosition =
	{
		weapon_->GetWorldPosition().x - 0.5f,
		weapon_->GetWorldPosition().y,
		weapon_->GetWorldPosition().z - 0.5f
	};

	// 剣の基点（柄など）の座標
	Vector3 leftBasePosition =
	{
		weapon_->GetWorldPosition().x - 0.5f ,
		weapon_->GetWorldPosition().y ,
		weapon_->GetWorldPosition().z - 1.2f
	};

	// 剣の基点（柄など）の座標
	Vector3 rightBasePosition =
	{
		weapon_->GetWorldPosition().x + 0.5f,
		weapon_->GetWorldPosition().y,
		weapon_->GetWorldPosition().z - 0.3f
	};




	//移動

	if (input_->PushKey(DIK_W))
	{
		slashingEffect->SetPosition(backBasePosition);
		slashingEffect->SetVelocity({ -0.7f,0.0f,0.0f });
		slashingEffect->SetVelocityX(false);
		slashingEffect->SetVelocityZ(true);
		move.z = PlayerSpeed;
		isPlayWalkAnimation = true;

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}
	if (input_->PushKey(DIK_S))
	{
		slashingEffect->SetPosition(basePosition);
		slashingEffect->SetVelocity({ 0.3f,0.0f,0.0f });
		slashingEffect->SetVelocityX(false);
		slashingEffect->SetVelocityZ(true);
		move.z = -PlayerSpeed;
		isPlayWalkAnimation = true;

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}
	if (input_->PushKey(DIK_A))
	{
		slashingEffect->SetPosition(leftBasePosition);
		slashingEffect->SetVelocity({ 0.0f,0.0f,-0.5f });
		slashingEffect->SetVelocityZ(false);
		slashingEffect->SetVelocityX(true);
		move.x = -PlayerSpeed;
		isPlayWalkAnimation = true;

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}
	if (input_->PushKey(DIK_D))
	{
		slashingEffect->SetPosition(rightBasePosition);
		slashingEffect->SetVelocity({ 0.2f,0.0f,0.7f });
		slashingEffect->SetVelocityZ(false);
		slashingEffect->SetVelocityX(true);
		move.x = PlayerSpeed;
		isPlayWalkAnimation = true;

		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);

	}

	// Y軸周り角度(θy)	歩いている方向に顔を向ける
	model_->worldTransform_->rotation_.y = LerpShortAngle(model_->worldTransform_->rotation_.y, angle_, 0.1f);
	model_->worldTransform_->translation_.x += move.x;
	model_->worldTransform_->translation_.z += move.z;

	ExperienceBar->worldTransform_->translation_.x += move.x;
	ExperienceBar->worldTransform_->translation_.z += move.z;
	ExperiencePoint->worldTransform_->translation_.x += move.x;
	ExperiencePoint->worldTransform_->translation_.z += move.z;
	ExperienceText->worldTransform_->translation_.x += move.x;
	ExperienceText->worldTransform_->translation_.z += move.z;
	LevelMax->worldTransform_->translation_.x += move.x;
	LevelMax->worldTransform_->translation_.z += move.z;
	SkillUI->worldTransform_->translation_.x += move.x;
	SkillUI->worldTransform_->translation_.z += move.z;
	Skill1->worldTransform_->translation_.x += move.x;
	Skill1->worldTransform_->translation_.z += move.z;
	Skill2->worldTransform_->translation_.x += move.x;
	Skill2->worldTransform_->translation_.z += move.z;
	Skill3->worldTransform_->translation_.x += move.x;
	Skill3->worldTransform_->translation_.z += move.z;
	LevelUp->worldTransform_->translation_.x += move.x;
	LevelUp->worldTransform_->translation_.z += move.z;



	if (input_->GetJoystickState(0, joyState))
	{
		const float threshold = 0.9f;
		bool isMoving = false;
		move = { 0,0,0 };

		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0)
		{
			// 移動量
			move =
			{
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
				0.0f,
				(float)joyState.Gamepad.sThumbLY / SHRT_MAX
			};

			float inputMagnitude = Length(move);

			// スティックの入力が一定の閾値以上の場合のみ移動処理を実行
			if (inputMagnitude > threshold)
			{
				isMoving = true;

				// スティックの入力に応じて速度を調整する
				float adjustedSpeed = PlayerSpeed * inputMagnitude;

				// 最大速度を超えないようにする
				if (adjustedSpeed > 0.05f)
				{
					adjustedSpeed = 0.05f;
				}

				// 実際の移動量を計算
				move.x *= adjustedSpeed;
				move.z *= adjustedSpeed;

				// 歩行アニメーションを開始
				isPlayWalkAnimation = true;

				// 目標角度の算出
				angle_ = std::atan2(move.x, move.z);


				// Y軸周り角度(θy)	歩いている方向に顔を向ける
				model_->worldTransform_->rotation_.y = LerpShortAngle(model_->worldTransform_->rotation_.y, angle_, 0.1f);
				model_->worldTransform_->translation_.x += move.x;
				model_->worldTransform_->translation_.z += move.z;

				ExperienceBar->worldTransform_->translation_.x += move.x;
				ExperienceBar->worldTransform_->translation_.z += move.z;
				ExperiencePoint->worldTransform_->translation_.x += move.x;
				ExperiencePoint->worldTransform_->translation_.z += move.z;
				ExperienceText->worldTransform_->translation_.x += move.x;
				ExperienceText->worldTransform_->translation_.z += move.z;
				LevelMax->worldTransform_->translation_.x += move.x;
				LevelMax->worldTransform_->translation_.z += move.z;
				SkillUI->worldTransform_->translation_.x += move.x;
				SkillUI->worldTransform_->translation_.z += move.z;
				Skill1->worldTransform_->translation_.x += move.x;
				Skill1->worldTransform_->translation_.z += move.z;
				Skill2->worldTransform_->translation_.x += move.x;
				Skill2->worldTransform_->translation_.z += move.z;
				Skill3->worldTransform_->translation_.x += move.x;
				Skill3->worldTransform_->translation_.z += move.z;
				LevelUp->worldTransform_->translation_.x += move.x;
				LevelUp->worldTransform_->translation_.z += move.z;
			}
		}
	}





	CoolDown();

}

void Player::Attack()
{

	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			isUnderAttack = true;
		}
	}

	if (input_->IsLeftMouseClicked()) {
		isUnderAttack = true;
	}


	if (isUnderAttack == true)
	{
		behaviorRequest_ = Animation::kAttack;
		slashingEffect->SetFlag(true);
		slashingEffect->StartParticle();
	}
}

void Player::Skill()
{
	XINPUT_STATE joyState;


	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		isSkill = true;
	}


	if (input_->PushKey(DIK_LSHIFT))
	{
		isSkill = true;
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
	XINPUT_STATE joyState;
	//剣を振りかぶる
	Input::GetInstance()->GetJoystickState(0, joyState);
	//移動
	//Move();

	MotionTimer_++;

	if (MotionCount_ == 0)
	{
		if (MotionTimer_ == 10)
		{
			MotionCount_ = 1;
		}

		weapon_->GetWorldTransform()->rotation_.x += 0.252f;
		weapon_->GetWorldTransform()->rotation_.y += 0.3f;


		LeftFootModel_->worldTransform_->rotation_.x += 0.1f / 10;
		LeftFootModel_->worldTransform_->rotation_.z -= 0.18f / 10;

		RightFootModel_->worldTransform_->rotation_.x -= 0.1f / 10;
		RightFootModel_->worldTransform_->rotation_.z += 0.18f / 10;

		bodyModel_->worldTransform_->rotation_.y -= 0.2f / 10;
	}

	if (MotionCount_ == 1 && input_->IsLeftMouseClicked() && MotionTimer_ >= 10 && MotionTimer_ <= 60) {
		MotionCount_ = 2;
		MotionTimer_ = 60;
		weapon_->GetWorldTransform()->rotation_.x = 2.83f;
		weapon_->GetWorldTransform()->rotation_.y = 6.5f;
		weapon_->GetWorldTransform()->rotation_.z = 0;
	}


	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && MotionCount_ == 1 && MotionTimer_ >= 10 && MotionTimer_ <= 60) {
		MotionCount_ = 2;
		MotionTimer_ = 60;
		weapon_->GetWorldTransform()->rotation_.x = 2.83f;
		weapon_->GetWorldTransform()->rotation_.y = 6.5f;
		weapon_->GetWorldTransform()->rotation_.z = 0;
	}


	if (MotionCount_ == 1 && MotionTimer_ >= 60) {
		MotionCount_ = 99;
	}

	if (MotionCount_ == 2) {
		if (MotionTimer_ == 70) {
			MotionCount_ = 3;
		}
		//ここに１コンボ目の剣の動きを書く

		weapon_->GetWorldTransform()->rotation_.y -= 0.331f;
		//LeftFootModel_->worldTransform_->rotation_.x += 0.1f / 10;
		//LeftFootModel_->worldTransform_->rotation_.z -= 0.18f / 10;

		//RightFootModel_->worldTransform_->rotation_.x -= 0.1f / 10;
		//RightFootModel_->worldTransform_->rotation_.z += 0.18f / 10;

		bodyModel_->worldTransform_->rotation_.y += 0.2f / 10;

	}
	if (MotionCount_ == 3 && input_->IsLeftMouseClicked() && MotionTimer_ >= 80 && MotionTimer_ <= 130) {
		MotionCount_ = 4;
		MotionTimer_ = 130;
		weapon_->GetWorldTransform()->rotation_.x = 3.0f;
		weapon_->GetWorldTransform()->rotation_.y = 2.7f;
		weapon_->GetWorldTransform()->rotation_.z = -0.24f;
	}

	if (MotionCount_ == 3 && joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && MotionTimer_ >= 80 && MotionTimer_ <= 130) {
		MotionCount_ = 4;
		MotionTimer_ = 130;
		weapon_->GetWorldTransform()->rotation_.x = 3.0f;
		weapon_->GetWorldTransform()->rotation_.y = 2.7f;
		weapon_->GetWorldTransform()->rotation_.z = -0.24f;
	}


	if (MotionCount_ == 3 && MotionTimer_ >= 130) {
		MotionCount_ = 99;
	}
	if (MotionCount_ == 4) {
		if (MotionTimer_ == 180) {
			MotionCount_ = 99;
		}
		//ここに2コンボ目の剣の動きを書く
		weapon_->GetWorldTransform()->rotation_.y += 0.0862f;
		/*	LeftFootModel_->worldTransform_->rotation_.x += 0.1f / 10;
			LeftFootModel_->worldTransform_->rotation_.z -= 0.18f / 10;

			RightFootModel_->worldTransform_->rotation_.x -= 0.1f / 10;
			RightFootModel_->worldTransform_->rotation_.z += 0.18f / 10;*/

		bodyModel_->worldTransform_->rotation_.y -= 0.3f / 50;
		Speed = 0.05f;

	}

	if (MotionCount_ == 99)
	{
		Speed = 0.03f;
		behaviorRequest_ = Animation::kRoot;
		isUnderAttack = false;
	}
}



void Player::RootUpdate()
{
	//スキルのフラグ
	if (input_->PushKey(DIK_LSHIFT) && isSkillCooldown_ == false)
	{
		isSkill = true;
		ui_skill_keyboard->SetMaterialData({ 1.0f,1.0f,1.0f,0.5f });
	}

	if (playerLevel->nowskilllevel == 1 && isSkillCooldown_ == false && isSkill == false)
	{
		ui_skill_keyboard->SetMaterialData({ 1.0f,1.0f,1.0f,1.0f });
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
	//被弾時硬直

	//スキルフラグとクールタイムが終わっていたら
	if (isSkill == true)
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
			ui_skill_keyboard->SetMaterialData({ 1.0f,1.0f,1.0f,1.0f });
			//isSkill = false;

		}
	}

	//歩行アニメーション
	MotionTimer_++;

	if (isPlayWalkAnimation == true)
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



		LeftFootModel_->worldTransform_->rotation_.x = LerpShortTranslate(LeftFootModel_->worldTransform_->rotation_.x, 0.0f, 0.2f);
		RightFootModel_->worldTransform_->rotation_.x = LerpShortTranslate(RightFootModel_->worldTransform_->rotation_.x, 0.0f, 0.2f);

		bodyModel_->worldTransform_->translation_.y = LerpShortTranslate(bodyModel_->worldTransform_->rotation_.y, 0.0f, 0.2f);

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

		bodyModel_->worldTransform_->translation_.y -= 0.02f;

	}

	if (MotionCount_ == 3)
	{
		if (MotionTimer_ == 60)
		{
			MotionCount_ = 4;
		}

		LeftFootModel_->worldTransform_->rotation_.x += 0.2f / 10;
		RightFootModel_->worldTransform_->rotation_.x -= 0.2f / 10;

		bodyModel_->worldTransform_->translation_.y += 0.02f;

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

		bodyModel_->worldTransform_->translation_.y -= 0.02f;

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

		float dashSpeed = 0.3f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;
		ExperienceBar->worldTransform_->translation_.x += dashX;
		ExperienceBar->worldTransform_->translation_.z += dashZ;
		ExperiencePoint->worldTransform_->translation_.x += dashX;
		ExperiencePoint->worldTransform_->translation_.z += dashZ;
		ExperienceText->worldTransform_->translation_.x += dashX;
		ExperienceText->worldTransform_->translation_.z += dashZ;
		LevelMax->worldTransform_->translation_.x += dashX;
		LevelMax->worldTransform_->translation_.z += dashZ;
		SkillUI->worldTransform_->translation_.x += dashX;
		SkillUI->worldTransform_->translation_.z += dashZ;
		Skill1->worldTransform_->translation_.x += dashX;
		Skill1->worldTransform_->translation_.z += dashZ;
		Skill2->worldTransform_->translation_.x += dashX;
		Skill2->worldTransform_->translation_.z += dashZ;
		Skill3->worldTransform_->translation_.x += dashX;
		Skill3->worldTransform_->translation_.z += dashZ;
		LevelUp->worldTransform_->translation_.x += dashX;
		LevelUp->worldTransform_->translation_.z += dashZ;

		//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;
	}

	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する

		isSkillCooldown_ = true;
		skillCooldownTime_ = 60 * 1;
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
		if (MotionTimer_ == 40)
		{
			MotionCount_ = 1;
		}

		float directionAngle = model_->worldTransform_->rotation_.y;

		float dashSpeed = 0.2f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;

		weapon_->GetWorldTransform()->rotation_.x = 3.0f;
		weapon_->GetWorldTransform()->rotation_.y += 1.0f;
		weapon_->GetWorldTransform()->rotation_.z = -0.2f;
		if (MotionTimer_ <= 10) {
			weapon_->GetWorldTransform()->translation_.z += 0.5f;
		}
		if (10 <= MotionTimer_) {
			weapon_->GetWorldTransform()->translation_.z -= 0.1f;
		}
		if (weapon_->GetWorldTransform()->rotation_.y >= 6.28f) {
			weapon_->GetWorldTransform()->rotation_.y = 0.0f;
		}

		if (weapon_->GetWorldTransform()->translation_.z >= 4.0f) {
			weapon_->GetWorldTransform()->translation_.z = 4.0f;
		}
		ExperienceBar->worldTransform_->translation_.x += dashX;
		ExperienceBar->worldTransform_->translation_.z += dashZ;
		ExperiencePoint->worldTransform_->translation_.x += dashX;
		ExperiencePoint->worldTransform_->translation_.z += dashZ;
		ExperienceText->worldTransform_->translation_.x += dashX;
		ExperienceText->worldTransform_->translation_.z += dashZ;
		LevelMax->worldTransform_->translation_.x += dashX;
		LevelMax->worldTransform_->translation_.z += dashZ;
		SkillUI->worldTransform_->translation_.x += dashX;
		SkillUI->worldTransform_->translation_.z += dashZ;
		Skill1->worldTransform_->translation_.x += dashX;
		Skill1->worldTransform_->translation_.z += dashZ;
		Skill2->worldTransform_->translation_.x += dashX;
		Skill2->worldTransform_->translation_.z += dashZ;
		Skill3->worldTransform_->translation_.x += dashX;
		Skill3->worldTransform_->translation_.z += dashZ;
		LevelUp->worldTransform_->translation_.x += dashX;
		LevelUp->worldTransform_->translation_.z += dashZ;
			//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;

	}


	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する
		isSkillCooldown_ = true;
		skillCooldownTime_ = 60 * 5;
		isSkillAttack = false;

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

		float dashSpeed = 0.2f;

		float dashX = std::sin(directionAngle) * dashSpeed;
		float dashZ = std::cos(directionAngle) * dashSpeed;

		model_->worldTransform_->translation_.x += dashX;
		model_->worldTransform_->translation_.z += dashZ;

		weapon_->GetWorldTransform()->rotation_.x = 3.0f;
		weapon_->GetWorldTransform()->rotation_.y += 1.0f;
		weapon_->GetWorldTransform()->rotation_.z = -0.2f;
		weapon_->GetWorldTransform()->translation_.z += 0.5f;

		if (weapon_->GetWorldTransform()->rotation_.y >= 6.28f) {
			weapon_->GetWorldTransform()->rotation_.y = 0.0f;
		}

		if (weapon_->GetWorldTransform()->translation_.z >= 4.0f) {
			weapon_->GetWorldTransform()->translation_.z = 4.0f;
		}
		ExperienceBar->worldTransform_->translation_.x += dashX;
		ExperienceBar->worldTransform_->translation_.z += dashZ;
		ExperiencePoint->worldTransform_->translation_.x += dashX;
		ExperiencePoint->worldTransform_->translation_.z += dashZ;
		ExperienceText->worldTransform_->translation_.x += dashX;
		ExperienceText->worldTransform_->translation_.z += dashZ;
		LevelMax->worldTransform_->translation_.x += dashX;
		LevelMax->worldTransform_->translation_.z += dashZ;
		SkillUI->worldTransform_->translation_.x += dashX;
		SkillUI->worldTransform_->translation_.z += dashZ;
		Skill1->worldTransform_->translation_.x += dashX;
		Skill1->worldTransform_->translation_.z += dashZ;
		Skill2->worldTransform_->translation_.x += dashX;
		Skill2->worldTransform_->translation_.z += dashZ;
		Skill3->worldTransform_->translation_.x += dashX;
		Skill3->worldTransform_->translation_.z += dashZ;
		LevelUp->worldTransform_->translation_.x += dashX;
		LevelUp->worldTransform_->translation_.z += dashZ;
		//カメラ直書き
		camera_->transform.translate.x += dashX;
		camera_->transform.translate.z += dashZ;

	}

	if (MotionCount_ == 1)
	{
		behaviorRequest_ = Animation::kRoot;
		// スキル使用後、クールダウンを開始する
		isSkillCooldown_ = true;
		skillCooldownTime_ = 60 * 5;
		isSkillAttack = false;

	}

}

void Player::RootInitialize()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;

	isSkill = false;
	isUnderAttack = false;

	weapon_->GetWorldTransform()->rotation_ = { 0.0f,0.0f,-2.4f };
	weapon_->GetWorldTransform()->translation_ = { 0.0f,1.66f,0.87f };


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



	weapon_->GetWorldTransform()->rotation_ = { 0.0f,0.0f,-2.4f };
	weapon_->GetWorldTransform()->translation_ = { 0.0f,1.66f,0.87f };

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

	isSkillAttack = true;
}

void Player::Skill3Initialzie()
{
	MotionTimer_ = 0;
	MotionCount_ = 0;

	isSkillAttack = true;
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