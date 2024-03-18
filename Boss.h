#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Playerlevel/Playerlevel.h"
#include <random>
#include "Collider.h"
#include "Player.h"
#include "imgui.h"
class Boss : public Collider {
public:
	~Boss();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void Finalize();

public:

	void SetTranslate(std::mt19937& randomEngine, Vector3 translate);

	Vector3 GetTranslate() { return model_->worldTransform_->translation_; }

	Matrix4x4 GetMatWorld() { return model_->worldTransform_->matWorld_; }

	bool GetIsDead() { return isDead_; }

	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	int GetHP() { return hp; }

	bool GetIsCoolDown() { return isCoolDown; }

private:
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;

	Input* input_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

	int hp = 0;

	bool isCoolDown = false;
	int coolDownTimer = 0;

private:

	void CoolDown();

};

