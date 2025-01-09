#include "PrecompiledHeader.h"
#include "BaseCharacter.h"

Vector3 BaseCharacter::GetCenterPosition() const
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = matWorld.m[3][0];
	worldPos.y = matWorld.m[3][1];
	worldPos.z = matWorld.m[3][2];

	return worldPos;
}
