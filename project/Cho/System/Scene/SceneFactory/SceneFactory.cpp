#include "PrecompiledHeader.h"
#include "SceneFactory.h"
#include"Scene/MainScene.h"
#include"Game/Scens/GameScene/GameScene.h"
#include"Game/Scens/TitleScene/TitleScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	/*次のシーン生成*/
	BaseScene* newScene = nullptr;
	if (sceneName == "MainScene") {
		newScene = new MainScene();
	}
	else if (sceneName == "Game") {
		newScene = new GameScene();
	}
	else if (sceneName == "Title") {
		newScene = new TitleScene();
	}
	return newScene;
}
