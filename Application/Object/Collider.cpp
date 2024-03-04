#include "Collider.h"

void Collider::Initialize()
{
	model_.reset(Model::Create("Resources/DefaultAssets/sphere.obj"));
}

void Collider::UpdateWorldTransform()
{
	model_->worldTransform_->translation_ = GetWorldPosition();
	model_->worldTransform_->UpdateWorldMatrix();
	
}

void Collider::Draw(Camera* camera)
{
	model_->Draw(camera);
}
