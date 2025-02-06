#include "PrecompiledHeader.h"
#include "TitleLight.h"

void TitleLight::Initialize()
{
	dirLight->direction = Vector3(-1.0f, -1.0f, 1.0f);
	dirLight->active = 1;
	dirLight->color = Vector3(1.0f, 0.6f, 1.0f);
}

void TitleLight::Update()
{
}
