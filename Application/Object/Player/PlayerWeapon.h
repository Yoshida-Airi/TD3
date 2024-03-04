#pragma once
#include"Model.h"

class PlayerWeapon
{
public:

	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void SetPosition(Vector3 position);

private:
	std::unique_ptr<Model> weapon = nullptr;
	
};

