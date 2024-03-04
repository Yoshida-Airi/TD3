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

private:
	std::unique_ptr<Model> model_ = nullptr;

	int deathTimer = 0;
	bool isDead_ = false;

};

