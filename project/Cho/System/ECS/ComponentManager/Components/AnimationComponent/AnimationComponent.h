#pragma once
#include"ChoMath.h"
struct AnimationComponent
{
	float time = 0.0f;

	std::string modelName = "";

	uint32_t animationIndex = 0;
};