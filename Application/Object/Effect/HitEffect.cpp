#include "HitEffect.h"

void HitEffect::Initialize(Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture = textureManager_->LoadTexture("Resources/DefaultAssets/circle.png");

	Vector3 velocity = { 0.0f,5.0f,0.0f };
	hitEffect.reset(ParticleSystem::Create(circleTexture, camera, velocity, true));
	hitEffect->emitter_->count = 100;
	hitEffect->emitter_->transform.scale = { 0.5f,0.0f,0.0f };
	hitEffect->SetLifeTime(1.0f, 3.0f);
	hitEffect->SetUseBillBoard();
	hitEffect->SetRandomVelocityY();
}

void HitEffect::Update()
{
	hitEffect->Debug("hitEffect");
	
	if (flag)
	{
		playTime--;
		hitEffect->Update();

		if (playTime <= 0)
		{
			//実行時間を過ぎたら粒の生成を止める
			hitEffect->StopMakeParticle();
		}
	}
}

void HitEffect::Draw()
{
	if (flag)
	{
		hitEffect->Draw();
	}
}

