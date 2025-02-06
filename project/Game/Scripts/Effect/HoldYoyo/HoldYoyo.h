#pragma once
#include "Game/Scripts/Effect/BaseEffect/BaseEffect.h"
class HoldYoyo : public BaseEffect
{
public:
	// BaseEffect を介して継承されました
	void Initialize() override;
	void Update() override;
};

