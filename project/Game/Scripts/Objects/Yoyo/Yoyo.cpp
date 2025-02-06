#include "PrecompiledHeader.h"
#include "Yoyo.h"
#include "Game/Scripts/GlobalVariables/GlobalVariables.h"
#include "Game/Scripts/Objects/Boss/Boss.h"

void Yoyo::Initialize()
{
	SetModel("yoyo");
	SetTypeID(TypeID::kYoyo);
	SetRadius(1.0f);
	transform->scale = { 0,0,0 };
	behavior = YoyoBehavior::kRoot;
	behaviorRequest = std::nullopt;
	AddGlobalVariables();
	ApplyGlobalVariables();
}

void Yoyo::Reset()
{
	// フラグをリセット
	isPushKey = false;

	returnTimer_ = 0;
	yoyoLengthX_ = 0;

	// 通常行動に戻す
	behavior = YoyoBehavior::kRoot;
	behaviorRequest = std::nullopt;
	BehaviorRootInitialize();
}

void Yoyo::Update()
{
#ifdef _DEBUG
	// 変更した値を適用
	ApplyGlobalVariables();
#endif // _DEBUG

	// 速度の初期化
	velocity_ = { 0,0,0 };


	// 振る舞いの初期化
	BehaviorInitialize();
	// 振る舞いの更新
	BehaviorUpdate();

	if(behavior != YoyoBehavior::kRoot){
		velocity_.y -= playerVelo_.y;
		velocity_.z -= playerVelo_.z;
		DelayUpdate(Vector3(0.0f, playerVelo_.y, playerVelo_.z));
		transform->degrees.z += rotateDegreeZ_ * DeltaTime();
	}
	else {
		DelayUpdate(Vector3(0.0f, 0.0f, 0.0f));
		transform->degrees.z = 0;
	}

	// 座標の更新
	transform->translation += (velocity_ + getDelayedPosition()) * DeltaTime();
}

void Yoyo::FrontShot()
{
	// 通常行動時からのみ発射可能
	if(behavior == YoyoBehavior::kRoot)
	{
		directionX = 1.0f;
		behaviorRequest = YoyoBehavior::kShot;
	}
}

void Yoyo::BackShot()
{
	// 通常行動時からのみ発射可能
	if (behavior == YoyoBehavior::kRoot)
	{
		directionX = -1.0f;
		behaviorRequest = YoyoBehavior::kShot;
	}
}

void Yoyo::IsPush()
{
	if (behavior == YoyoBehavior::kRoot || behavior == YoyoBehavior::kShot || behavior == YoyoBehavior::kKeep) {
		isPushKey = true;
	}
}

void Yoyo::AddGlobalVariables()
{
#ifdef _DEBUG
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Yoyo";
	globalVar_->CreateGroup(groupName);
	globalVar_->AddItem(groupName, "ShotSpeed", 0.6f);
	globalVar_->AddItem(groupName, "ShotLength", 10.0f);
	globalVar_->AddItem(groupName, "ShotLengthMax", 20.0f);
	globalVar_->AddItem(groupName, "ReturnTime", 1.0f);
	globalVar_->AddItem(groupName, "DelayTime", 0.3f);
	globalVar_->AddItem(groupName, "rotateDegreeZ", 360.0f);


	globalVar_->SaveFile(groupName);
#endif // _DEBUG
}

void Yoyo::ApplyGlobalVariables()
{
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Yoyo";
	shotSpeed_ = globalVar_->GetFloatValue(groupName, "ShotSpeed");
	kShotLength_ = globalVar_->GetFloatValue(groupName, "ShotLength");
	kMaxLength_ = globalVar_->GetFloatValue(groupName, "ShotLengthMax");
	kReturnTime_ = globalVar_->GetFloatValue(groupName, "ReturnTime");
	SetDelayTime(globalVar_->GetFloatValue(groupName, "DelayTime"));
	rotateDegreeZ_ = globalVar_->GetFloatValue(groupName, "rotateDegreeZ");
	kPowerUpMultiplier_ = globalVar_->GetFloatValue("PlayerCharacter", "PowerUpMultiplier");
}

void Yoyo::BehaviorInitialize()
{
	if (behaviorRequest) {
		// 振る舞いを変更する
		behavior = behaviorRequest.value();
		// 各振る舞い毎の初期化を実行
		switch (behavior) {
		case YoyoBehavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case YoyoBehavior::kShot:
			BehaviorShotInitialize();
			break;
		case YoyoBehavior::kKeep:
			BehaviorKeepInitialize();
			break;
		case YoyoBehavior::kReturn:
			BehaviorReturnInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest = std::nullopt;
	}
}

void Yoyo::BehaviorUpdate()
{
	// 各振る舞い毎の更新
	switch (behavior) {
	case YoyoBehavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case YoyoBehavior::kShot:
		BehaviorShotUpdate();
		break;
	case YoyoBehavior::kKeep:
		BehaviorKeepUpdate();
		break;
	case YoyoBehavior::kReturn:
		BehaviorReturnUpdate();
		break;
	}
}

void Yoyo::BehaviorRootInitialize()
{
	// プレイヤーと同じ位置に戻す
	transform->translation = { 0,0,0 };
	transform->scale = { 0,0,0 };
	directionX = 0;
	isHitEnemy_ = false;
	DelayReset();
}

void Yoyo::BehaviorRootUpdate()
{
	// 通常状態では何もしない
}

void Yoyo::BehaviorShotInitialize()
{
	transform->scale = { 1,1,1 };
}

void Yoyo::BehaviorShotUpdate()
{
	if (isHitEnemy_) {
		behaviorRequest = YoyoBehavior::kReturn;
	}
	if (transform->translation.x < kShotLength_ && transform->translation.x > -kShotLength_)
	{
		velocity_.x += shotSpeed_ * directionX;
	}
	else
	{
		// 座標を修正
		transform->translation.x = kShotLength_ * directionX;
		behaviorRequest = YoyoBehavior::kKeep;
	}
}

void Yoyo::BehaviorKeepInitialize()
{
	// 座標を修正
	transform->translation.x = kShotLength_ * directionX;
}

void Yoyo::BehaviorKeepUpdate()
{
	if (isHitEnemy_) {
		behaviorRequest = YoyoBehavior::kReturn;
	}
	if (isPushKey) {
		if (transform->translation.x > -kMaxLength_) {
			velocity_.x -= playerVelo_.x * 0.833f; // * 0.833f : プレイヤーのスケールに合わせた応急処置
		}
		else {
			// 座標を修正
			transform->translation.x = -kMaxLength_;
		}
	}
	else {
		behaviorRequest = YoyoBehavior::kReturn;
		yoyoLengthX_ = transform->translation.x;
	}
	isPushKey = false;
}

void Yoyo::BehaviorReturnInitialize()
{
	yoyoLengthX_ = transform->translation.x;
}

void Yoyo::BehaviorReturnUpdate()
{
	// どんな距離でも同じ時間で帰ってくる
	if (kReturnTime_ > returnTimer_) {
		returnTimer_ += DeltaTime();
		float t = (std::min)((returnTimer_ / kReturnTime_), 1.0f);
		transform->translation.x = ((1.0f - t) * yoyoLengthX_);
	}
	else {
		returnTimer_ = 0;
		behaviorRequest = YoyoBehavior::kRoot;
	}
}

void Yoyo::OnCollision([[maybe_unused]] Collider* other)
{
	if(behavior != YoyoBehavior::kRoot)
	{
		// 敵に当たった時
		if (other->GetTypeID() == TypeID::kEnemy && !isHitEnemy_) 
		{ 
			if (boss_->GetCenterPosition().x > GetCenterPosition().x)
			{
				backAttackMultiplier_ = kPowerUpMultiplier_;
			}
			else
			{
				backAttackMultiplier_ = 1.0f;
			}
			if (isPowerUp_) {
				kAttackMultiplier_ = kPowerUpMultiplier_ * backAttackMultiplier_;
			}
			else {
				kAttackMultiplier_ = 1.0f * backAttackMultiplier_;
			}

			if (behavior == YoyoBehavior::kReturn)
			{
				boss_->TakeDamage(kAttackPower_ * (std::sqrtf(yoyoLengthX_ * yoyoLengthX_) / kShotLength_) * kAttackMultiplier_);
			}
			else
			{
				boss_->TakeDamage(kAttackPower_ * kAttackMultiplier_);
			}
			isHitEnemy_ = true;
		}

		if (false) { // 壊せる障害物に当たった時

		}
	}
}

void YoyoRope::Initialize()
{
	SetModel("Cube");
	transform->scale = { 0.0f, 0.2f,0.2f };
}

void YoyoRope::Update()
{
}
