#pragma once
#include"Input.h"

#include"Model.h"

class Player
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> playerModel_ = nullptr;

	float playerSpeed = 0;

private:

	//移動
	void Move();

};

