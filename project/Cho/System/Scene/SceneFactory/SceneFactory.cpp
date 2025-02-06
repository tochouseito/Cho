#include "PrecompiledHeader.h"
#include "SceneFactory.h"
#include "Scene/MainScene.h"
#include "Game/Scenes/GamePlayScene/GamePlayScene.h"
#include "Game/Scenes/TitlePlayScene/TitlePlayScene.h"
#include "Game/Scenes/ResultScene/ResultScene.h"
#include "Game/Scenes/GameOverScene/GameOverScene.h"
#include <cassert>

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
	else if (sceneName == "Title") {
		newScene = new TitlePlayScene();
	}
	else if (sceneName == "Result") {
		newScene = new ResultScene();
	}
	else if (sceneName == "GameOver") {
		newScene = new GameOverScene();
	}
	else {
		// 未定義のシーン名
		assert(0);
	}
	return newScene;
}
