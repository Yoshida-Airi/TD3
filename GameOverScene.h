#pragma once
#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Camera.h"

#include"Input.h"

#include"DeathEffect.h"

class GameOverScene : public BaseScene
{
public:
	~GameOverScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	std::unique_ptr<DeathEffect> effect = nullptr;

};

