#include "PrecompiledHeader.h"
#include "TitleGround.h"

void TitleGround::Initialize()
{
	SetModel("field");

	// モデルの向きを修正
	transform->degrees.y = -90.0f;

	material->enableLighting = 1;
}

void TitleGround::Update()
{
	
}
