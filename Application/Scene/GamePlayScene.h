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

#include"Player.h"

#include"Input.h"


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
	TextureManager* textureManager_ = nullptr;
	SceneManager* sceneManager_ = nullptr;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;
	uint32_t circle;

	Camera* camera;
	

	Input* input;


	std::unique_ptr<Triangle> triangle = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sphere> sphere = nullptr;
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<ParticleSystem> particle = nullptr;



	std::unique_ptr<Player> player = nullptr;

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG


};

