#pragma once
#include"ChoMath.h"
struct AnimationComponent
{
	float time = 0.0f;
	float transitionTime = 0.0f;// 遷移中経過時間
	float transitionDuration = 0.5f;// 遷移にかける時間
	float lerpTime = 0.0f;// 線形補間の時間
	bool transition = false;
	std::string modelName = "";
	uint32_t numAnimation = 0;// アニメーションの数
	uint32_t animationIndex = 0;// アニメーションのIndex
	uint32_t prevAnimationIndex = 0;// 1つ前のアニメーションのIndex
	uint32_t transitionIndex = 0;// 遷移スタートのアニメーションのIndex
};