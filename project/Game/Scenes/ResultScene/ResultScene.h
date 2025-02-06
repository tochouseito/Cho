#pragma once
#include "Scene/BaseScene/BaseScene.h"

// Camera
#include "Game/Scripts/Camera/ResultCamera/ResultCamera.h"

// Fade
#include "Game/Scripts/SceneTransition/Fade/Fade.h"

// Sprite
#include "Game/Scripts/Sprite/Result/ClearSprite.h"

// Audio
#include "Audio/AudioManager.h"

// Light
#include "Game/Scripts/Light/StreetLight/StreetLight.h"
#include "Game/Scripts/Light/TitleLight/TitleLight.h"

// Object
#include "Game/Scripts/Objects/ResultPlayer/ResultPlayer.h"
#include "Game/Scripts/Objects/TitleGround/TitleGround.h"
#include "Game/Scripts/Objects/BackGround/BackGround.h"

// C++
#include <memory>
#include <array>

class InputManager;
class AudioManager;
class SceneManager;

class ResultScene : public BaseScene
{
public:
	// BaseScene を介して継承されました
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

	void ChangeScene() override;

private:
	InputManager* input = nullptr;
	AudioManager* audio = nullptr;

	// シーン遷移用フェード
	std::unique_ptr<Fade> fade = nullptr;
	// 現在のフェーズ
	Phase phase = Phase::kFadeIn;

	// リザルトカメラ
	std::unique_ptr<ResultCamera> resultCamera = nullptr;

	std::unique_ptr<ClearSprite> clearSpr = nullptr;

	// タイトルライト
	std::unique_ptr<TitleLight> titleLight = nullptr;

	// 街灯
	std::array<std::unique_ptr<StreetLight>, 4> streetLight = { nullptr,nullptr };

	// プレイヤーキャラクター
	std::unique_ptr<ResultPlayer> playerCharacter = nullptr;

	// タイトル地面
	std::unique_ptr<TitleGround> titleGround = nullptr;

	// 背景
	std::unique_ptr<BackGround> backGround = nullptr;

	AudioManager::SoundData buttonSE;
	AudioManager::SoundData SE;
};

