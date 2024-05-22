#include"ImGuiManager.h"
#include"SceneManager.h"
#include"Input.h"
#include"Audio.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"Camera.h"
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
	float DebugExperienvepoint = 0.5f; //デバック用経験値
	int nowlevel = 1;
	int count = 2;
	int nowskilllevel = 0;
	std::unique_ptr<Sprite> sprite0 = nullptr;
	std::unique_ptr<Sprite> sprite1 = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Sprite> sprite3 = nullptr;
	float maxbar = 106.0f;//経験値バーの最大値
	int maxlevel = 17;
	int maxskilllevel = 3;
private:

	TextureManager* textureManager_ = nullptr;
	Input* input;
	Camera* camera;
	uint32_t levelbar;
	uint32_t experiencepointbar;
	uint32_t levelpointbar;
	uint32_t skillbar;

	
#ifdef _DEBUG
	ImGuiManager* imgui;
#endif // _DEBUG
};

