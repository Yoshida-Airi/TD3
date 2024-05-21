#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include <random>
#include "Collider.h"
#include"ImGuiManager.h"

class Enemy : public Collider {
public:
	~Enemy();

	void Initialize(Player* player);

	void Update();

	void Draw(Camera* camera);

	void Finalize();

public:

	void SetTranslate(std::mt19937& randomEngine, Vector3 translate);

	Vector3 GetTranslate() { return model_->worldTransform_->translation_; }

	Matrix4x4 GetMatWorld() { return model_->worldTransform_->matWorld_; }

	bool GetIsDead() { return isDead_(); }

	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	bool GetIsRelottery() { return isRelottery; }

private:
	std::unique_ptr<Model> model_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;

	uint32_t texture;

	bool isRelottery;

	int deathTimer = 120;
	int EnemyHP = 2000;
	bool isDead_() {
		return EnemyHP < 0;
	}

};

