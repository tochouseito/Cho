#include "PrecompiledHeader.h"
#include "HealingItem.h"

void HealingItem::Initialize()
{
	SetModel("heart");
	defaultScale_ = { 0.4f, 0.4f, 0.4f };
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kHP);
}

void HealingItem::Reset()
{
	transform->scale = defaultScale_;
	SetRadius(0.1f);
	SetTypeID(TypeID::kHP);
	isItemCollected = false;
}
