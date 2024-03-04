#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet() {

}

void EnemyBullet::Initialize() {
	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	model_->worldTransform_->scale_ = { 0.2f };
}

void EnemyBullet::Update() {
	model_->Update();
}

void EnemyBullet::Draw(Camera* camera) {
	model_->Draw(camera);
}

void EnemyBullet::SetTranslate(Vector3 translate) {
	model_->worldTransform_->translation_ = translate;
}