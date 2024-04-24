#pragma once
#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Camera.h"

#include"Input.h"

#include"DeathEffect.h"
#include"Sprite.h"

class GameClearScene : public BaseScene
{
public:
	~GameClearScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	uint32_t ClearSceneTex;
	std::unique_ptr<Sprite>clearSprite = nullptr;


};

