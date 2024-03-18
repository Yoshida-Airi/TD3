#include "Collider.h"

void Collider::Initialize()
{
	model_.reset(Model::Create("Resources/DefaultAssets/sphere.obj"));
}

void Collider::UpdateWorldTransform()
{
	model_->worldTransform_->translation_ = GetWorldPosition();
	model_->worldTransform_->scale_.x = radius_.x;
	model_->worldTransform_->scale_.y = radius_.y;
	model_->worldTransform_->scale_.z = radius_.z;
	model_->worldTransform_->UpdateWorldMatrix();
	
}

void Collider::Draw(Camera* camera)
{
	model_->Draw(camera);
}
