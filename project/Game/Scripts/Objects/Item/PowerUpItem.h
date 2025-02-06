#pragma once
#include "BaseItem.h"
class PowerUpItem :
    public BaseItem
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;

	void Reset() override;
};

