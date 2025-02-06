#pragma once

#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

class BackGround : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;
	void SetPositionX(const float& x);
private:

};

