#include "Sword.h"
#include"CollisionConfig.h"


void Sword::Initialize(Player* player)
{
	input_ = Input::GetInstance();

	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon));

	model_.reset(Model::Create("Resources/DefaultAssets/Sword.obj"));

	model_->worldTransform_->translation_ = { -0.37f,1.66f,0.87f };

	SetRadius({ 3.0f,1.0f,1.0f });
	player_ = player;

	model_->worldTransform_->parent_ = player_->GetWorldTransform();
}

void Sword::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	model_->ModelDebug("Sword");

	//コントローラーチェンジ
	if (input_->PushKey(DIK_1)) {
		gamePad = true;
		keyBoard = false;
	}
	if (input_->PushKey(DIK_2)) {
		gamePad = false;
		keyBoard = true;
	}


	Attack();
}

void Sword::Draw(Camera* camera)
{
	model_->Draw(camera);
}

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

void Sword::OnCollision([[maybe_unused]] Collider* other)
{
}

void Sword::Attack()
{

}
