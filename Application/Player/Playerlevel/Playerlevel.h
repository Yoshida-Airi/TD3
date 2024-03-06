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

private:

	TextureManager* textureManager_ = nullptr;
	Input* input;
	Camera* camera;
	uint32_t levelbar;
	std::unique_ptr<Sprite> sprite = nullptr;
#ifdef _DEBUG
	ImGuiManager* imgui;
#endif // _DEBUG
};

