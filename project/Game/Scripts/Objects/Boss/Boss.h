#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
#include "Game/Scripts/Collider/Collider.h"
#include "Game/Scripts/Objects/DelayObject/DelayObject.h"

// C++
#include <optional>
#include <memory>

// FieldBlock
#include"Game/Scripts/Objects/FieldBlock/FieldBlock.h"

#include "Game/Scripts/Effect/EnemyBackHit/EnemyBackHit.h"
#include "Game/Scripts/Effect/EnemyHit/EnemyHit.h"

class FieldBlock;

enum class EnemyBehavior {
    kRoot,      // 通常状態
    kKnockback, // ノックバック
};

class Boss :
    public BaseObject, public Collider, public DelayObject
{
public:
    ~Boss() override;
    // BaseObject を介して継承されました
    void Initialize() override;
    void Update() override;

    // ゲームオーバーやリトライ時の使用を想定
    void Reset();

    void TakeDamage(float Damage);

    void SetPlayerX(float playerPosX) { playerX_ = playerPosX; }

    // ブロック情報のポインタを設定
    void SetFieldBlock(FieldBlock* ptr) { fieldBlock = ptr; }

    bool isPowerUp();

    float GetHP() { return HP_; }
    float  GetMaxHP() { return kMaxHP_; }

    bool GetIsAlive() { return isAlive_; }

    Vector3 GetCenterPosition() const override {
        return Vector3{
        transform->matWorld.m[3][0],
        transform->matWorld.m[3][1],
        transform->matWorld.m[3][2]
        };
    }


    // チュートリアル追加時に変える
    void BossStart();

private: // GlobalVariables
    // ファイルや要素を追加
    void AddGlobalVariables();
    // 適用
    void ApplyGlobalVariables();
private:
    // 通常行動初期化
    void BehaviorInitialize();
    // 通常行動更新
    void BehaviorUpdate();

    // 通常行動
    void BehaviorRootInitialize();
    void BehaviorRootUpdate();

    // ダメージ受けた時のノックバック
    void BehaviorKnockbackInitialize();
    void BehaviorKnockbackUpdate();

    // 動きの処理
    void Move();

    void SpeedUp();

    void DebugStutsView(CollisionMapInfo& info);

    void OnCollision([[maybe_unused]] Collider* other) override;


private:// MapChipCollision
    // 移動量を加味して衝突判定する
    void MapCollision(CollisionMapInfo& info);
    // マップ衝突判定
    // 各方向衝突判定
    // 右方向
    void RightCollision(CollisionMapInfo& info);
    // 左方向
    void LeftCollision(CollisionMapInfo& info);

    Vector3 CornerPosition(const Vector3& center, Corner corner) const;

    void CheckNowLane();


private:
    // ステータス
    float kMaxHP_ = 20.0f;
    float HP_ = 1.0f;

    bool isPowerUp_ = false;
    float kPowerUpTime_ = 10.0f;
    float powerUpTimer_ = 0.0f;

    bool isSpeedUp_ = false;
    float kSpeedUpTime_ = 3.0f;
    float speedUpTimer_ = 0.0f;
    float speedBoost_ = 0.0f;

    float healingValue_ = 1.0f;

    bool isAlive_ = true;

    bool attackedPlayer_ = false;

    float kSpeed = 6.6f;

    Vector3 velocity_ = {};

    float playerX_ = 0;

    // ノックバック
    float kKnockbackSpeed_ = 0.0f;
    float kKnockbackTIme_ = 0.0f;
    float knockbackTimer_ = 0.0f;


    // キャラクターの当たり判定サイズ
    float kWidth = 0.8f;
    float kHeight = 0.8f;
    float kBlank = 0.1f;
    float kCorrectBlank = 0.5f;

    // レーン
    Lane nowLane = Lane::kCenterLane;

    // マップデータ
    FieldBlock* fieldBlock = nullptr;

    // SE
    AudioManager::SoundData bossDamageSE;
    AudioManager::SoundData bossStartSE;


    // 振る舞い
    EnemyBehavior behavior = EnemyBehavior::kRoot;
    std::optional<EnemyBehavior> behaviorRequest = std::nullopt;
    std::unique_ptr<EnemyBackHit> enemyBackHit = nullptr;
    std::unique_ptr<EnemyHit> enemyHit = nullptr;
};

