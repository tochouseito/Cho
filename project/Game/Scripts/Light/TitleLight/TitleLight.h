#pragma once
#include "Game/Scripts/Light/BaseLight/BaseLight.h"
class TitleLight : public BaseLight
{
public:
	// BaseLight を介して継承されました
	void Initialize() override;
	void Update() override;
};

