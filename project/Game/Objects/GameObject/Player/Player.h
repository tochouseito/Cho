#pragma once

//C++
#include<string>

#include"ECS/ComponentManager/Components/Components.h"
class SceneManager;
class ComponentManager;
class InputManager;

class Player
{
public:
	void Init(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);

	void Update();

private:

	void Input();

	void Move();

	void Jump();

	void Fall();

public:
	const std::string name = "Player";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;
	InputManager* input = nullptr;

	// Components
	TransformComponent* tf = nullptr;
	MeshComponent* mesh = nullptr;
	MaterialComponent* material = nullptr;

	const std::string modelName = "BaseHuman";

	// parm
	Vector3 velocity;
	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;

	const float fallVelocity = 0.5f;

	// 重力加速度
	const float kGravityAcceleration = 0.05f;

	// flag
	bool isJump = false;
};

