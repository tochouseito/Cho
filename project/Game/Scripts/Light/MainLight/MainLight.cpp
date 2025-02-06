#include "PrecompiledHeader.h"
#include "MainLight.h"

void MainLight::Initialize()
{
	dirLight->direction = Vector3(-1.0f, -1.0f, 1.0f);
	dirLight->active = true;
	dirLight->color = Vector3(1.0f, 0.6f, 1.0f);
}

void MainLight::Update()
{
}
