#include "PrecompiledHeader.h"
#include "ResultScene.h"

// SceneManager
#include "Scene/SceneManager/SceneManager.h"

// InputManager
#include "Input/InputManager.h"

// AudioManager
#include "Audio/AudioManager.h"

void ResultScene::Initialize()
{
	SceneManager* sceneManager = GetSceneManager();
	ComponentManager* componentManager = sceneManager->GetCompPtr();
	input = GetSceneManager()->GetInputManagerPtr();
	audio = GetSceneManager()->GetAudioManagerPtr();

	// Fade
	fade = std::make_unique<Fade>();
	fade->Create("FadeSprite", sceneManager, componentManager, input);
	fade->Initialize();
	fade->Start(Fade::Status::FadeIn, 1.0f);

	// ResultCamera
	resultCamera = std::make_unique<ResultCamera>();
	resultCamera->Create("ResultCamera", sceneManager, componentManager, input);
	resultCamera->Initialize();

	clearSpr = std::make_unique<ClearSprite>();
	clearSpr->Create("ClearSprite", sceneManager, componentManager, input);
	clearSpr->Initialize();

	buttonSE = audio->SoundLordWave("Game/Assets/Sounds/botan.wav");
	SE = audio->SoundLordWave("Game/Assets/Sounds/gameclear.wav");
	audio->SoundPlayWave(SE);

	// プレイヤーキャラクター
	playerCharacter = std::make_unique<ResultPlayer>();
	playerCharacter->Create("PlayerCharacter", sceneManager, componentManager, input);
	playerCharacter->Initialize();

	// タイトル地面
	titleGround = std::make_unique<TitleGround>();
	titleGround->Create("TitleGround", sceneManager, componentManager, input);
	titleGround->Initialize();

	titleLight = std::make_unique<TitleLight>();
	titleLight->Create("TitleLight", LightType::Directional, sceneManager, componentManager, input);
	titleLight->Initialize();

	for (int i = 0; i < streetLight.size(); i++)
	{
		streetLight[i] = std::make_unique<StreetLight>();
		streetLight[i]->Create("StreetLight", LightType::Point, sceneManager, componentManager, input);
		streetLight[i]->Initialize();
		streetLight[i]->SetPosition(titleGround->GetPos(), i);
	}

	// 背景
	backGround = std::make_unique<BackGround>();
	backGround->Create("BackGround", sceneManager, componentManager, input);
	backGround->Initialize();
}

void ResultScene::Finalize()
{
	audio->SoundUnLord(&buttonSE);
	audio->SoundUnLord(&SE);
}

void ResultScene::Update()
{
	// シーン遷移
	fade->Update();
	if ((input->TriggerKey(DIK_SPACE) || input->IsTriggerPadButton(PadButton::A)) && fade->IsFinished()) {
		audio->SoundPlayWave(buttonSE);
		fade->Start(Fade::Status::FadeOut, 1.0f);
	}
	if (fade->IsFinished()) {
		if (fade->GetStatus() == Fade::Status::FadeOut) {
			ChangeScene();
		}
	}
}

void ResultScene::Draw()
{
}

void ResultScene::ChangeScene()
{
	BaseScene::GetSceneManager()->ChangeScene("Title");
}
