#include "PrecompiledHeader.h"
#include "PowerUpItem.h"

void PowerUpItem::Initialize()
{
	SetModel("fire");
	defaultScale_ = { 0.7f, 0.7f, 0.7f };
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kPower);
}

void PowerUpItem::Reset()
{
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kPower);
	isItemCollected = false;
}
