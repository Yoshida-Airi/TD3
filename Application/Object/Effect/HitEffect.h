#pragma once
#include"ParticleSystem.h"

class HitEffect
{
public:
	void Initialize(Camera* camera);
	void Update();
	void Draw();
	void SetFlag(bool isFlag)
	{
		flag = isFlag;
	}

private:

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<ParticleSystem> hitEffect = nullptr;
	uint32_t circleTexture;

	bool flag;	//実行中かどうかのフラグ


};

