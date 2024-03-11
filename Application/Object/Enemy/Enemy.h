#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include <random>
#include "Collider.h"

class Enemy : public Collider {
public:
	~Enemy();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void Finalize();

public:

	void SetTranslate(std::mt19937& randomEngine);

	Vector3 GetTranslate() { return model_->worldTransform_->translation_; }

	Matrix4x4 GetMatWorld() { return model_->worldTransform_->matWorld_; }

	bool GetIsDead() { return isDead_; }

	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	std::unique_ptr<Model> model_ = nullptr;
	Input* input_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

};

