#pragma once
#include "Scene/BaseScene/BaseScene.h"

// C++
#include <memory>

// Objects
#include"Game/Scripts/Objects/PlayerCharacter/PlayerCharacter.h"
#include"Game/Scripts/Objects/FieldBlock/FieldBlock.h"
#include"Game/Scripts/Objects/FieldGround/FieldGround.h"
#include "Game/Scripts/Objects/Boss/Boss.h"
#include "Game/Scripts/Objects/BackGround/BackGround.h"

// Item
#include "Game/Scripts/Objects/Item/BaseItem.h"
#include "Game/Scripts/Objects/Item/HealingItem.h"

// Camera
#include"Game/Scripts/Camera/MainCamera/MainCamera.h"

// Sprite
#include"Game/Scripts/Sprite/PlayerHP/PlayerHP.h"

// Light
#include "Game/Scripts/Light/MainLight/MainLight.h"

#include "Game/Scripts/ColliderManager/ColliderManager.h"

// AudioManager
#include "Audio/AudioManager.h"

// Sprite
#include "Game/Scripts/Sprite/RuleSprite/ruleSprite.h"
#include "Game/Scripts/Sprite/RuleSprite/rule2.h"
#include "Game/Scripts/Sprite/RuleSprite/ruleMove.h"
#include "Game/Scripts/Sprite/RuleSprite/ruleShot.h"
#include "Game/Scripts/Sprite/RuleSprite/ruleHold.h"
#include "Game/Scripts/Sprite/RuleSprite/ruleEliminate.h"
#include "Game/Scripts/Sprite/BossHP/BossHP.h"
#include "Game/Scripts/Sprite/BossHP/BossHPBack.h"
#include "Game/Scripts/Sprite/BossHP/BossHPFrame.h"

// Fade
#include "Game/Scripts/SceneTransition/Fade/Fade.h"

// Effect


class InputManager;
class AudioManager;

class GamePlayScene :
    public BaseScene
{
    // BaseScene を介して継承されました
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void ChangeScene() override;

private: // GlobalVariables
	// ファイルや要素を追加
	void AddGlobalVariables();
	// 適用
	void ApplyGlobalVariables();

private:

	// オブジェクトの位置を原点に戻す
	void ResetObjectPosition();


	// UIの更新
	void UIUpdate();

	// ゲームプレイ終了
	void GameFinished();
	
private:
	InputManager* input = nullptr;
	AudioManager* audio = nullptr;

	// プレイヤーキャラクター
	std::unique_ptr<PlayerCharacter> player = nullptr;

	// ボス
	std::unique_ptr<Boss> boss = nullptr;

	// メインカメラ
	std::unique_ptr<MainCamera> mainCamera = nullptr;

	// フィールドブロック
	std::unique_ptr<FieldBlock> fieldBlock = nullptr;

    // フィールド地面
	std::unique_ptr<FieldGround> fieldGround = nullptr;

	// 背景
	std::unique_ptr<BackGround> backGround = nullptr;

	// アイテム
	std::shared_ptr<std::list<std::unique_ptr<BaseItem>>> items;

	// UI
	std::list<std::unique_ptr<PlayerHP>> playerHP;
	
	// ライト
	std::unique_ptr<MainLight> mainLight = nullptr;

	std::unique_ptr<CollisionManager> colliderManager = nullptr;

	bool isGameEnd = false;

	bool isTutorial = true;

	// Fade
	std::unique_ptr<Fade> fade = nullptr;
	// 現在のフェーズ
	Phase phase = Phase::kFadeIn;

	// SE
	AudioManager::SoundData explosionSE;
	AudioManager::SoundData ruleSE;
	AudioManager::SoundData buttonSE;

	// BGM
	AudioManager::SoundData bgmData;

	std::unique_ptr<ruleSprite> ruleSpr = nullptr;
	std::unique_ptr<ruleJump> ruleJumpSpr = nullptr;
	std::unique_ptr<ruleMove> ruleMoveSpr = nullptr;
	std::unique_ptr<ruleShot> ruleShotSpr = nullptr;
	std::unique_ptr<ruleHold> ruleHoldSpr = nullptr;
	std::unique_ptr<ruleEliminate> ruleEliminateSpr = nullptr;

	std::unique_ptr<BossHP> bossHPSpr = nullptr;
	std::unique_ptr<BossHPBack> bossHPBackSpr = nullptr;
	std::unique_ptr<BossHPFrame> bossHPFrameSpr = nullptr;

	// エフェクト
	
};

