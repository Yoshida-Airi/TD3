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

	if (--deathTimer <= 0) {
		isDead_ = true;
	}

}

void Enemy::Draw(Camera* camera) {

	model_->Draw(camera);

}

void Enemy::Finalize() {
	
}

void Enemy::SetTranslate(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> translateX (-3.0f, 3.0f);
	std::uniform_real_distribution<float> translateZ (-3.0f, 3.0f);

	model_->worldTransform_->translation_ = { translateX(randomEngine),0.0f,translateZ(randomEngine) };

}
