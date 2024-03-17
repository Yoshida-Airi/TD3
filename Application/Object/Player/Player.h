#pragma once
#include"Input.h"
#include"MathUtilty.h"
#include"Model.h"
#include"Sprite.h"
#include"Collider.h"

class PlayerWeapon;

class Player :public Collider
{
public:
	void Initialize(Camera* camera);
	void Update();
	void Draw();
	void PLevelUp();

	void SetWeapon(PlayerWeapon* playerWeapon)
	{
		Weapon_ = playerWeapon;
	}

	
	Vector3 GetPosition()const { return model_->worldTransform_->translation_; };
	bool GetIsUnderAttack() { return isUnderAttack; };
	bool GetIsSkill() { return isSkill; };
	Vector3 GetWorldPosition()override;
	WorldTransform* GetWorldTransform() { return model_->worldTransform_; };

	void OnCollision([[maybe_unused]] Collider* other)override;
	std::unique_ptr<Model> model_ = nullptr;
	int AttackPower = 5;

private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

	PlayerWeapon* Weapon_ = nullptr;

	float Speed = 0.03f;	//速度
	bool isUnderAttack = false;	//攻撃中かどうか　true : 攻撃中
	bool isSkill = false; //skill中がどうか　true : skill発動中

	// レベルアップ時のHP増加量
	const int HPIncreasePerLevel = 2000;
	// レベルアップ時の攻撃力増加量
	const int AttackPowerIncreasePerLevel = 5;

	int HP = 5000;
	float angle_ = 0.0f;

private:

	//移動
	void Move();
	//攻撃
	void Attack();
	//SKILL
	void Skill();

	void Direction();

	float Lerp(const float& a, const float& b, float t);

	float LerpShortAngle(float a, float b, float t);
	
	void Rotate();

};
