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

	void StartFadeOut();

	void UpdateFadeOut();

	void StartFadeIn();

	void UpdateFadeIn();

private:
	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	uint32_t ClearSceneTex;
	std::unique_ptr<Sprite>clearSprite = nullptr;

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	uint32_t UI_MouseTex;
	uint32_t UI_GamePadABottonTex;
	std::unique_ptr<Sprite>UI_Mouse = nullptr;
	std::unique_ptr<Sprite>UI_GamePadABotton = nullptr;

	float fadeOutAlpha = 0;
	float fadeInAlpha = 0;
	bool isFadeOut = false;
	bool isFadeIn = false;
};

