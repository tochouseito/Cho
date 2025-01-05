#pragma once

//C++
#include<string>

#include"ECS/ComponentManager/Components/Components.h"
class SceneManager;
class ComponentManager;

class Player
{
public:
	void Init(SceneManager* sceneManager, ComponentManager* compManager);

	void Update();

private:

	void Input();

	

public:
	const std::string name = "Player";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;

	// Components
	TransformComponent* tf = nullptr;
	MeshComponent* mesh = nullptr;
	MaterialComponent* material = nullptr;

	const std::string modelName = "BaseHuman";
};

