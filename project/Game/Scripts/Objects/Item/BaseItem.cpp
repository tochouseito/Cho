#include "PrecompiledHeader.h"
#include "BaseItem.h"

void BaseItem::Update()
{
	if (!isItemCollected) {
		if (isUp_) {
			if (transform->translation.y < floatY_ + 1.0f) {
				transform->translation.y += floatY_ * DeltaTime();
			}
			else {
				isUp_ = false;
				isDown_ = true;
			}
		}
		else if (isDown_) {
			if (transform->translation.y > 1.0f) {
				transform->translation.y -= floatY_ * DeltaTime();
			}
			else {
				isDown_ = false;
				isUp_ = true;
			}
		}
		transform->degrees.y += 45.0f * DeltaTime();
	}
	else {
		if (isScaleUp_) {
			if (kMaxScale_ > scale_) {
				scale_ += 2.5f * DeltaTime();
			}
			else {
				isScaleUp_ = false;
			}
		}
		else {
			if (0 < scale_) {
				scale_ -= 3.0f * DeltaTime();
			}
			if (0 > scale_) {
				scale_ = 0;
			}
		}
		SetScale(defaultScale_ * scale_);
	}
}

void BaseItem::SetPosition(const Vector3& position)
{
	transform->translation = position;
}

void BaseItem::SetScale(const Scale& scale)
{
	transform->scale = scale;
}

void BaseItem::OnCollision([[maybe_unused]] Collider* other)
{
	if (other->GetTypeID() == TypeID::kPlayer || other->GetTypeID() == TypeID::kEnemy) {
		isItemCollected = true;
		SetTypeID(TypeID::kNone);
		SetScale({ 0,0,0 });
	}
	else if (other->GetTypeID() == TypeID::kYoyo) {
		SetPosition(other->GetCenterPosition());
	}
}
