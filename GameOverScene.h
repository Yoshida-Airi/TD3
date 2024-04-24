#pragma once
#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"
#include"Camera.h"
#include"Input.h"

#include"DeathEffect.h"
#include"Sprite.h"

class GameOverScene : public BaseScene
{
public:
	~GameOverScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void StartFadeOut();

	void UpdateFadeOut();

private:
	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	std::unique_ptr<DeathEffect> effect = nullptr;

	uint32_t overSceneTex;
	std::unique_ptr<Sprite>overSprite = nullptr;

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	float fadeOutAlpha = 0;
	bool isFadeOut = false;

};

