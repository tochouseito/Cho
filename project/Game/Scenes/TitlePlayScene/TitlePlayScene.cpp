#include "PrecompiledHeader.h"
#include "TitlePlayScene.h"

// SceneManager
#include "Scene/SceneManager/SceneManager.h"

// InputManager
#include "Input/InputManager.h"

// AudioManager
#include "Audio/AudioManager.h"


#include "Game/Scripts/GlobalVariables/GlobalVariables.h"

void TitlePlayScene::Initialize()
{
	SceneManager* sceneManager = GetSceneManager();
	ComponentManager* componentManager = sceneManager->GetCompPtr();
	input = GetSceneManager()->GetInputManagerPtr();
	audio = GetSceneManager()->GetAudioManagerPtr();

	titleCamera = std::make_unique<TitleCamera>();
	titleCamera->Create("TitleCamera", sceneManager,componentManager, input);
	titleCamera->Initialize();

	titleGround = std::make_unique<TitleGround>();
	titleGround->Create("TitleGround", sceneManager, componentManager, input);
	titleGround->Initialize();

	titleCar = std::make_unique<TitleCar>();
	titleCar->Create("TitleCar", sceneManager, componentManager, input);
	titleCar->Initialize();

	titleLight = std::make_unique<TitleLight>();
	titleLight->Create("TitleLight",LightType::Directional, sceneManager, componentManager, input);
	titleLight->Initialize();

	for (int i = 0; i < streetLight.size(); i++)
	{
		streetLight[i] = std::make_unique<StreetLight>();
		streetLight[i]->Create("StreetLight", LightType::Point, sceneManager, componentManager, input);
		streetLight[i]->Initialize();
		streetLight[i]->SetPosition(titleGround->GetPos(), i);
	}

	titleSprite = std::make_unique<TitleSprite>();
	titleSprite->Create("TitleSprite", sceneManager, componentManager, input);
	titleSprite->Initialize();

	titleStartSprite = std::make_unique<TitleStartSprite>();
	titleStartSprite->Create("TitleStartSprite", sceneManager, componentManager, input);
	titleStartSprite->Initialize();

	// Fade
	fade = std::make_unique<Fade>();
	fade->Create("FadeSprite", sceneManager, componentManager, input);
	fade->Initialize();
	fade->Start(Fade::Status::FadeIn, 1.0f);

	// 背景
	backGround = std::make_unique<BackGround>();
	backGround->Create("BackGround", sceneManager, componentManager, input);
	backGround->Initialize();

	// BGM
	bgmData = audio->SoundLordWave("Game/Assets/Sounds/BGM.wav");
	audio->SoundPlayWave(bgmData, true);

	// SE
	buttonSE = audio->SoundLordWave("Game/Assets/Sounds/botan.wav");

	GlobalVariables::GetInstance()->LoadFiles();
}

void TitlePlayScene::Finalize()
{
	audio->SoundUnLord(&bgmData);
	audio->SoundUnLord(&buttonSE);
}

void TitlePlayScene::Update()
{
	GlobalVariables::GetInstance()->Update();
	titleCar->Update();

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

void TitlePlayScene::Draw()
{
}

void TitlePlayScene::ChangeScene()
{
	GetSceneManager()->ChangeScene("Game");
}
