#pragma once
#include "Model.h"
#include "Collider.h"

class EnemyBullet : public Collider
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

	Vector3 GetWorldPosition()override;
	Vector3 GetRotate()override;
	void SetPosition(Vector3 translation) override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void SetScale(Vector3 scale);

private:
	std::unique_ptr<Model> model_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

	Vector3 speed_;

};

