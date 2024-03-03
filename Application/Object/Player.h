#pragma once
#include"Input.h"

#include"Model.h"

class playerWeapon;

class Player
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void SetWeapon(playerWeapon* playerWeapon)
	{
		Weapon_ = playerWeapon;
	}

	Vector3 GetPosition()const { return Model_->worldTransform_->translation_; };

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> Model_ = nullptr;

	float Speed = 0;	//速度

	playerWeapon* Weapon_ = nullptr;

	bool isUnderAttack = false;	//攻撃中かどうか　true : 攻撃中

private:

	//移動
	void Move();

	//攻撃
	void Attack();

};
