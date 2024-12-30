#pragma once

#include"ECS/ComponentManager/Components/CameraComponent/CameraComponent.h"

class ComponentManager;
class SystemManager;

class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ComponentManager* componentManager,SystemManager* systemManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:// getter
	const CameraComponent& GetCameraComponent()const { return cameraComponent; }

public:// Setter
	void SetAspect(const float& newAspect) { cameraComponent.aspectRatio = newAspect; }
private:
	// ポインタ
	ComponentManager* componentManager_ = nullptr;
	SystemManager* systemManager_ = nullptr;

	CameraComponent cameraComponent;
};

