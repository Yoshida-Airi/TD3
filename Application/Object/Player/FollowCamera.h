#pragma once
#include"Camera.h"
#include"WorldTransform.h"

class FollowCamera
{
public:
	~FollowCamera();
	void Initialize();
	void Update();

	void CameraDebug();

	void SetTarget(const WorldTransform* target) { target_ = target; };
	Camera* GetCamera() { return camera_; };

private:

	Camera* camera_ = nullptr;
	const WorldTransform* target_ = nullptr;
	Vector3 offset;

};

