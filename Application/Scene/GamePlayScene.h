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
#include"playerWeapon.h"
#include"Sword.h"
#include"CollisionManager.h"

#include"Input.h"

#include "Playerlevel/Playerlevel.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include <random>
#include "../Boss.h"

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

private:
	TextureManager* textureManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
	Playerlevel* playerlevel;
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

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

private:

	void CheckAllCollisions();

	void BossSceneAllCollisions();
};
