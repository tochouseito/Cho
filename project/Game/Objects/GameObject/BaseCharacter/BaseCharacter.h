#pragma once
#include"Game/Collider/Collider.h"
#include"ChoMath.h"
class BaseCharacter : public Collider
{
protected:
	Matrix4 matWorld;
public:
	// 中心座標を取得
	virtual Vector3 GetCenterPosition() const override;
};

