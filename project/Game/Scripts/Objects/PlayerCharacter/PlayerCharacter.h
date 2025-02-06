#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
#include "Game/Scripts/Collider/Collider.h"
#include "Game/Scripts/Objects/Boss/Boss.h"

// C++
#include <optional>
#include <memory>

// FieldBlock
#include"Game/Scripts/Objects/FieldBlock/FieldBlock.h"

// Yoyo
#include"Game/Scripts/Objects/Yoyo/Yoyo.h"

#include "Game/Scripts/Effect/PlayerHit/PlayerHit.h"
#include "Game/Scripts/Effect/PlayerLandEffect/PlayerLandEffect.h"
#include "Game/Scripts/Effect/HoldYoyo/HoldYoyo.h"

#include "Game/Scripts/ColliderManager/ColliderManager.h"

enum class Behavior {
	kRoot,// 通常状態
	kJump, // ジャンプ
};

enum PlayerAnimation {
	kResult,
	kHit,
	kRun,
	kStop,
};

class PlayerCharacter :
    public BaseObject, public Collider
{
public:
	~PlayerCharacter() override;
    // BaseObject を介して継承されました
    void Initialize() override;
    void Update() override;

	// ゲームオーバーやリトライ時の使用を想定
	void Reset();

	bool IsPosLimit() {
		if (transform->translation.x > kLimitPositionX) {
			return true;
		}
		return false;
	}

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override {
		return Vector3{
		transform->matWorld.m[3][0],
		transform->matWorld.m[3][1],
		transform->matWorld.m[3][2]
		};
	}

public: // setter / getter
	int32_t GetHP() { return HP_; }
	int32_t  GetMaxHP() { return kMaxHP_; }
	const Vector3 GetPos() { return transform->translation; }

	// 現在のレーン番号を取得
	Lane GetLane() { return nowLane; }
	// レーン番号を設定
	void SetLane(Lane lane) { this->nowLane = lane; }

	// ブロック情報のポインタを設定
	void SetFieldBlock(FieldBlock* ptr) { fieldBlock = ptr; }

	void SetBoss(Boss* boss) { 
		boss_ = boss;
		yoyo->SetBoss(boss_);
	}

	void SetColliderManager(CollisionManager* manager) { colliderManager_ = manager; }

	void AddColliderYoyo() { colliderManager_->AddCollider(yoyo.get()); }

	void SetItems(std::shared_ptr<std::list<std::unique_ptr<BaseItem>>> itemList) { items_ = itemList; }

	bool GetIsTakeDamage() { return isTakeDamage_ && !isBeforeTakeDamage_; }

	bool GetIsHeal() { return isHeal_; }

	bool GetIsAlive() { return isAlive; }

	float GetPosX() { return transform->translation.x; }

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

	// ヨーヨーの操作
	void YoyoOperation();

	// キャラクターの移動
    void CharMove();

	void PowerUp();

	void SpeedUp();

	void DebugStutsView();

	void AnimationUpdate();

private:// MapChipCollision
	// 移動量を加味して衝突判定する
	void MapCollision(CollisionMapInfo& info);
	// 判定結果を反映して移動させる
	void HitMove(const CollisionMapInfo& info);
	// 天井に接触している場合の処理
	void HitCeiling(const CollisionMapInfo& info);
	// 壁に接触している場合の処理
	void HitWall(const CollisionMapInfo& info);
	// 設置状態の切り替え
	void OnGround(const CollisionMapInfo& info);
	// マップ衝突判定
	// 各方向衝突判定
	// 上方向
	void TopCollision(CollisionMapInfo& info);
	// 下方向
	void BottomCollision(CollisionMapInfo& info);
	// 右方向
	void RightCollision(CollisionMapInfo& info);
	// 左方向
	void LeftCollision(CollisionMapInfo& info);

	// レーン移動
	void LaneSwap();
	// 左移動
	void MoveLeft();
	// 右移動
	void MoveRight();

	Vector3 CornerPosition(const Vector3& center, Corner corner) const;
private:

    Vector3 velocity = {};

	// マップデータ
	FieldBlock* fieldBlock = nullptr;

	// Yoyo
	std::unique_ptr<Yoyo> yoyo = nullptr;

	Boss* boss_ = nullptr;

	// item
	std::shared_ptr<std::list<std::unique_ptr<BaseItem>>> items_;

	// ステータス
	int32_t HP_ = 1;
	int32_t kMaxHP_ = 5;

	bool isHeal_ = false;

	bool isPowerUp_ = false;
	float kPowerUpTime_ = 20.0f;
	float powerUpTimer_ = 0.0f;

	bool isSpeedUp_ = false;
	float kSpeedUpTime_ = 3.0f;
	float speedUpTimer_ = 0.0f;
	float speedBoost_ = 0.0f;

	bool isTakeDamage_ = false;
	bool isBeforeTakeDamage_ = false;


	bool isMoveLeft_ = false;
	bool isMoveRight_ = false;
	bool isLaneSwap_ = false;
	float swapTimer_ = 0.0f;
	float laneMoveLength_ = 6.0f;				// レーン移動の移動量（1レーンの幅）
	float kSwapTime_ = 0.6f;

	const float kGravity = 6.0f;				// 重力
	const float kJumpPower = 6.0f;				// ジャンプ力
	const float kJumpPowerDecay = 6.0f;			// ジャンプ力減衰
	float kSpeed = 6.0f;						// 速度(調整できるようにconst外した)

	const float kMaxFallSpeed = 6.0f;			// 最大落下速度
	float kMaxSpeed = 6.6f;						// 最大速度
	float kAcceleration = 1.8f;					// 加速度
	float kAttenuation = 0.0f;					// 減衰  //いらないかも
	const float kAttenuationLanding = 0.0f;		// 着地時の速度減衰  //いらないかも
	float kGravityAcceleration = 36.0f;			// 重力加速度（下方向）
	const float kLimitFallSpeed = 60.0f;		// 最大落下速度（下方向）
	const float kAttenuationWall = 36.0f;		// 壁に接触時速度
	float kJumpAcceleration = 18.0f;			// ジャンプ速度（上方向）

	// キャラクターの当たり判定サイズ
	float kWidth = 0.8f;
	float kHeight = 0.8f;
	float kBlank = 0.1f;
	float kCorrectBlank = 0.5f;

	// フラグ
	bool onGround = true;   // 着地しているか
	bool isJumping = false; // ジャンプ中か
	bool landing = false;   // 着地しているか
	bool preLand = false;   // 前フレームの着地状態
	bool isAlive = true;    // 生存しているか

	// ワールド座標の限界位置
	float kLimitPositionX = 150.0f;

	// レーン
	Lane nowLane = Lane::kCenterLane;

	// SE
	AudioManager::SoundData playerDamageSE;
	AudioManager::SoundData jumpSE;
	AudioManager::SoundData yoyoSE;

	// 振る舞い
	Behavior behavior = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest = std::nullopt;

	PlayerAnimation nowAnimation = PlayerAnimation::kRun;// 現在のアニメーション保存用、ゲームスクリプトでは更新しない
	PlayerAnimation nextAnimation = PlayerAnimation::kRun;// 次のアニメーション指定用、ゲームスクリプトではこちらにアニメーションを指定する
	std::optional<PlayerAnimation> nextAnimationRequest = std::nullopt; // 次のアニメーションリクエスト。空なら現在のアニメーションがループする
	bool forcedTerminationAnimation = false;// アニメーションを強制終了するかどうか

	CollisionManager* colliderManager_ = nullptr;

	std::unique_ptr<PlayerHit> playerHit = nullptr;
	std::unique_ptr<PlayerLandEffect> playerLandEffect = nullptr;
	std::unique_ptr<HoldYoyo> holdYoyo = nullptr;
};

