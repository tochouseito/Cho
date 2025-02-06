#pragma once
#include "Game/Scripts/Camera/BaseCamera/BaseCamera.h"
class ResultCamera : public BaseCamera
{
public:
	// BaseCamera を介して継承されました
	void Initialize() override;
	void Update() override;
};

