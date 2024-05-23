#include "Enemy.h"
#include"CollisionConfig.h"

Enemy::~Enemy() {

}

void Enemy::Initialize(Player* player) {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kEnemy));

	texture = TextureManager::GetInstance()->LoadTexture("Resources/Enemy/a.png");

	model_.reset(Model::Create("Resources/Enemy/ene.obj"));
	model_->SetTexture(texture);

	input_ = Input::GetInstance();
	input_->Initialize();

	model_->worldTransform_->scale_ = { 0.3f,0.3f,0.3f };

	SetRadius(model_->worldTransform_->scale_);
	player_ = player;

	currentTime = int(time(nullptr));
	srand(currentTime);

	hitSound[0] = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Hit.wav");
	hitSound[1] = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Hit2.wav");
	hitSound[2] = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Hit3.wav");

	isRelottery = false;
}

void Enemy::Update() {
	input_->Update();

	model_->Update();

	Collider::UpdateWorldTransform();

	Direction();
	LotteryHitSound();

	model_->worldTransform_->translation_.x += 0.001f;

	CoolDown();
	/*if (--deathTimer <= 0) {
		isDead_ = true;
	}*/

	HitAction();

#ifdef _DEBUG
	ImGui::Begin("EnemyHP");
	ImGui::Text("%d", EnemyHP);
	ImGui::End();
#endif // _DEBUG


}

void Enemy::Draw(Camera* camera) {

	model_->Draw(camera);

}

void Enemy::Finalize() {
	
	
}

void Enemy::SetTranslate(std::mt19937& randomEngine, Vector3 translate) {
	std::uniform_real_distribution<float> translateX (-7.0f, 7.0f);
	std::uniform_real_distribution<float> translateZ (-7.0f, 7.0f);

	model_->worldTransform_->translation_ = { translate.x + translateX(randomEngine),0.0f, translate.z + translateZ(randomEngine) };
}

void Enemy::CoolDown()
{
	if (isCoolDown == true) {
		coolDownTimer++;
	}

	if (coolDownTimer == 30) {
		isCoolDown = false;
		coolDownTimer = 0;
	}
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

void Enemy::Direction() {
	const float kBulletSpeed = 0.04f;
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


void Enemy::HitAction()
{
	if (isplayHitAction == true)
	{
		MotionTimer++;
		
		if (motionCount == 0)
		{
			model_->SetColor({ 1.0f,0.0f,1.0f,0.5f });

			if (MotionTimer >= 20)
			{
				motionCount = 1;
			}
		}

		if (motionCount == 1)
		{
			model_->SetColor({ 1.0f,0.0f,1.0f,0.8f });

			if (MotionTimer >= 40)
			{
				motionCount = 2;
			}
		}


		if (motionCount == 2)
		{
			model_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			isplayHitAction = false;
			MotionTimer = 0.0f;
			motionCount = 0;
		}


	}

}


void Enemy::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		isplayHitAction = true;
		EnemyHP -= player_->AttackPower;
		isCoolDown = true;
		isPlayHitSound = true;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		isRelottery = true;
	}
}

float Enemy::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}

// 最短角度補間
float Enemy::LerpShortAngle(float a, float b, float t)
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

float Enemy::LerpShortTranslate(float a, float b, float t) {
	return a + t * (b - a);
}

void Enemy::LotteryHitSound() {
	if (isPlayHitSound == true && isPlayNum == false) {
		for (int i = 0; i < 1; i++) {
			hitSoundNumber = rand() % 3;
		}
		Audio::GetInstance()->SoundPlayWave(hitSound[hitSoundNumber], false);
		Audio::GetInstance()->SoundVolume(hitSound[hitSoundNumber], 0.3f);
		isPlayNum = true;
	}

	if (isPlayNum == true) {
		isPlayHitSound = false;
		isPlayNum = false;
	}

}