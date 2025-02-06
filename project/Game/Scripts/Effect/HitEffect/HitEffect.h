#pragma once
#include "Game/Scripts/Effect/BaseEffect/BaseEffect.h"
class HitEffect : public BaseEffect
{
	// BaseEffect を介して継承されました
	void Initialize() override;
	void Update() override;
};

