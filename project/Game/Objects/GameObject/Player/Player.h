#pragma once

//C++
#include<string>

class ComponentManager;

class Player
{
public:
	void Init(ComponentManager* compManager);

	void Update();

public:
	const std::string name = "Player";
private:

	ComponentManager* compManager_ = nullptr;
};

