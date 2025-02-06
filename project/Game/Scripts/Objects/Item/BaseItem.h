#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
#include "Game/Scripts/Collider/Collider.h"

class BaseItem :
    public BaseObject, public Collider
{
public:
	// BaseObject を介して継承されました
	void Initialize() override {};
	void Update() override;

	virtual void Reset() = 0;

	void SetPosition(const Vector3& position);

	void SetScale(const Scale& scale);

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override {
		return Vector3{
		transform->matWorld.m[3][0],
		transform->matWorld.m[3][1],
		transform->matWorld.m[3][2]
		};
	}

	bool GetIsItemCollected() { return isItemCollected; }

protected:
	bool isItemCollected = false;   // アイテムが取得済みか

	Scale defaultScale_ = {};
	float scale_ = 1.0f;
	const float kMaxScale_ = 1.5f;
	bool isScaleUp_ = true;


	float floatY_ = 0.4f;
	bool isUp_ = true;
	bool isDown_ = false;


};

