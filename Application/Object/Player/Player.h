#pragma once
#include"Input.h"
#include"MathUtilty.h"
#include"Model.h"
#include"Sprite.h"
#include"Collider.h"
#include "Playerlevel/Playerlevel.h"
#include"SlashingEffect.h"

class Sword;
class Player :public Collider
{
public:

	enum class Animation
	{
		kRoot,		//待機
		kAttack,	//攻撃
		kSkill1,	//ダッシュ
		kSkill2,	//ダッシュ＋攻撃
		kSkill3	//範囲攻撃
	};

	void Initialize(Camera* camera);
	void Update();
	void Draw();
	void TextureDraw();
	void PLevelUp();

	void SetWeapon(Sword* playerWeapon)
	{
		weapon_ = playerWeapon;
	}

	void SetSkill(bool isSkill)
	{
		this->isSkill = isSkill;
	}

	void SetIsHit(bool isHit)
	{
		this->isHit = isHit;
	}
	
	Vector3 GetPosition()const { return model_->worldTransform_->translation_; };
	bool GetIsUnderAttack() { return isUnderAttack; };
	bool GetIsSkillAttack() { return isSkillAttack; };
	Vector3 GetWorldPosition()override;
	WorldTransform* GetWorldTransform() { return bodyModel_->worldTransform_; };
	WorldTransform* GetLeftArmWorldTransform() { return LeftArmModel_->worldTransform_; };

	void OnCollision([[maybe_unused]] Collider* other)override;
	std::unique_ptr<Model> model_ = nullptr;
	uint32_t GetNowPower() { return AttackPower; };
	int AttackPower = 5;

	float Lerp(const float& a, const float& b, float t);
	float LerpShortAngle(float a, float b, float t);
	float LerpShortTranslate(float a, float b, float t);
	bool GetIsCoolDown() { return isCoolDown; }
	bool GetIsHit() { return isHit; }
	int GetHP() { return HP; };
	Playerlevel* GetPlayerLevel() { return playerLevel.get(); };
	
private:
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

	Sword* weapon_ = nullptr;

	float Speed = 0.03f;//速度
	bool isUnderAttack = false;	//攻撃中かどうか　true : 攻撃中
	bool isSkill = false; //skill中がどうか　true : skill発動中
	bool isSkillAttack = false;

	// レベルアップ時のHP増加量
	const int HPIncreasePerLevel = 2000;
	// レベルアップ時の攻撃力増加量
	const int AttackPowerIncreasePerLevel = 100;

	int HP = 5000;
	float angle_ = 0.0f;

	bool isHit = false;

	bool isCoolDown = false;
	int coolDownTimer = 0;

	bool keyBoard = true;
	bool gamePad = false;

	float PlayerSpeed = 0.05f;


	//Playerlevel* playerlevel;
	std::unique_ptr<Playerlevel>playerLevel = nullptr;

	std::unique_ptr<Model> bodyModel_ = nullptr;
	std::unique_ptr<Model> headModel_ = nullptr;
	std::unique_ptr<Model> LeftArmModel_ = nullptr;
	std::unique_ptr<Model> RightArmModel_ = nullptr;
	std::unique_ptr<Model> LeftFootModel_ = nullptr;
	std::unique_ptr<Model> RightFootModel_ = nullptr;

	std::unique_ptr<SlashingEffect> slashingEffect = nullptr;

	uint32_t playerTex;
	uint32_t headTex;
	uint32_t bodyTex;
	uint32_t leftFootTex;
	uint32_t rightFootTex;
	uint32_t HpTex;

	uint32_t ui_skill_padTex;
	uint32_t ui_skill_keyboardTex;
	uint32_t ui_hpTex;
	uint32_t ui_playerLevelTex;
	uint32_t ui_skillLevelTex;

	std::unique_ptr<Sprite> hpSprite_;
	std::unique_ptr<Sprite> ui_skill_pad;
	std::unique_ptr<Sprite> ui_skill_keyboard;
	std::unique_ptr<Sprite> ui_hp;
	std::unique_ptr<Sprite> ui_playerLevel;
	std::unique_ptr<Sprite> ui_skillLevel;

	Vector2 hpSpriteScale;

	Animation behavior_ = Animation::kRoot;
	std::optional<Animation>behaviorRequest_ = std::nullopt;


	int MotionTimer_ = 0;
	int MotionCount_ = 0;

	bool isSkillCooldown_; // スキルのクールダウン中かどうかを示すフラグ
	int skillCooldownTime_; // スキルのクールダウン時間
	unsigned int skillCooldownDuration_; // スキルのクールダウン期間

	float rotationmax = 1.57f;
	float rotationmin = 0.0f;
	float rotationspeed = 0.2f;


	bool isPlayWalkAnimation;

private:

	//移動
	void Move();
	//攻撃
	void Attack();
	//SKILL
	void Skill();

	//一回あたったときの無敵時間
	void CoolDown();
	//hitstop管理

	void RootUpdate();
	void AttackUpdate();
	void Skill1Update();
	void Skill2Update();
	void Skill3Update();

	void RootInitialize();	//待機
	void AttackInitialize();
	void Skill1Initialize();	//スキル１
	void Skill2Initialize();	//スキル２
	void Skill3Initialzie();	//スキル３


};
