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

#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include <random>
#include "../Boss.h"
#include "../BossBullet.h"
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



private:

	void EnemySpawn();
	void EnemyAttack();

	/// <summary>
	/// 敵が消滅するときのエフェクト生成
	/// </summary>
	void CreateDeathEffect(Vector3 position);

	void StartFadeOut();

	void UpdateFadeOut();

	void StartFadeIn();

	void UpdateFadeIn(const std::string& sceneName);

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
	std::unique_ptr<BossBullet> bossBullet_ = nullptr;

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



	float cameraZ;//lerpしたときの誤差修正数値

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	float fadeAlpha = 0;
	float fadeInAlpha = 0;
	bool isFadeOut = false;

	bool isFadeIn = false;

	bool sceneClear;
	bool sceneOver;

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

private:

	void CheckAllCollisions();
	void BossSceneAllCollisions();
	void Hitstop();

	//被弾時硬直の時間
	int hitstopTimer = 0;
	//被弾後の無敵時間
	int throughTimer = 0;


	bool isCameraShake = false;
	uint32_t cameraShakeTime;
	//雑魚戦の総秒数
	uint32_t kFullWaveTime = 120;
};
