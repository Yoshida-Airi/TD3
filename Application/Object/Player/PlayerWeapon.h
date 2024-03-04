#pragma once
#include"Model.h"
#include"Collider.h"

class PlayerWeapon :public Collider
{
public:

	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void SetPosition(Vector3 position);

	Vector3 GetWorldPosition()override;
	Vector3 GetCenterPosition()const override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	std::unique_ptr<Model> model = nullptr;
	
};

