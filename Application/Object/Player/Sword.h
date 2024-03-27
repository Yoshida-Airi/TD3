#pragma once
#include"Input.h"
#include"MathUtilty.h"

#include"Model.h"
#include"Collider.h"

class PlayerWeapon;

class Sword :public Collider
{
public:
	void Initialize();
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
	bool isSkill = false; //skill中がどうか　true : skill発動中

private:
	Input* input_ = nullptr;
	PlayerWeapon* Weapon_ = nullptr;

	float Speed = 0.03f;	//速度
private:

	//攻撃
	void Attack();
	void Skill();

};

