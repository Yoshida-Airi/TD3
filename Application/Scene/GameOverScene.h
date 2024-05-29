#pragma once
#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"
#include"Camera.h"
#include"Input.h"
#include"Audio.h"
#include"DeathEffect.h"
#include"Sprite.h"
#include"Model.h"

class GameOverScene : public BaseScene
{
public:
	~GameOverScene()override;
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

	std::unique_ptr<DeathEffect> effect = nullptr;

	uint32_t overSceneTex;
	std::unique_ptr<Sprite>overSprite = nullptr;

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	uint32_t UI_MouseTex;
	uint32_t UI_GamePadABottonTex;
	std::unique_ptr<Sprite>UI_Mouse = nullptr;
	std::unique_ptr<Sprite>UI_GamePadABotton = nullptr;

	uint32_t enemyTexture[4];

	std::unique_ptr<Model> demo_stage = nullptr;
	std::unique_ptr<Model> bodyModel_ = nullptr;
	std::unique_ptr<Model> headModel_ = nullptr;
	std::unique_ptr<Model> LeftArmModel_ = nullptr;
	std::unique_ptr<Model> RightArmModel_ = nullptr;
	std::unique_ptr<Model> LeftFootModel_ = nullptr;
	std::unique_ptr<Model> RightFootModel_ = nullptr;
	std::unique_ptr<Model> swoad_ = nullptr;
	std::unique_ptr<Model> enemy_ = nullptr;

	std::unique_ptr<Model> ui_gameOver_ = nullptr;

	uint32_t playerTex;
	uint32_t headTex;
	uint32_t bodyTex;
	uint32_t leftFootTex;
	uint32_t rightFootTex;

	uint32_t GameoverBGM;


	float fadeOutAlpha = 0;
	float fadeInAlpha = 0;
	bool isFadeOut = false;
	bool isFadeIn = false;

	float addScale = 0.00f;


private:

	void SetPlayerModel();
	void gameoverAnimation();

};

