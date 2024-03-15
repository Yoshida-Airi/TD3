#pragma once
#include"Input.h"
#include"MathUtilty.h"

#include"Model.h"
#include"Collider.h"

class PlayerWeapon;

class Player :public Collider
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);
	void PLevel();

	void SetWeapon(PlayerWeapon* playerWeapon)
	{
		Weapon_ = playerWeapon;
	}

	Vector3 GetPosition()const { return model_->worldTransform_->translation_; };
	bool GetIsUnderAttack() { return isUnderAttack; };
	bool GetIsSkill() { return isSkill; };
	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;
	std::unique_ptr<Model> model_ = nullptr;
	

private:
	Input* input_ = nullptr;
	PlayerWeapon* Weapon_ = nullptr;

	float Speed = 0.03f;	//速度
	bool isUnderAttack = false;	//攻撃中かどうか　true : 攻撃中
	bool isSkill = false; //skill中がどうか　true : skill発動中

	// レベルアップ時のHP増加量
	const int HPIncreasePerLevel = 20;
	// レベルアップ時の攻撃力増加量
	const int AttackPowerIncreasePerLevel = 5;

	int HP = 50;
	int AttackPower = 5;

private:

	//移動
	void Move();
	//攻撃
	void Attack();
	//SKILL
	void Skill();
	
};
