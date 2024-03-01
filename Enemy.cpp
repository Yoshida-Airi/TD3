#include "Enemy.h"

Enemy::~Enemy() {

}

void Enemy::Initialize() {
	model_.reset(Model::Create("Resources/DefaultAssets/axis.obj"));

	input_ = Input::GetInstance();
	input_->Initialize();

}

void Enemy::Update() {
	input_->Update();

	model_->Update();

	model_->worldTransform_->translation_.x += 0.001f;

}

void Enemy::Draw(Camera* camera) {

	model_->Draw(camera);

}

void Enemy::Finalize() {
	
}