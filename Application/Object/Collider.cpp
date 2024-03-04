#include "Collider.h"

void Collider::Initialize()
{
	model_.reset(Model::Create("Resources/DefaultAssets/sphere.obj"));
}

void Collider::UpdateWorldTransform()
{
	model_->worldTransform_->translation_ = GetWorldPosition();
	model_->worldTransform_->scale_.x = radius_;
	model_->worldTransform_->scale_.y = radius_;
	model_->worldTransform_->scale_.z = radius_;
	model_->worldTransform_->UpdateWorldMatrix();
	
}

void Collider::Draw(Camera* camera)
{
	model_->Draw(camera);
}
