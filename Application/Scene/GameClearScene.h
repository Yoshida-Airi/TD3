#pragma once
#include"BaseScene.h"

#include"WinApp.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"
#include"Audio.h"
#include"Camera.h"
#include"Timer.h"

#include"Input.h"

#include"DeathEffect.h"
#include"Sprite.h"
#include"Model.h"

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

	void SetPlayerModel();

	void GameClearAction();

private:
	WinApp* winApp_ = nullptr;
	Input* input;
	SceneManager* sceneManager_ = nullptr;
	Camera* camera;
	Timer timer;

	uint32_t ClearSceneTex;
	std::unique_ptr<Sprite>clearSprite = nullptr;

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	uint32_t UI_MouseTex;
	uint32_t UI_GamePadABottonTex;
	std::unique_ptr<Sprite>UI_Mouse = nullptr;
	std::unique_ptr<Sprite>UI_GamePadABotton = nullptr;

	uint32_t ClearBGM;

	float fadeOutAlpha = 0;
	float fadeInAlpha = 0;
	bool isFadeOut = false;
	bool isFadeIn = false;

	uint32_t numberTex[3];
	std::string texNum = "Resources/Numbers/number.png";
	std::unique_ptr<Sprite>numberSprite[3];
	uint32_t scoreTex;
	std::string texScore = "Resources/Numbers/seconds_white.png";
	std::unique_ptr<Sprite>scoreSprite;

	std::unique_ptr<Model> demo_stage = nullptr;
	std::unique_ptr<Model> bodyModel_ = nullptr;
	std::unique_ptr<Model> headModel_ = nullptr;
	std::unique_ptr<Model> LeftArmModel_ = nullptr;
	std::unique_ptr<Model> RightArmModel_ = nullptr;
	std::unique_ptr<Model> LeftFootModel_ = nullptr;
	std::unique_ptr<Model> RightFootModel_ = nullptr;
	std::unique_ptr<Model> swoad_ = nullptr;

	std::unique_ptr<Model> ui_gameOver_ = nullptr;

	uint32_t playerTex;
	uint32_t headTex;
	uint32_t bodyTex;
	uint32_t leftFootTex;
	uint32_t rightFootTex;

	float MotionTimer = 0.0f;
	int motionCount = 0;
	bool isplayHitAction = false;	//あたったときの点滅描画をするかどうか

	float lightDirectionX = 0.1f;
};

