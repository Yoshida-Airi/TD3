#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Playerlevel/Playerlevel.h"
#include <random>
#include "Collider.h"
#include "Player.h"
#include "imgui.h"
#include "../BossBullet.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class SceneManager;

enum BossAction {
	MOVE,
	SHOT,
	TACKLE,
};

class Boss : public Collider {
public:
	~Boss();

	void Initialize(Player* player, BossBullet* bullet);

	void Update(SceneManager* scene);

	void Draw(Camera* camera);

	void Finalize();

public:

	void SetTranslate(Vector3 translate);

	Vector3 GetTranslate() { return model_->worldTransform_->translation_; }

	Matrix4x4 GetMatWorld() { return model_->worldTransform_->matWorld_; }

	bool GetIsDead() { return isDead_; }

	Vector3 GetWorldPosition()override;
	Vector3 GetRotate()override;
	void SetPosition(Vector3 translation) override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	int GetHP() { return hp; }

	bool GetIsCoolDown() { return isCoolDown; }

private:
	std::unique_ptr<Model> model_ = nullptr;
	Player* player_ = nullptr;
	BossBullet* bullet_ = nullptr;

	Input* input_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

	int hp = 0;

	bool isCoolDown = false;
	int coolDownTimer = 0;

	Vector3 speed_;

	float angle_ = 0.0f;

	BossAction bAction = MOVE;
	unsigned int currentTime;
	int action = 0;

	int nextActionTimer = 0;
	bool isNextAction = true;

	bool isAttack = false;
	bool isAssignment = false;
	bool isNext = false;
	bool isBAlive = false;
	int aimTimer = 0;
	int BTimer = 0;

	uint32_t bossTex;

private:

	void CoolDown();

	void Move();

	void NextAction();

	void Direction(float speed);
	
	void Shot();

	void Tackle();

	void Dead(SceneManager* scene);

	//ここから下は回転用
	float Lerp(const float& a, const float& b, float t);

	// 最短角度補間
	float LerpShortAngle(float a, float b, float t);
	
	float LerpShortTranslate(float a, float b, float t);

};

