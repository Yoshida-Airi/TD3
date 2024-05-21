#include "CollisionManager.h"

void CollisionManager::Initialize()
{
	for (Collider* collider : colliders_)
	{
		collider->Initialize();
	}
}

void CollisionManager::UpdateWorldTransform()
{
	for (Collider* collider : colliders_)
	{
		collider->UpdateWorldTransform();
	}
}

void CollisionManager::Draw( Camera* camera)
{
	for (Collider* collider : colliders_)
	{
		collider->Draw(camera);
	}
}

void CollisionManager::ListClear()
{
	colliders_.clear(); 
}

/// 衝突判定と応答
void CollisionManager::ChackAllCollisions() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			// コライダーAトコライダーBの当たり判定
			CheakCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheakCollisionPair(Collider* colliderA, Collider* colliderB) {

	// 衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() ||
		colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask()) {
		return;
	}
	// コライダーAのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	// コライダーBのワールド座標を取得
	Vector3 posB = colliderB->GetWorldPosition();

	if (colliderA->GetTypeID() == static_cast<uint32_t>(CollisionTypeDef::kPlayer))
	{
		playerRotate = colliderA->GetRotate();
		playerTranslate = colliderA->GetWorldPosition();
	}

	if (colliderA->GetTypeID() == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		Collider::OBB obb
		{
			posA,//obbの中心 x座標の位置に違和感
			{{1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f}},//座標軸　正規化・直交必須 0=x 1=y 2=z
			colliderA->GetRadius()//中心から各面までの距離　
		};
		Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(colliderA->GetRotate().x + playerRotate.x), Multiply(MakeRotateYMatrix(colliderA->GetRotate().y + playerRotate.y), MakeRotateYMatrix(colliderA->GetRotate().z + playerRotate.z)));
		obb.orientations[0].x = rotateMatrix.m[0][0];
		obb.orientations[0].y = rotateMatrix.m[0][1];
		obb.orientations[0].z = rotateMatrix.m[0][2];

		obb.orientations[1].x = rotateMatrix.m[1][0];
		obb.orientations[1].y = rotateMatrix.m[1][1];
		obb.orientations[1].z = rotateMatrix.m[1][2];

		obb.orientations[2].x = rotateMatrix.m[2][0];
		obb.orientations[2].y = rotateMatrix.m[2][1];
		obb.orientations[2].z = rotateMatrix.m[2][2];

		Matrix4x4 OBBWorldMatrix
		{
			obb.orientations[0].x,
			obb.orientations[0].y,
			obb.orientations[0].z,
			0.0f,

			obb.orientations[1].x,
			obb.orientations[1].y,
			obb.orientations[1].z,
			0.0f,

			obb.orientations[2].x,
			obb.orientations[2].y,
			obb.orientations[2].z,
			0.0f,

			obb.center.x,
			obb.center.y,
			obb.center.z,
			1.0f
		};
		Collider::Sphere sphere
		{
			posB,
			colliderB->GetRadius().x
		};
		Matrix4x4 OBBWorldMatrixInverce = Inverse(OBBWorldMatrix);
		Vector3 centerInOBBLocalSpace = CoorTransform(sphere.center, OBBWorldMatrixInverce);
		Collider::AABB aabbOBBLocal{ {-obb.size.x,-obb.size.y,-obb.size.z},{0.0f,obb.size.y,obb.size.z} };
		Collider::Sphere sphereOBBLocal{ centerInOBBLocalSpace ,sphere.radius };
		Vector3 closestPoint
		{
			std::clamp(sphereOBBLocal.center.x,aabbOBBLocal.min.x,aabbOBBLocal.max.x),
			std::clamp(sphereOBBLocal.center.y,aabbOBBLocal.min.y,aabbOBBLocal.max.y),
			std::clamp(sphereOBBLocal.center.z,aabbOBBLocal.min.z,aabbOBBLocal.max.z)
		};
		float distance = Length(Subtract(closestPoint, sphereOBBLocal.center));
		if (distance <= sphereOBBLocal.radius)
		{
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA);
		}
	}
	else if (colliderB->GetTypeID() == static_cast<uint32_t>(CollisionTypeDef::kPlayerWeapon))
	{
		Collider::OBB obb
		{
			posB,
			{{1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f}},
			colliderB->GetRadius()
		};
		Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(colliderB->GetRotate().x + playerRotate.x), Multiply(MakeRotateYMatrix(colliderB->GetRotate().y + playerRotate.y), MakeRotateYMatrix(colliderB->GetRotate().z + playerRotate.z)));
		obb.orientations[0].x = rotateMatrix.m[0][0];
		obb.orientations[0].y = rotateMatrix.m[0][1];
		obb.orientations[0].z = rotateMatrix.m[0][2];

		obb.orientations[1].x = rotateMatrix.m[1][0];
		obb.orientations[1].y = rotateMatrix.m[1][1];
		obb.orientations[1].z = rotateMatrix.m[1][2];

		obb.orientations[2].x = rotateMatrix.m[2][0];
		obb.orientations[2].y = rotateMatrix.m[2][1];
		obb.orientations[2].z = rotateMatrix.m[2][2];

		Matrix4x4 OBBWorldMatrix
		{
			obb.orientations[0].x,
			obb.orientations[0].y,
			obb.orientations[0].z,
			0.0f,

			obb.orientations[1].x,
			obb.orientations[1].y,
			obb.orientations[1].z,
			0.0f,

			obb.orientations[2].x,
			obb.orientations[2].y,
			obb.orientations[2].z,
			0.0f,

			obb.center.x,
			obb.center.y,
			obb.center.z,
			1.0f
		};
		Collider::Sphere sphere
		{
			posA,
			colliderA->GetRadius().x
		};
		Matrix4x4 OBBWorldMatrixInverce = Inverse(OBBWorldMatrix);
		Vector3 centerInOBBLocalSphace = CoorTransform(sphere.center, OBBWorldMatrixInverce);
		Collider::AABB aabbOBBLocal{ {-obb.size.x,-obb.size.y,-obb.size.z},{0.0f,obb.size.y,obb.size.z} };
		Collider::Sphere sphereOBBLocal{ centerInOBBLocalSphace ,sphere.radius };
		Vector3 closestPoint
		{
			std::clamp(sphereOBBLocal.center.x,aabbOBBLocal.min.x,aabbOBBLocal.max.x),
			std::clamp(sphereOBBLocal.center.y,aabbOBBLocal.min.y,aabbOBBLocal.max.y),
			std::clamp(sphereOBBLocal.center.z,aabbOBBLocal.min.z,aabbOBBLocal.max.z)
		};
		float distance = Length(Subtract(closestPoint, sphereOBBLocal.center));
		if (distance <= sphereOBBLocal.radius)
		{
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA);
		}
	}
	else
	{
		// 座標AとBの距離を求める
		Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };

		// 各軸方向の距離の二乗を計算
		float distanceSquaredX = distance.x * distance.x;
		float distanceSquaredY = distance.y * distance.y;
		float distanceSquaredZ = distance.z * distance.z;

		// 各軸方向の半径を取得
		Vector3 radiusA = colliderA->GetRadius();
		Vector3 radiusB = colliderB->GetRadius();

		// 各軸方向の半径の和を計算
		float radiusSumX = radiusA.x + radiusB.x;
		float radiusSumY = radiusA.y + radiusB.y;
		float radiusSumZ = radiusA.z + radiusB.z;

		// 球と球の交差判定
		if (distanceSquaredX / (radiusSumX * radiusSumX) +
			distanceSquaredY / (radiusSumY * radiusSumY) +
			distanceSquaredZ / (radiusSumZ * radiusSumZ) <= 1.0f)
		{
			// コライダーAの衝突時コールバックを呼び出す
			colliderA->OnCollision(colliderB);
			// コライダーBの衝突時コールバックを呼び出す
			colliderB->OnCollision(colliderA);
		}
	}
}

void CollisionManager::AddColliders(Collider* collider)
{
	//リストに登録する
	colliders_.push_back(collider);
}