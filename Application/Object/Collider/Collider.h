#pragma once
#include"VectorMath.h"
#include"WorldTransform.h"
#include"Model.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
public:

	void Initialize();
	void UpdateWorldTransform();
	void Draw( Camera* camera);

	struct Sphere {
		Vector3 center;
		float radius;
	};

	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	struct OBB {
		Vector3 center;
		Vector3 orientations[3];
		Vector3 size;
	};
	OBB GetOBB()const { return obb_; }

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	virtual Vector3 GetRotate() = 0;
	virtual void SetPosition(Vector3 translation) = 0;

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

	//半径を取得
	Vector3 GetRadius() const { return radius_; };

	struct Sphere {
		Vector3 center;
		float radius;
	};
	
	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	struct OBB {
		Vector3 center;
		Vector3 orientations[3];
		Vector3 size;
	};
	OBB GetOBB()const { return obb_; }

	//半径を設定
	void SetRadius(Vector3 radius) { radius_ = radius; };
	void SetOBB(OBB obb) { obb_ = obb; }


	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; };
	//衝突属性(自分)を指定
	void SetCollisionAttribute(uint32_t attribute)
	{
		collisionAttribute_ = attribute;
	}

	//衝突マスク(相手)を取得
	uint32_t GetCollisionMask() const { return CollisionMask_; };

	//衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t mask)
	{
		CollisionMask_ = mask;
	}

	//種別IDを取得
	uint32_t GetTypeID()const { return typeID_; };
	//種別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; };

private:
	//衝突半径
	Vector3 radius_ = { 1.0f,1.0f,1.0f };
	uint32_t typeID_ = 0u;
	OBB obb_ = { {1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f} };



	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
	WorldTransform* worldTransform;

	std::unique_ptr<Model>model_;

};