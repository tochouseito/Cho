#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
class TitleGround :
	public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

	Vector3 GetPos() const { return transform->translation; }
};

