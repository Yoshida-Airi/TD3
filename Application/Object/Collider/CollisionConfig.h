#pragma once

#include<cstdint>

enum class CollisionTypeDef : uint32_t
{
	kDefault,
	kPlayer,
	kPlayerWeapon,
	kEnemy,
	kEnemyBullet
};
