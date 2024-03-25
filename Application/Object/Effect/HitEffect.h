#pragma once
#include"ParticleSystem.h"

class HitEffect
{
public:
	void Initialize(Camera* camera);
	void Update();
	void Draw();

	/// <summary>
	/// 実行するかどうか
	/// </summary>
	/// <param name="isFlag"></param>
	void SetFlag(bool isFlag)
	{
		flag = isFlag;
	}

	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		hitEffect->emitter_->transform.translate = position;
	}

	/// <summary>
	/// 実行が終わったかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDead()
	{
		return playTime <= 0;
	}

private:

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<ParticleSystem> hitEffect = nullptr;
	uint32_t circleTexture;

	bool flag;	//実行中かどうかのフラグ

	int playTime = 180;


};

