#include "Boss.h"
#include"CollisionConfig.h"

Boss::~Boss() {

}

void Boss::Initialize(Player* player) {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kBoss));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	input_ = Input::GetInstance();
	input_->Initialize();

	SetRadius(model_->worldTransform_->scale_);

	hp = 100;

	player_ = player;

}

void Boss::Update() {
	input_->Update();

	model_->Update();

	Collider::UpdateWorldTransform();

	//model_->worldTransform_->translation_.x += 0.001f;

	if (hp <= 0)
	{
		isDead_ = true;
	}

	Move();
	CoolDown();

	/*if (--deathTimer <= 0) {
		isDead_ = true;
	}*/
	ImGui::Begin("Boss");
	ImGui::Text("HP : %d", hp);
	ImGui::End();
}

void Boss::Draw(Camera* camera) {

	model_->Draw(camera);

}

void Boss::Finalize() {

}

void Boss::SetTranslate(std::mt19937& randomEngine, Vector3 translate) {
	std::uniform_real_distribution<float> translateX(-3.0f, 3.0f);
	std::uniform_real_distribution<float> translateZ(-3.0f, 3.0f);

	model_->worldTransform_->translation_ = { translate.x + translateX(randomEngine),0.0f, translate.z + translateZ(randomEngine) };
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

Vector3 Boss::GetRotate()
{
	return model_->worldTransform_->rotation_;
}

void Boss::SetPosition(Vector3 translation)
{
	model_->worldTransform_->translation_ = translation;
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

	const float kBulletSpeed = 0.04f;
	Vector3 playerPos = player_->GetPosition();
	Vector3 enemyPos = model_->worldTransform_->translation_;
	Vector3 speed;

	speed.x = playerPos.x - enemyPos.x;
	speed.y = playerPos.y - enemyPos.y;
	speed.z = playerPos.z - enemyPos.z;

	speed = Normalize(speed);

	speed.x *= kBulletSpeed;
	speed.y *= kBulletSpeed;
	speed.z *= kBulletSpeed;

	speed = TransformNormal(speed, model_->worldTransform_->matWorld_);

	model_->worldTransform_->translation_.x += speed.x;
	model_->worldTransform_->translation_.y += speed.y;
	model_->worldTransform_->translation_.z += speed.z;

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