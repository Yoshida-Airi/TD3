#pragma once
#include"ParticleSystem.h"
class SlashingEffect
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
		hitEffect->emitter_->transform.translate.x = position.x;
		hitEffect->emitter_->transform.translate.y = position.y + 0.7f;
		hitEffect->emitter_->transform.translate.z = position.z;
	}

	/// <summary>
	/// 実行が終わったかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsDead();

	//粒の生成を始める
	void StartParticle();

	//粒の生成を終える
	void StopMakeParticle();

	void SetVelocity(Vector3 velocity);

	void SetVelocityX(bool isMove)
	{
		hitEffect->SetRandomVelocityX(isMove);
	}	
	void SetVelocityZ(bool isMove)
	{
		hitEffect->SetRandomVelocityZ(isMove);
	}

private:

	TextureManager* textureManager_ = nullptr;

	std::unique_ptr<ParticleSystem> hitEffect = nullptr;
	uint32_t circleTexture;

	Vector3 velocity = { 0,0,0 };
	bool flag;	//実行中かどうかのフラグ
	int playTime = 20;
	bool isDead = false;


};

