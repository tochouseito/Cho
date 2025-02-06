#include "PrecompiledHeader.h"
#include "GameOverScene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"

void GameOverScene::Initialize()
{
	input = BaseScene::GetSceneManager()->GetInputManagerPtr();
	audio = BaseScene::GetSceneManager()->GetAudioManagerPtr();

	// ゲームオーバーカメラ
	gameOverCamera = std::make_unique<GameOverCamera>();
	gameOverCamera->Create("GameOverCamera", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	gameOverCamera->Initialize();

	// Fade
	fade = std::make_unique<Fade>();
	fade->Create("FadeSprite", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	fade->Initialize();

	gameOverSpr = std::make_unique<GameOverSprite>();
	gameOverSpr->Create("GameOverSprite", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	gameOverSpr->Initialize();

	buttonSE = audio->SoundLordWave("Game/Assets/Sounds/botan.wav");
	SE = audio->SoundLordWave("Game/Assets/Sounds/gameover.wav");
	audio->SoundPlayWave(SE);

	// タイトル地面
	titleGround = std::make_unique<TitleGround>();
	titleGround->Create("TitleGround", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	titleGround->Initialize();

	// タイトルライト
	titleLight = std::make_unique<TitleLight>();
	titleLight->Create("TitleLight", LightType::Directional, BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	titleLight->Initialize();

	for (int i = 0; i < streetLight.size(); i++)
	{
		streetLight[i] = std::make_unique<StreetLight>();
		streetLight[i]->Create("StreetLight", LightType::Point, BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
		streetLight[i]->Initialize();
		streetLight[i]->SetPosition(titleGround->GetPos(), i);
	}

	// 背景
	backGround = std::make_unique<BackGround>();
	backGround->Create("BackGround", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	backGround->Initialize();

	// プレイヤー
	gameOverPlayer = std::make_unique<GameOverPlayer>();
	gameOverPlayer->Create("GameOverPlayer", BaseScene::GetSceneManager(), BaseScene::GetSceneManager()->GetCompPtr(), input);
	gameOverPlayer->Initialize();
}

void GameOverScene::Finalize()
{
	audio->SoundUnLord(&buttonSE);
	audio->SoundUnLord(&SE);
}

void GameOverScene::Update()
{
	gameOverPlayer->Update();
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

void GameOverScene::Draw()
{
}

void GameOverScene::ChangeScene()
{
	BaseScene::GetSceneManager()->ChangeScene("Title");
}
