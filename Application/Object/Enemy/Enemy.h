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

	void Direction();
		
public:

	void SetTranslate(std::mt19937& randomEngine, Vector3 translate);

	Vector3 GetTranslate() { return model_->worldTransform_->translation_; }

	Matrix4x4 GetMatWorld() { return model_->worldTransform_->matWorld_; }

	bool GetIsDead() { return isDead_(); }

	bool GetIsCoolDown() { return isCoolDown; }

	void CoolDown();
	
	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	bool GetIsRelottery() { return isRelottery; }

private:
	std::unique_ptr<Model> model_ = nullptr;
	Player* player_ = nullptr;
	Input* input_ = nullptr;

	uint32_t texture;
	Vector3 speed_;

	float angle_ = 0.0f;

	bool isRelottery;

	int deathTimer = 120;
	int EnemyHP = 20;
	bool isDead_() {
		return EnemyHP < 1;
	}
	bool isCoolDown = false;
	int coolDownTimer = 0;

	float MotionTimer = 0.0f;
	int motionCount = 0;
	bool isplayHitAction = false;	//あたったときの点滅描画をするかどうか

	float Lerp(const float& a, const float& b, float t);

	// 最短角度補間
	float LerpShortAngle(float a, float b, float t);

	float LerpShortTranslate(float a, float b, float t);

	void HitAction();

};

