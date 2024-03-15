#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Input.h"
#include"Audio.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Camera.h"
#include"Player.h"
#pragma once
class Playerlevel
{
public:
	~Playerlevel();
	void Initialize();
	void Update();
	void Draw();
	void Levelup();
	void Skillup();
	float Experiencepoint = 0.0f;//経験値
	int nowlevel = 1;
	int nowskilllevel = 0;
private:

	TextureManager* textureManager_ = nullptr;
	Input* input;
	Camera* camera;
	std::unique_ptr<Player> player = nullptr;
	uint32_t levelbar;
	uint32_t experiencepointbar;
	uint32_t levelpointbar;
	uint32_t skillbar;
	std::unique_ptr<Sprite> sprite0 = nullptr;
	std::unique_ptr<Sprite> sprite1 = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Sprite> sprite3 = nullptr;
	float maxbar = 106.0f;//経験値バーの最大値
	int maxlevel = 17;
	int maxskilllevel = 3;
#ifdef _DEBUG
	ImGuiManager* imgui;
#endif // _DEBUG
};

