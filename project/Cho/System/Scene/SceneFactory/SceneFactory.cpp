#include "PrecompiledHeader.h"
#include "SceneFactory.h"
#include"Scene/MainScene.h"
#include"Game/Scenes/GamePlayScene/GamePlayScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	/*次のシーン生成*/
	BaseScene* newScene = nullptr;
	if (sceneName == "MainScene") {
		newScene = new MainScene();
	}
	else if (sceneName == "Game") {
		newScene = new GamePlayScene();
	}
	else {
		// 未定義のシーン名
		assert(0);
	}
	return newScene;
}
