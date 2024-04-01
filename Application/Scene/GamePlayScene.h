#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ParticleSystem.h"
#include"Camera.h"
#include"Timer.h"

#include"Player.h"
#include"Sword.h"
#include"CollisionManager.h"

#include"Input.h"

#include "Playerlevel/Playerlevel.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include <random>
#include "../Boss.h"
#include"DeathEffect.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene:public BaseScene
{
public:
	~GamePlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	enum class Skill
	{
		kRoot,	//待機
		kSkill1,	//ダッシュ
		kSkill2,	//ダッシュ＋攻撃
		kSkill3	//範囲攻撃
	};

private:

	void EnemySpawn();
	void EnemyAttack();

	/// <summary>
	/// 敵が消滅するときのエフェクト生成
	/// </summary>
	void CreateDeathEffect(Vector3 position);

private:
	TextureManager* textureManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	
	//当たり判定処理
	std::unique_ptr<CollisionManager> colliderManager_ = nullptr;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;
	uint32_t circle;

	Camera* camera;
	
	Timer timer;

	Input* input;


	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sphere> sphere = nullptr;
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<ParticleSystem> particle = nullptr;
	std::unique_ptr<Boss> boss_ = nullptr;


	std::unique_ptr<Model> demo_stage = nullptr;

	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Sword> sword = nullptr;


	std::list<Enemy*> enemy_;
	std::list<EnemyBullet*> enemyBullet_;
	std::list<DeathEffect*>deathEffect_;	//敵消滅エフェクト

	int enemyCount = 1;
	const int MaxEnemySpawn = 5;
	bool isEnemySpawn = true;
	bool isEnemyReSpawn = false;

	bool isEnemyAttack = true;
	int enemyAttackCoolDown = 0;



	bool isBossSpawn = false;

	int enemyDeathCount = 0;

	std::random_device generator;

	Vector3 offset;
	Vector3 targetPosition;

	Skill behavior_ = Skill::kRoot;
	std::optional<Skill>behaviorRequest_ = std::nullopt;

	int MotionTimer_ = 0;
	int MotionCount_ = 0;

	bool isSkill;
	bool isSkillCooldown_; // スキルのクールダウン中かどうかを示すフラグ
	int skillCooldownTime_; // スキルのクールダウン時間
	unsigned int skillCooldownDuration_; // スキルのクールダウン期間

	float cameraZ;//lerpしたときの誤差修正数値


#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

private:

	void CheckAllCollisions();
	void BossSceneAllCollisions();

	void skillRootUpdate();
	void skill1Update();
	void skill2Update();
	void skill3Update();
	
	void skillRootInitialize();	//待機
	void skill1Initialize();	//スキル１
	void skill2Initialize();	//スキル２
	void skill3Initialzie();	//スキル３
	

};
