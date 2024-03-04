#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include <random>

class Enemy{
public:
	~Enemy();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void Finalize();

public:

	void SetTranslate(std::mt19937& randomEngine);

	bool GetIsDead() { return isDead_; }

private:
	std::unique_ptr<Model> model_ = nullptr;
	Input* input_ = nullptr;

	int deathTimer = 120;
	bool isDead_ = false;

};

