#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"

class Enemy{
public:
	~Enemy();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void Finalize();

private:
	std::unique_ptr<Model> model_ = nullptr;
	Input* input_ = nullptr;

};

