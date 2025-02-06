#include "PrecompiledHeader.h"
#include "BackGround.h"

void BackGround::Initialize()
{
	SetModel("backGround");

	// モデルの回転修正用
	transform->degrees.y = 180.0f;

	material->enableLighting = true;

	render->visible = true;
}

void BackGround::Update()
{
}

void BackGround::SetPositionX(const float& x)
{
	transform->translation.x = x;
}
