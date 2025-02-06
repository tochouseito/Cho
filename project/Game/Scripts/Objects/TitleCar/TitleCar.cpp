#include "PrecompiledHeader.h"
#include "TitleCar.h"

void TitleCar::Initialize()
{
	SetModel("enemy");

	transform->translation.x = -20.0f;
	transform->translation.y = 1.8f;
	transform->degrees.y = -90.0f;

	material->enableLighting = true;

	// 車のライト生成
	for (int i = 0;i < 2;i++) {
		carLights[i] = std::make_unique<CarLight>();
		carLights[i]->Create("CarLight",LightType::Spot, sceneManager_, compManager_, input);
		carLights[i]->Initialize();
		carLights[i]->SetPosition(transform->translation, i);
		carLights[i]->SetParent(this);
	}
}

void TitleCar::Update()
{
	if (transform->translation.x > 40.0f) {
		transform->translation.x = -20.0f;
	}

	static const float speed = 0.5f;
	transform->translation.x += speed;

	for (int i = 0;i < 2;i++) {
		carLights[i]->Update(i);
	}
}
