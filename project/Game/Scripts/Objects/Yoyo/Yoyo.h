#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
#include "Game/Scripts/Collider/Collider.h"
#include "Game/Scripts/Objects/DelayObject/DelayObject.h"
#include "Game/Scripts/Objects/Boss/Boss.h"

enum class YoyoBehavior {
	kRoot,
	kShot,
	kKeep,
	kReturn,
};

class YoyoRope : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;
};

class Yoyo :
    public BaseObject, public Collider, public DelayObject
{
public:
	// 初期化
	void Initialize() override;

	// ゲームオーバーやリトライ時の使用を想定
	void Reset();

	// 更新
	void Update() override;

	// 前方へ発射
	void FrontShot();
	// 後方へ発射
	void BackShot();

public: // setter / getter
	// 親のトランスフォームをセット
	void SetParent(Matrix4* parent) { transform->parent = parent; }
	// 発射キーを押し続けているか
	void IsPush();

	// プレイヤーの移動量を取得
	void SetPlayerVelocity(const Vector3& velocity = Vector3(0,0,0)) { playerVelo_ = velocity; }

	void SetBoss(Boss* boss) { boss_ = boss; }

	void IsPowerUp() { isPowerUp_ = true; }

	void NoPowerUp() { isPowerUp_ = false; }

private: // GlobalVariables
	// ファイルや要素を追加
	void AddGlobalVariables();
	// 適用
	void ApplyGlobalVariables();


private:
	// 行動初期化
	void BehaviorInitialize();
	// 行動更新
	void BehaviorUpdate();


	// 通常行動
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	// 発射
	void BehaviorShotInitialize();
	void BehaviorShotUpdate();

	// キープ中
	void BehaviorKeepInitialize();
	void BehaviorKeepUpdate();

	// 戻ってくる
	void BehaviorReturnInitialize();
	void BehaviorReturnUpdate();

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override {
		return Vector3{
		transform->matWorld.m[3][0],
		transform->matWorld.m[3][1],
		transform->matWorld.m[3][2]
		};
	}

private:
	// 調整不要
	bool isPushKey = false;				// 発射キーを押しているか
	float directionX = 0;				// 飛び出す向き(Xのみで)
	Vector3 velocity_ = { 0,0,0 };		// ヨーヨーの移動量
	Vector3 playerVelo_ = { 0,0,0 };	// プレイヤーの移動量
	float yoyoLengthX_ = 0;				// ヨーヨーのX軸の距離
	float returnTimer_ = 0.0f;			// 帰還タイマー
	bool isHitEnemy_ = false;			// 敵に当たったか
	bool isGetItem_ = false;			// アイテムを取得したか
	float isPowerUp_ = false;			// パワーアップ中か


	// 調整項目
	float shotSpeed_ = 0.6f;			// 発射速度
	float kShotLength_ = 10.0f;			// 発射距離
	float kMaxLength_ = 20.0f;			// 最長距離
	float kReturnTime_ = 1.0f;			// 帰還時間
	float rotateDegreeZ_ = 0.0f;		// 回転速度


	// 攻撃
	float kAttackPower_ = 1.0f;			// 攻撃力
	float kAttackMultiplier_ = 1.0f;	// 現在の攻撃倍率
	float kPowerUpMultiplier_ = 1.0f;	// アイテム取得時の攻撃倍率

	float backAttackMultiplier_ = 1.0f;
	float kBackAttackMultiplier_ = 1.0f;


	Boss* boss_ = nullptr;				// ボスのポインタ


	// 振る舞い
	YoyoBehavior behavior = YoyoBehavior::kRoot;
	std::optional<YoyoBehavior> behaviorRequest = std::nullopt;
};

