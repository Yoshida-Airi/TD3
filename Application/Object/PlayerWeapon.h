#pragma once
#include"Model.h"

class playerWeapon
{
public:

	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void SetPosition(Vector3 position);

private:
	std::unique_ptr<Model> weapon = nullptr;
	
};

