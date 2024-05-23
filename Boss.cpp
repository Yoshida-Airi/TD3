#include "Boss.h"
#include"CollisionConfig.h"
#include"TextureManager.h"
Boss::~Boss() {

}

void Boss::Initialize(Player* player, BossBullet* bullet) {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kBoss));

	bossTex = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a2.png");

	model_.reset(Model::Create("Resources/Enemy/ene.obj"));
	model_->SetTexture(bossTex);

	input_ = Input::GetInstance();
	input_->Initialize();

	bullet_ = bullet;
	bullet_->Initialize();

	currentTime = int(time(nullptr));
	srand(currentTime);

	SetRadius(model_->worldTransform_->scale_);
	model_->worldTransform_->translation_ = { player->GetPosition().x, 6.0f, player->GetPosition().z};
	hp = 100;

	player_ = player;

}

void Boss::Update(SceneManager* scene) {
	input_->Update();

	model_->Update();

	Collider::UpdateWorldTransform();

	if (model_->worldTransform_->translation_.y <= 0.0f)
	{

		NextAction();

		switch (bAction)
		{
		case MOVE:

			Move();

			break;

		case SHOT:

			Shot();
			bullet_->Update();

			break;

		case TACKLE:

			Tackle();

			break;

		}
	}
	else {
		model_->worldTransform_->translation_.y -= 0.05f;
	}

	Dead(scene);

	CoolDown();

	/*if (--deathTimer <= 0) {
		isDead_ = true;
	}*/

#ifdef _DEBUG
	ImGui::Begin("Boss");
	ImGui::Text("HP : %d", hp);
	ImGui::Text("Action : %d", isNextAction);
	ImGui::Text("NextA : %d", action);
	ImGui::Text("NTimer : %d", nextActionTimer);
	ImGui::End();
#endif // _DEBUG

	
}

void Boss::Draw(Camera* camera) {

	model_->Draw(camera);

	if (isBAlive == true) {
		bullet_->Draw(camera);
	}

}

void Boss::Finalize() {

}

void Boss::SetTranslate(Vector3 translate) {
	
	model_->worldTransform_->translation_ = { translate.x + 0.00000001f ,6.0f, translate.z };
}

Vector3 Boss::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

void Boss::CoolDown() {
	if (isCoolDown == true) {
		coolDownTimer++;
	}

	if (coolDownTimer == 120) {
		isCoolDown = false;
		coolDownTimer = 0;
	}

}

void Boss::Move() {
	Direction(0.04f);
	model_->worldTransform_->translation_.x += speed_.x;
	model_->worldTransform_->translation_.y += speed_.y;
	model_->worldTransform_->translation_.z += speed_.z;
}

void Boss::Dead(SceneManager* scene) {
	if (hp <= 0)
	{
		isDead_ = true;
		//scene->ChangeScene("GAMECLEAR");
	}
}

void Boss::Shot() {
	if (aimTimer <= 90 && isAttack == false) {
		aimTimer++;
		Direction(0.1f);
	}
	else {
		aimTimer = 0;
		isAttack = true;
	}

	if (isAttack == true && isAssignment == false) {
		bullet_->SetSpeed(speed_);
		bullet_->SetTranslate(model_->worldTransform_->translation_);
		isAssignment = true;
		isBAlive = true;
	}

	if (isAssignment == true && isNextAction == false) {
		BTimer++;
	}

	if (BTimer >= 60) {
		isNextAction = true;
		isBAlive = false;
		BTimer = 0;
	}
}

void Boss::Tackle() {
	if (aimTimer <= 90 && isAttack == false) {
		aimTimer++;
		Direction(0.5f);
	}
	else {
		aimTimer = 0;
		isAttack = true;
	}

	if (isAttack == true && isAssignment == false) {
		isAssignment = true;
	}

	if (isAssignment == true && isNextAction == false) {
		BTimer++;
		if (BTimer <= 30) {
			model_->worldTransform_->translation_.x -= speed_.x / 10.0f;
			model_->worldTransform_->translation_.y -= speed_.y / 10.0f;
			model_->worldTransform_->translation_.z -= speed_.z / 10.0f;
		}
		else if(BTimer > 30 && BTimer <= 55) {
			model_->worldTransform_->translation_.x += speed_.x;
			model_->worldTransform_->translation_.y += speed_.y;
			model_->worldTransform_->translation_.z += speed_.z;
		}
		else if (BTimer > 55) {
			model_->worldTransform_->translation_.x += speed_.x / 10.0f;
			model_->worldTransform_->translation_.y += speed_.y / 10.0f;
			model_->worldTransform_->translation_.z += speed_.z / 10.0f;
		}
	}

	if (BTimer >= 85) {
		isNextAction = true;
		BTimer = 0;
	}
}

void Boss::NextAction() {
	if (isNextAction == true) {
		nextActionTimer++;
	}

	if (nextActionTimer >= 60) {
		action = rand() % 3;
		nextActionTimer = 0;
		bullet_->SetScale({ 0.5f,0.5f,0.5f });
		isNext = true;
	}

	if (action == 0 && isNext == true) {
		bAction = MOVE;
		isNext = false;
	}
	else if (action == 1 && isNext == true) {
		bAction = SHOT;
		isNextAction = false;
		isNext = false;
		isAttack = false;
		isAssignment = false;
	}
	else if (action == 2 && isNext == true) {
		bAction = TACKLE;
		isNextAction = false;
		isNext = false;
		isAttack = false;
		isAssignment = false;
	}

}

void Boss::Direction(float speed) {
	const float kBulletSpeed = speed;
	Vector3 playerPos = player_->GetPosition();
	Vector3 enemyPos = model_->worldTransform_->translation_;

	const float threshold = 0.7f;
	Vector3 move = { 0.0f,0.0f,0.0f };
	bool isMoveing = false;

	speed_.x = playerPos.x - enemyPos.x;
	//speed_.y = playerPos.y - enemyPos.y;
	speed_.z = playerPos.z - enemyPos.z;

	speed_ = Normalize(speed_);

	speed_.x *= kBulletSpeed;
	//speed_.y *= kBulletSpeed;
	speed_.z *= kBulletSpeed;

	//目標角度の算出
	angle_ = std::atan2(speed_.x, speed_.z);

	model_->worldTransform_->rotation_.y = LerpShortAngle(model_->worldTransform_->rotation_.y, angle_, 0.1f);

}

void Boss::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		hp -= player_->AttackPower;
		isCoolDown = true;
	}


}

float Boss::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}

// 最短角度補間
float Boss::LerpShortAngle(float a, float b, float t)
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

float Boss::LerpShortTranslate(float a, float b, float t) {
	return a + t * (b - a);
}