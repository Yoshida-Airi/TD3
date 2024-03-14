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

	void SetWeapon(PlayerWeapon* playerWeapon)
	{
		Weapon_ = playerWeapon;
	}

	uint32_t direction = TextureManager::GetInstance()->LoadTexture("Resources/DefaultAssets/reticle.png");

	Vector3 GetPosition()const { return model_->worldTransform_->translation_; };
	bool GetIsUnderAttack() { return isUnderAttack; };
	bool GetIsSkill() { return isSkill; };
	Vector3 GetWorldPosition()override;

	void OnCollision([[maybe_unused]] Collider* other)override;
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> directionModel_ = nullptr;

	std::unique_ptr<Sprite> sprite2DReticle_ = nullptr;

private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

	PlayerWeapon* Weapon_ = nullptr;

	float Speed = 0.03f;	//速度
	bool isUnderAttack = false;	//攻撃中かどうか　true : 攻撃中
	bool isSkill = false; //skill中がどうか　true : skill発動中

	int HP = 50;

private:

	//移動
	void Move();
	//攻撃
	void Attack();
	//SKILL
	void Skill();

	void Direction();
	void Set3DReticleMousePosition(const Camera* camera);
};

