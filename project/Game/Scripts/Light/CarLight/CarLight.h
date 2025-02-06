#pragma once
#include "Game/Scripts/Light/BaseLight/BaseLight.h"
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

class CarLight :
	public BaseLight
{
public:
	// BaseLight を介して継承されました
	void Initialize() override;
	void Update() override;

	void Update(const uint32_t& index);

	void SetPosition(const Vector3& position, const uint32_t& index);
	void SetParent(BaseObject* parent) { parent_ = parent; }

private:
	// 車のライトの位置に合わせるためのオフセット
	// 上から見た時の位置
	Vector3 leftFrontOffset = Vector3(3.5f, 0.5f, 2.0f);// 左前
	Vector3 rightFrontOffset = Vector3(3.5f, 0.5f, -2.0f);// 右前

	BaseObject* parent_ = nullptr;
};

