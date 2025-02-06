#include "PrecompiledHeader.h"
#include "GamePlayScene.h"
#include"Scene/SceneManager/SceneManager.h"
#include "Game/Scripts/GlobalVariables/GlobalVariables.h"

void GamePlayScene::Initialize()
{
	SceneManager* sceneManager = GetSceneManager();
	ComponentManager* componentManager = sceneManager->GetCompPtr();
	input = sceneManager->GetInputManagerPtr();
	audio = sceneManager->GetAudioManagerPtr();

	GlobalVariables::GetInstance()->LoadFiles();

	colliderManager = std::make_unique<CollisionManager>();

	// プレイヤーキャラクター
	player = std::make_unique<PlayerCharacter>();
	player->Create("player", sceneManager, componentManager, input);
	player->SetColliderManager(colliderManager.get());
	player->Initialize();
	colliderManager->AddCollider(player.get());
	player->AddColliderYoyo();

	// ボス
	boss = std::make_unique<Boss>();
	boss->Create("boss", sceneManager, componentManager, input);
	boss->Initialize();
	colliderManager->AddCollider(boss.get());

	player->SetBoss(boss.get());


	// メインカメラ
	mainCamera = std::make_unique<MainCamera>();
	mainCamera->Create("mainCamera", sceneManager, componentManager, input);
	mainCamera->Initialize();
	AddGlobalVariables();
	ApplyGlobalVariables();

	// アイテム
	items = std::make_shared<std::list<std::unique_ptr<BaseItem>>>();

	// フィールドブロック
	fieldBlock = std::make_unique<FieldBlock>();
	fieldBlock->SetPtr(sceneManager, componentManager, input);
	fieldBlock->SetItems(items);
	fieldBlock->Initialize();

	// PlayerにFieldBlockのポインタを渡す
	player->SetFieldBlock(fieldBlock.get());
	player->SetItems(items);

	boss->SetFieldBlock(fieldBlock.get());

	// フィールド地面
	fieldGround = std::make_unique<FieldGround>();
	fieldGround->SetPtr(sceneManager, componentManager, input);
	fieldGround->SetPlayer(player.get());
	fieldGround->SetFieldBlock(fieldBlock.get());
	fieldGround->Initialize();
	for (auto&& item : *items) {
		colliderManager->AddCollider(item.get());
	}

	// 背景
	backGround = std::make_unique<BackGround>();
	//backGround->GetGameObject("BackGround", sceneManager, componentManager, input);
	backGround->Create("BackGround", sceneManager, componentManager, input);
	backGround->Initialize();

	// ライト
	mainLight = std::make_unique<MainLight>();
	mainLight->Create("mainLight", LightType::Directional, sceneManager, componentManager, input);
	mainLight->Initialize();

	explosionSE = audio->SoundLordWave("Game/Assets/Sounds/explosion.wav");

	ruleSE = audio->SoundLordWave("Game/Assets/Sounds/rule.wav");
	buttonSE = audio->SoundLordWave("Game/Assets/Sounds/botan.wav");

	// Fade
	fade = std::make_unique<Fade>();
	fade->Create("FadeSprite", sceneManager, componentManager, input);
	fade->Initialize();
	fade->Start(Fade::Status::FadeIn, 1.0f);

	// UI
	for (int32_t index = 0; index < player->GetMaxHP(); ++index) {
		std::unique_ptr<PlayerHP> pHP = std::make_unique<PlayerHP>();
		pHP->Create("playerHP", sceneManager, componentManager, input);
		pHP->Initialize();
		pHP->offset(index);
		playerHP.push_back(std::move(pHP));
	}

	bossHPBackSpr = std::make_unique<BossHPBack>();
	bossHPBackSpr->Create("BossHPBack", sceneManager, componentManager, input);
	bossHPBackSpr->Initialize();

	bossHPSpr = std::make_unique<BossHP>();
	bossHPSpr->Create("BossHP", sceneManager, componentManager, input);
	bossHPSpr->Initialize();
	bossHPSpr->SetMaxHP(boss->GetMaxHP());
	bossHPSpr->SetCurrentHP(boss->GetHP());

	bossHPFrameSpr = std::make_unique<BossHPFrame>();
	bossHPFrameSpr->Create("BossHPFrame", sceneManager, componentManager, input);
	bossHPFrameSpr->Initialize();



	ruleSpr = std::make_unique<ruleSprite>();
	ruleSpr->Create("RuleSprite", sceneManager, componentManager, input);
	ruleSpr->Initialize();
	ruleSpr->SetIsRender(true);

	ruleJumpSpr = std::make_unique<ruleJump>();
	ruleJumpSpr->Create("ruleJump", sceneManager, componentManager, input);
	ruleJumpSpr->Initialize();
	ruleJumpSpr->SetIsRender(true);

	ruleMoveSpr = std::make_unique<ruleMove>();
	ruleMoveSpr->Create("ruleMove", sceneManager, componentManager, input);
	ruleMoveSpr->Initialize();
	ruleMoveSpr->SetIsRender(true);

	ruleShotSpr = std::make_unique<ruleShot>();
	ruleShotSpr->Create("ruleShot", sceneManager, componentManager, input);
	ruleShotSpr->Initialize();
	ruleShotSpr->SetIsRender(false);

	ruleHoldSpr = std::make_unique<ruleHold>();
	ruleHoldSpr->Create("ruleHold", sceneManager, componentManager, input);
	ruleHoldSpr->Initialize();
	ruleHoldSpr->SetIsRender(false);

	ruleEliminateSpr = std::make_unique<ruleEliminate>();
	ruleEliminateSpr->Create("ruleEliminate", sceneManager, componentManager, input);
	ruleEliminateSpr->Initialize();
	ruleEliminateSpr->SetIsRender(false);



	// BGM
	bgmData = audio->SoundLordWave("Game/Assets/Sounds/BGM.wav");
	audio->SoundPlayWave(bgmData, true);

	audio->SoundPlayWave(ruleSE);

	
}

void GamePlayScene::Finalize()
{
	audio->SoundUnLord(&explosionSE);
	audio->SoundUnLord(&ruleSE);
	audio->SoundUnLord(&buttonSE);
	audio->SoundUnLord(&bgmData);
}

void GamePlayScene::Update()
{
	fade->Update();
	GlobalVariables::GetInstance()->Update();
#ifdef _DEBUG
	// 変更した値を適用
	ApplyGlobalVariables();
#endif // _DEBUG
	if (isTutorial)
	{
		if (input->TriggerKey(DIK_SPACE) || input->IsTriggerPadButton(PadButton::A))
		{
			audio->SoundPlayWave(buttonSE);
			if (ruleJumpSpr->GetIsRender())
			{
				audio->SoundPlayWave(ruleSE);
				ruleJumpSpr->SetIsRender(false);
				ruleMoveSpr->SetIsRender(false);
				ruleShotSpr->SetIsRender(true);
				ruleHoldSpr->SetIsRender(true);
			}
			else if (ruleShotSpr->GetIsRender())
			{
				audio->SoundPlayWave(ruleSE);
				ruleShotSpr->SetIsRender(false);
				ruleHoldSpr->SetIsRender(false);
				ruleEliminateSpr->SetIsRender(true);
			}
			else if (ruleEliminateSpr->GetIsRender())
			{
				ruleEliminateSpr->SetIsRender(false);
				ruleSpr->SetIsRender(false);
				boss->BossStart();
				isTutorial = false;
			}
		}
	}
	else
	{

		GameFinished();

		for (const auto& item : *items) 
		{
			item->Update();
		}

		player->Update();
		backGround->SetPositionX(player->GetPos().x);
		// Zを遅らせて追いかけさせる
		boss->DelayUpdate(Vector3(0.0f, 0.0f, player->GetPos().z));
		boss->SetPlayerX(player->GetPos().x);
		boss->Update();
		if (!isGameEnd) {
			if (!player->GetIsAlive() || !boss->GetIsAlive()) {
				audio->SoundPlayWave(explosionSE);
				isGameEnd = true;
			}
		}
		fieldBlock->Update();
		fieldGround->Update();

		colliderManager->CheckAllCollisions();

		UIUpdate();

		// X座標だけ追尾する
		Vector3 playerPos = { player->GetPos().x, 0, 0 };
		mainCamera->FollowCamera(&playerPos);
		mainCamera->Update();

#ifdef _DEBUG
		// デバッグ用にRでリセット
		if (input->TriggerKey(DIK_R)) {
			player->Reset();
			boss->Reset();
			fieldBlock->ResetBlockScale();
			fieldBlock->ItemReset();
		}
#endif // _DEBUG
	}
}

void GamePlayScene::Draw()
{
}

void GamePlayScene::ChangeScene()
{

}

void GamePlayScene::AddGlobalVariables()
{
#ifdef _DEBUG
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Camera";
	globalVar_->CreateGroup(groupName);
	globalVar_->AddItem(groupName, "offsetPos", Vector3(0.0f, 4.0f, -30.0f));
	globalVar_->AddItem(groupName, "offsetDegrees", Vector3(0.0f, 0.0f, 0.0f));
	globalVar_->AddItem(groupName, "ShakeRange", Vector3(0.5f, 0.0f, 0.5f));
	globalVar_->AddItem(groupName, "ShakeTime", 0.3f);


	globalVar_->SaveFile(groupName);
#endif // _DEBUG
}

void GamePlayScene::ApplyGlobalVariables()
{
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Camera";
	mainCamera->SetOffsetPos(globalVar_->GetVector3Value(groupName, "offsetPos"));
	mainCamera->SetOffsetDegrees(globalVar_->GetVector3Value(groupName, "offsetDegrees"));
}

void GamePlayScene::ResetObjectPosition()
{
	// プレイヤーの位置の相対座標で戻す

}

void GamePlayScene::UIUpdate()
{
	if (player->GetIsTakeDamage()) {
		if (player->GetHP() >= 0) {
			for (auto it = playerHP.rbegin(); it != playerHP.rend(); ++it) {
				if ((*it)->GetIsRender()) {
					(*it)->SetIsRender(false);
					break;
				}
			}
		}
	}
	if (player->GetIsHeal()) {
		if (player->GetMaxHP() >= player->GetHP()) {
			for (auto it = playerHP.begin(); it != playerHP.end(); ++it) {
				if (!(*it)->GetIsRender()) {
					(*it)->SetIsRender(true);
					break;
				}
			}
		}
	}
	bossHPSpr->SetCurrentHP(boss->GetHP());
	bossHPSpr->Update();
}

void GamePlayScene::GameFinished()
{
	if (!isGameEnd)
	{
		if (!player->GetIsAlive() || !boss->GetIsAlive())
		{
			audio->SoundPlayWave(explosionSE);
			isGameEnd = true;
			fade->Start(Fade::Status::FadeOut, 1.0f);
		}
	}

	if (isGameEnd) {
		if (fade->IsFinished()) {
			if (fade->GetStatus() == Fade::Status::FadeOut) {
				if (!boss->GetIsAlive())		// クリア
				{
					BaseScene::GetSceneManager()->ChangeScene("Result");
				}
				else if (!player->GetIsAlive()) // ゲームオーバー
				{
					BaseScene::GetSceneManager()->ChangeScene("GameOver");
				}
			}
		}
	}
}
