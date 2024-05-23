#include "Sword.h"
#include"CollisionConfig.h"


void Sword::Initialize(Player* player)
{
	input_ = Input::GetInstance();

	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));

	model_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));

	model_->worldTransform_->translation_ = {0.0f,1.66f,0.87f };

	SetRadius({ 2.0f,1.0f,0.125f });
	player_ = player;

	model_->worldTransform_->scale_ = { 0.5f,0.5f,0.5f };
	model_->worldTransform_->parent_ = player_->GetWorldTransform();
}

void Sword::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	model_->ModelDebug("Sword");




	Attack();
}

void Sword::Draw(Camera* camera)
{
	model_->Draw(camera);
}

void Sword::SetPosition(Vector3 translation)
{
	model_->worldTransform_->translation_ = translation;
};
Vector3 Sword::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

Vector3 Sword::GetRotate()
{
	return model_->worldTransform_->rotation_;
}


void Sword::OnCollision([[maybe_unused]] Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy)|| typeID == static_cast<uint32_t>(CollisionTypeDef::kBoss))
	{
		//敵にあたったら
		isHit = true;
	}
}

void Sword::SetIsHit(bool hit)
{
	isHit = hit;
}

void Sword::Attack()
{

}
