#pragma once

// BaseScene
#include "Scene/BaseScene/BaseScene.h"

// Camera
#include "Game/Scripts/Camera/TitleCamera/TitleCamera.h"

// Object
#include "Game/Scripts/Objects/TitleGround/TitleGround.h"
#include "Game/Scripts/Objects/TitleCar/TitleCar.h"
#include "Game/Scripts/Objects/BackGround/BackGround.h"

// Light
#include "Game/Scripts/Light/TitleLight/TitleLight.h"
#include "Game/Scripts/Light/StreetLight/StreetLight.h"

// Sprite
#include "Game/Scripts/Sprite/TitleSprite/TitleSprite.h"
#include "Game/Scripts/Sprite/TitleStartSprite/TitleStartSprite.h"

// Fade
#include "Game/Scripts/SceneTransition/Fade/Fade.h"

// C++
#include <memory>
#include <array>

class InputManager;
class AudioManager;
class TitlePlayScene : public BaseScene
{
public:
	// BaseScene を介して継承されました
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void ChangeScene() override;
private:
	InputManager* input=nullptr;
	AudioManager* audio = nullptr;

	// シーン遷移用フェード
	std::unique_ptr<Fade> fade = nullptr;
	// 現在のフェーズ
	Phase phase = Phase::kFadeIn;

	// タイトルカメラ
	std::unique_ptr<TitleCamera> titleCamera = nullptr;

	// タイトル地面
	std::unique_ptr<TitleGround> titleGround = nullptr;

	// タイトル用の車
	std::unique_ptr<TitleCar> titleCar = nullptr;

	// タイトルライト
	std::unique_ptr<TitleLight> titleLight = nullptr;

	// 街灯
	std::array<std::unique_ptr<StreetLight>, 4> streetLight = { nullptr,nullptr };

	// タイトルスプライト
	std::unique_ptr<TitleSprite> titleSprite = nullptr;

	// タイトルスタートスプライト
	std::unique_ptr<TitleStartSprite> titleStartSprite = nullptr;

	// 背景
	std::unique_ptr<BackGround> backGround = nullptr;

	// BGM
	AudioManager::SoundData bgmData;

	// SE
	AudioManager::SoundData buttonSE;
};

