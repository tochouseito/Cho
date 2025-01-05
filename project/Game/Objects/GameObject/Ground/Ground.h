#pragma once
//C++
#include<string>

#include"ECS/ComponentManager/Components/Components.h"

class SceneManager;
class ComponentManager;
class Ground
{
public:
	void Init(SceneManager* sceneManager,ComponentManager* compManager);

	void Update();

public:
	const std::string name = "Ground";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;

	// Components
	TransformComponent* tf = nullptr;
	MeshComponent* mesh = nullptr;
};

