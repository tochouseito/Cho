#include "PrecompiledHeader.h"
#include "StreetLight.h"

void StreetLight::Initialize()
{
	pointLight->active = true;
	pointLight->intensity = 4.0f;
}

void StreetLight::Update()
{
}

void StreetLight::SetPosition(const Vector3& position, const uint32_t& index)
{
	switch (index)
	{
	case 0:
		pointLight->position = position + leftTopOffset;
		break;
	case 1:
		pointLight->position = position + rightTopOffset;
		break;
	case 2:
		pointLight->position = position + leftBottomOffset;
		break;
	case 3:
		pointLight->position = position + rightBottomOffset;
		break;
	default:
		break;
	}
}
