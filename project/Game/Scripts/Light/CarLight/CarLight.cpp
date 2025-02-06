#include "PrecompiledHeader.h"
#include "CarLight.h"

void CarLight::Initialize()
{
	spotLight->active = true;
	spotLight->cosAngle = 3.0f;
	spotLight->cosFalloffStart = 1.0f;
	spotLight->decay = 2.0f;
	spotLight->intensity = 10.0f;
	// 右を向かせる
	spotLight->direction = Vector3(1.0f, 0.0f, 0.0f);
}

void CarLight::Update()
{
}

void CarLight::Update(const uint32_t& index)
{
	switch (index)
	{
	case 0:
		spotLight->position = parent_->GetTransform()->translation + leftFrontOffset;
		spotLight->position.y = 0.0f;
		break;
	case 1:
		spotLight->position = parent_->GetTransform()->translation + rightFrontOffset;
		spotLight->position.y = 0.0f;
		break;
	default:
		break;
	}
}

void CarLight::SetPosition(const Vector3& position, const uint32_t& index)
{
	switch (index)
	{
	case 0:
		spotLight->position = position + leftFrontOffset;
		break;
	case 1:
		spotLight->position = position + rightFrontOffset;
		break;
	default:
		break;
	}
}
