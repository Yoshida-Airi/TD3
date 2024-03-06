#pragma once
#include "Model.h"

class EnemyBullet
{
public:
	~EnemyBullet();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

public:

	void SetTranslate(Vector3 translate);

	bool GetIsDead() { return isDead_; }

	void SetSpeed(Vector3 speed);

private:
	std::unique_ptr<Model> model_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

	Vector3 speed_;

};

