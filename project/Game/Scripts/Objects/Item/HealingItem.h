#pragma once
#include "BaseItem.h"
class HealingItem :
    public BaseItem
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;

	void Reset() override;
};

