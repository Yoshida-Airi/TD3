#include "BossBullet.h"
#include"CollisionConfig.h"

BossBullet::~BossBullet() {

}

void BossBullet::Initialize() {
	Collider::Initialize();

	//当たり判定用
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kBossBullet));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	uint32_t redTex = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/red.png");
	model_->SetTexture(redTex);
	model_->worldTransform_->scale_ = { 0.2f,0.2f,0.2f };

	SetRadius(model_->worldTransform_->scale_);
}

void BossBullet::Update() {
	model_->Update();

	Collider::UpdateWorldTransform();

	model_->worldTransform_->translation_.x += speed_.x;
	model_->worldTransform_->translation_.y += speed_.y;
	model_->worldTransform_->translation_.z += speed_.z;

	if (--deathTimer <= 0) {
		isDead_ = true;
	}
}

void BossBullet::Draw(Camera* camera) {
	model_->Draw(camera);
}

void BossBullet::SetTranslate(Vector3 translate) {
	model_->worldTransform_->translation_ = { translate.x,translate.y,translate.z };
}

void BossBullet::SetSpeed(Vector3 speed) {
	speed_ = speed;
}

Vector3 BossBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

Vector3 BossBullet::GetRotate()
{
	return Vector3();
}

void BossBullet::SetPosition(Vector3 translation)
{
}

void BossBullet::SetScale(Vector3 scale) {
	model_->worldTransform_->scale_ = scale;
}

void BossBullet::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kPlayer))
	{

	}


}