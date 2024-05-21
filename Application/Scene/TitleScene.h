#pragma once

#include"BaseScene.h"
#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Input.h"
#include"Audio.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Camera.h"

#include"DeathEffect.h"
#include"SlashingEffect.h"

#include"Enemy.h"
#include <random>


class TitleScene :public BaseScene
{
public:
	~TitleScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void StartFadeIn();

	void UpdateFadeIn();

private:

	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;

	uint32_t soundData;

	std::unique_ptr<DeathEffect> effect = nullptr;
	std::unique_ptr<SlashingEffect> slashingEffect = nullptr;

	uint32_t fadeTex;
	uint32_t titleTex;
	uint32_t UI_MouseTex;
	uint32_t UI_GamePadABottonTex;
	

	std::unique_ptr<Sprite>fadeSprite = nullptr;
	std::unique_ptr<Sprite>titleSprite = nullptr;
	std::unique_ptr<Sprite>UI_Mouse = nullptr;
	std::unique_ptr<Sprite>UI_GamePadABotton = nullptr;

	float alpha = 0;
	bool isFadingIn = false;


};

