#include "PrecompiledHeader.h"
#include "SpeedUpItem.h"

void SpeedUpItem::Initialize()
{
	SetModel("shose");
	defaultScale_ = { 0.5f, 0.5f, 1.0f };
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kSpeed);
}

void SpeedUpItem::Reset()
{
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kSpeed);
	isItemCollected = false;
}
