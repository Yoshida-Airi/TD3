#include "Player.h"
#include"PlayerWeapon.h"
#include"CollisionConfig.h"

#include"numbers"

void Player::Initialize(Camera* camera)
{
	input_ = Input::GetInstance();
	camera_ = camera;


	Collider::Initialize();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));

	model_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	directionModel_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));

	sprite2DReticle_.reset(Sprite::Create(direction));
	sprite2DReticle_->SetAnchorPoint({ 0.5f,0.5f });
	sprite2DReticle_->worldTransform_->translation_ = { 640.0f,360.0f };
	//sprite2DReticle_->SetPosition({ 640.0f,360.0f });

	SetRadius(model_->worldTransform_->scale_.x);

}

void Player::Update()
{
	Collider::UpdateWorldTransform();
	model_->Update();
	directionModel_->Update();
	sprite2DReticle_->Update();

	model_->ModelDebug("player");
	directionModel_->ModelDebug("playerDirection");

#ifdef _DEBUG

	ImGui::Begin("HitCheack");
	ImGui::Text("HP : %d", HP);
	ImGui::End();

#endif // _DEBUG

	

	Move();

	Attack();

	Skill();

	Direction();

	Set3DReticleMousePosition(camera_);

}

void Player::Draw()
{
	model_->Draw(camera_);
	directionModel_->Draw(camera_);

	sprite2DReticle_->Draw(camera_);

}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = model_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = model_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = model_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}


void Player::OnCollision([[maybe_unused]] Collider* other)
{

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		HP -= 1;
	}

	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemyBullet))
	{
		HP -= 1;
	}
}

void Player::Move()
{
	if (input_->PushKey(DIK_W))
	{
		model_->worldTransform_->translation_.z += Speed;
	}
	if (input_->PushKey(DIK_S))
	{
		model_->worldTransform_->translation_.z -= Speed;
	}
	if (input_->PushKey(DIK_A))
	{
		model_->worldTransform_->translation_.x -= Speed;
	}
	if (input_->PushKey(DIK_D))
	{
		model_->worldTransform_->translation_.x += Speed;
	}



}

void Player::Attack()
{
	if (input_->IsLeftMouseClicked())
	{
		isUnderAttack = true;
	}
	else
	{
		isUnderAttack = false;
	}
}

void Player::Skill()
{
	if (input_->PushKey(DIK_LSHIFT)) {
		isSkill = true;
	}
	else
	{
		isSkill = false;
	}
}

void Player::Direction()
{
	

}

void Player::Set3DReticleMousePosition(const Camera* camera)
{
	XINPUT_STATE joyState;

	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kCilentWidth, WinApp::kCilentHeight, 0, 1);

	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		POINT mousePosition;
		//マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);


		//マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));
	}

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV =
		Multiply(camera->matView, Multiply(camera->matProjection, matViewport));

	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear =
		Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
		Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	Vector2 spritePosirion = sprite2DReticle_->GetPosition();


	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		spritePosirion.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 10.0f;
		spritePosirion.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 10.0f;

		sprite2DReticle_->SetPosition(spritePosirion);
	}

	//スクリーン座標系からワールド座標系へ
	posNear = CoorTransform(posNear, matInverseVPV);
	posFar = CoorTransform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	directionModel_->worldTransform_->translation_ =
		Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

	directionModel_->worldTransform_->UpdateWorldMatrix();


#ifdef _DEBUG
	ImGui::Begin("reticle");
	ImGui::Text(
		"2DReticle:(%f,%f)", sprite2DReticle_->worldTransform_->translation_.x, sprite2DReticle_->worldTransform_->translation_.y);
	ImGui::Text("Near:(%+2f,%+2f,%+2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+2f,%+2f,%+2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
		"3DReticle:(%+2f,%+2f,%+2f)", directionModel_->worldTransform_->translation_.x,
		directionModel_->worldTransform_->translation_.y, directionModel_->worldTransform_->translation_.z);
	ImGui::End();
#endif // _DEBUG



}
