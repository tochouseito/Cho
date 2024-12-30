#pragma once

#include"ECS/ComponentManager/Components/CameraComponent/CameraComponent.h"

class SystemManager;

class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SystemManager* systemManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:// getter
	CameraComponent GetCameraComponent()const { return cameraComponent; }

private:
	// ポインタ
	SystemManager* systemManager_ = nullptr;

	CameraComponent cameraComponent;
};

