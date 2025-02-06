#pragma once
#include "Game/Scripts/Light/BaseLight/BaseLight.h"
class MainLight :
	public BaseLight
{
public:
	// BaseLight を介して継承されました
	void Initialize() override;
	void Update() override;
};

