#include "EnemyBullet.h"
#include"CollisionConfig.h"

EnemyBullet::~EnemyBullet() {

}

void EnemyBullet::Initialize() {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	model_->worldTransform_->scale_ = { 0.2f,0.2f,0.2f };

	SetRadius(model_->worldTransform_->scale_);
}

void EnemyBullet::Update() {
	model_->Update();

	Collider::UpdateWorldTransform();

	model_->worldTransform_->translation_.x += speed_.x;
	model_->worldTransform_->translation_.y += speed_.y;
	model_->worldTransform_->translation_.z += speed_.z;

	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(Camera* camera) {
	model_->Draw(camera);
}

void EnemyBullet::SetTranslate(Vector3 translate) {
	model_->worldTransform_->translation_ = { translate.x,translate.y,translate.z };
}

void EnemyBullet::SetSpeed(Vector3 speed) {
	speed_ = speed;
}

Vector3 EnemyBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

Vector3 EnemyBullet::GetRotate()
{
	return model_->worldTransform_->rotation_;
}

void EnemyBullet::SetPosition(Vector3 translation)
{
	model_->worldTransform_->translation_ = translation;
}


void EnemyBullet::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{

	}


}