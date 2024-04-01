#pragma once
#include"Input.h"
#include"MathUtilty.h"
#include"Model.h"
#include "Player.h"
#include"Collider.h"

class PlayerWeapon;

class Sword :public Collider
{
public:
	void Initialize(Player* player);
	void Update();
	void Draw(Camera* camera);

	void SetWeapon(PlayerWeapon* playerWeapon)
	{
		Weapon_ = playerWeapon;
	}

	Vector3 GetPosition()const { return model_->worldTransform_->translation_; };
	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;
	std::unique_ptr<Model> model_ = nullptr;
	
	WorldTransform* GetWorldTransform() { return model_->worldTransform_; };

	float rotationmax = 1.57f;
	float rotationmin = 0.0f;
	float rotationspeed = 0.2f;
	bool isAttack = false;
	bool combo1 = false;
	bool combo2 = false;
	int count = 0;

private:
	Input* input_ = nullptr;
	PlayerWeapon* Weapon_ = nullptr;
	Player* player_ = nullptr;

	float Speed = 0.03f;	//速度

	bool keyBoard = true;
	bool gamePad = false;

private:

	//攻撃
	void Attack();

};

