#include "Enemy.h"
#include"CollisionConfig.h"

Enemy::~Enemy() {

}

void Enemy::Initialize(Player* player) {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kEnemy));

	model_.reset(Model::Create("Resources/Enemy/enemy.obj"));

	input_ = Input::GetInstance();
	input_->Initialize();

	model_->worldTransform_->scale_ = { 0.3f,0.3f,0.3f };

	SetRadius(model_->worldTransform_->scale_);
	player_ = player;
}

void Enemy::Update() {
	input_->Update();

	model_->Update();

	Collider::UpdateWorldTransform();

	model_->worldTransform_->translation_.x += 0.001f;

	CoolDown();
	/*if (--deathTimer <= 0) {
		isDead_ = true;
	}*/

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
	std::uniform_real_distribution<float> translateX (-3.0f, 3.0f);
	std::uniform_real_distribution<float> translateZ (-3.0f, 3.0f);

	model_->worldTransform_->translation_ = { translate.x + translateX(randomEngine),0.0f, translate.z + translateZ(randomEngine) };
}

void Enemy::CoolDown()
{
	if (isCoolDown == true) {
		coolDownTimer++;
	}

	if (coolDownTimer == 25) {
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


void Enemy::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		EnemyHP -= player_->AttackPower;
		isCoolDown = true;
	}

	
}