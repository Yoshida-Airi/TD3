#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet() {

}

void EnemyBullet::Initialize() {

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	model_->worldTransform_->scale_ = { 0.2f,0.2f,0.2f };
}

void EnemyBullet::Update() {
	model_->Update();

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