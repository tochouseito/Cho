//C++
#include<string>

#include"ECS/ComponentManager/Components/Components.h"

class SceneManager;
class ComponentManager;
class InputManager;
class FollowCamera
{
public:
	void Init(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);

	void Update();

private:

	void Input();

	void Move();

public:
	const std::string name = "FollowCamera";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;
	InputManager* input = nullptr;

	// Components
	CameraComponent* comp = nullptr;

	// 追従対象
	const std::string targetName = "Player";

	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = { 0.0f, 20.0f, -30.0f };
};