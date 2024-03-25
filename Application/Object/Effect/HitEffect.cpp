#include "HitEffect.h"

void HitEffect::Initialize(Camera* camera)
{
	textureManager_ = TextureManager::GetInstance();
	circleTexture = textureManager_->LoadTexture("Resources/DefaultAssets/circle.png");

	Vector3 velocity = { 0.0f,0.0f,0.0f };
	hitEffect.reset(ParticleSystem::Create(circleTexture, camera, velocity, false));
	hitEffect->emitter_->count = 20;
	hitEffect->emitter_->frequency = 0.5f;
	hitEffect->SetLifeTime(1.0f, 3.0f);
	hitEffect->SetUseBillBoard();
	hitEffect->SetRandomAllVelocity();
}

void HitEffect::Update()
{
	hitEffect->Debug("hitEffect");
	hitEffect->Update();

	if (flag)
	{
		playTime--;
		

		if (playTime <= 0)
		{
			//実行時間を過ぎたら粒の生成を止める
			hitEffect->StopMakeParticle();

			if (hitEffect->GetIsParticleEmpty())
			{
				//エフェクトの粒が0になったら消滅フラグを立てる
				isDead = true;
			}
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

bool HitEffect::IsDead()
{
	return isDead;
}

