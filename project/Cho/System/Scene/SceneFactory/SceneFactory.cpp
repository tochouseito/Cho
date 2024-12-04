#include "PrecompiledHeader.h"
#include "SceneFactory.h"
#include"Scene/MainScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	/*次のシーン生成*/
	BaseScene* newScene = nullptr;
	if (sceneName == "MainScene") {
		newScene = new MainScene();
	}
	return newScene;
}
