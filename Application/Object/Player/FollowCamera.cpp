#include "FollowCamera.h"

FollowCamera::~FollowCamera()
{
	delete camera_;
}


void FollowCamera::Initialize()
{
	camera_ = new Camera();
	camera_->Initialize();
	//camera_->transform.translate = { 0.0f,20.0f,-10.0f };
	camera_->transform.rotate.x = 0.2f;

}

void FollowCamera::Update()
{
	if (target_)

	{
		//追従対象からカメラまでのオフセット
		offset = { 0.0f,0.0f,-20.0f };

		Matrix4x4 rotateX = MakeRotateXMatrix(0.2f);
		Matrix4x4 rotateY = MakeRotateYMatrix(0.0f);
		Matrix4x4 rotateZ = MakeRotateZMatrix(0.0f);
		Matrix4x4 rotation = Multiply(rotateX, Multiply(rotateY, rotateZ));

		offset = TransformNormal(offset, rotation);

		camera_->transform.translate = Add(target_->translation_, offset);


	}


	camera_->UpdateMatrix();
	camera_->TransferMatrix();

}


void FollowCamera::CameraDebug()
{
#ifdef _DEBUG
	ImGui::Begin("followCamera");

	float translate[3] = { camera_->transform.translate.x,camera_->transform.translate.y,camera_->transform.translate.z };
	ImGui::DragFloat3("transform", translate, 0.01f);
	float rotate[3] = { camera_->transform.rotate.x,camera_->transform.rotate.y,camera_->transform.rotate.z };
	ImGui::DragFloat3("rotate", rotate, 0.01f);


	camera_->transform.translate = { translate[0],translate[1],translate[2] };
	camera_->transform.rotate = { rotate[0],rotate[1],rotate[2] };


	ImGui::End();

#endif // _DEBUG




}
