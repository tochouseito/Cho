//C++
#include<string>

#include"ECS/ComponentManager/Components/Components.h"

class SceneManager;
class ComponentManager;
class FollowCamera
{
public:
	void Init(SceneManager* sceneManager, ComponentManager* compManager);

	void Update();

public:
	const std::string name = "FollowCamera";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;

	// Components
	CameraComponent* comp = nullptr;
};