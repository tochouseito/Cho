#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

// フロントライト
#include "Game/Scripts/Light/CarLight/CarLight.h"

// C++
#include <memory>
#include <array>

class TitleCar :
	public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

private:
	std::array<std::unique_ptr<CarLight>, 2> carLights;// 車のライト2つ
};

