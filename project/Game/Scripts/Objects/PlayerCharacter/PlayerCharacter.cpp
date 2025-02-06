#include "PrecompiledHeader.h"
#include "PlayerCharacter.h"
//#include "Game/Scripts/Objects/FieldBlock/FieldBlock.h"
#include "Game/Scripts/GlobalVariables/GlobalVariables.h"
#include "Game/Scripts/Camera/CameraShake/CameraShake.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG


PlayerCharacter::~PlayerCharacter()
{
	audio->SoundUnLord(&playerDamageSE);
	audio->SoundUnLord(&jumpSE);
	audio->SoundUnLord(&yoyoSE);
}

void PlayerCharacter::Initialize()
{
	SetModel("player_animation");
	transform->translation = { 15.0f, 1.0f, 0.0f };
	material->enableLighting = 1;
	transform->scale = { 1.2f, 1.2f, 1.2f };
	SetTypeID(TypeID::kPlayer);
	AddGlobalVariables();
	ApplyGlobalVariables();
	HP_ = kMaxHP_;

	// Yoyoの生成
	yoyo = std::make_unique<Yoyo>();
	yoyo->Create("yoyo", sceneManager_, compManager_, input);
	yoyo->Initialize();
	yoyo->SetParent(&transform->matWorld);

	playerDamageSE = audio->SoundLordWave("Game/Assets/Sounds/player_damage.wav");
	jumpSE = audio->SoundLordWave("Game/Assets/Sounds/jump.wav");
	yoyoSE = audio->SoundLordWave("Game/Assets/Sounds/yoyo.wav");

	//// HoldYoyo
	//holdYoyo = std::make_unique<HoldYoyo>();
	//holdYoyo->Create("HoldYoyo", sceneManager_, compManager_, input);
	//holdYoyo->Initialize();

	// PlayerHit
	playerHit = std::make_unique<PlayerHit>();
	playerHit->Create("PlayerHit", sceneManager_, compManager_, input);
	playerHit->CreateEffectResource();
	playerHit->Initialize();

	// PlayerLandEffect
	playerLandEffect = std::make_unique<PlayerLandEffect>();
	playerLandEffect->Create("PlayerLandEffect", sceneManager_, compManager_, input);
	playerLandEffect->CreateEffectResource();
	playerLandEffect->Initialize();
}

void PlayerCharacter::Update()
{
#ifdef _DEBUG
	// 変更した速度を適用
	ApplyGlobalVariables();
#endif // _DEBUG

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;

	// 
	static float areaOffset = 1.0f;
	if (transform->translation.x>=(areaOffset*fieldBlock->GetNumBlockHorizontal()))
	{
		fieldBlock->ChangeMapArea();
		std::list<std::unique_ptr<BaseItem>> itemList = fieldBlock->GenerateNewAreaItems();
		for (auto&& item : itemList) {
			colliderManager_->AddCollider(item.get());
		}
		items_->splice(items_->end(), std::move(itemList));
		areaOffset += 1.0f;
	}

	if (!isAlive) {
		velocity.Initialize();
	}
	else {
		// 振る舞いの初期化
		BehaviorInitialize();
		// 振る舞いの更新
		BehaviorUpdate();
	}

	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity * DeltaTime();
	// 移動量を加味して衝突判定する
	MapCollision(collisionMapInfo);
	// 判定結果を反映して移動させる
	HitMove(collisionMapInfo);
	// 天井に接触している場合の処理
	HitCeiling(collisionMapInfo);
	// 壁に接触している場合の処理
	HitWall(collisionMapInfo);
	// 設置状態の切り替え
	OnGround(collisionMapInfo);

	// アニメーション更新
	AnimationUpdate();

	// デバッグ用ステータス表示
	DebugStutsView();

	// 座標の更新
	transform->translation += velocity * DeltaTime();
	yoyo->SetPlayerVelocity(velocity);
	yoyo->Update();

}

void PlayerCharacter::Reset()
{
	// 初期位置に戻す
	transform->translation = { 15.0f, 1.0f, 0.0f };

	velocity = { 0,0,0 };

	// 初期の残機を適用
	HP_ = kMaxHP_;
	onGround = true;
	isJumping = false;
	landing = false;
	preLand = false;
	isAlive = true;

	nowLane = Lane::kCenterLane;

	// ヨーヨーの初期化
	yoyo->Reset();

	// その他初期値に戻さなきゃいけないもの
	behavior = Behavior::kRoot;
	behaviorRequest = std::nullopt;
	BehaviorRootInitialize();
	BehaviorRootUpdate();
	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;

	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity * DeltaTime();
	// 移動量を加味して衝突判定する
	MapCollision(collisionMapInfo);
	// 判定結果を反映して移動させる
	HitMove(collisionMapInfo);
	// 天井に接触している場合の処理
	HitCeiling(collisionMapInfo);
	// 壁に接触している場合の処理
	HitWall(collisionMapInfo);
	// 設置状態の切り替え
	OnGround(collisionMapInfo);

}

void PlayerCharacter::AddGlobalVariables()
{
#ifdef _DEBUG
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "PlayerCharacter";
	globalVar_->CreateGroup(groupName);
	globalVar_->AddItem(groupName, "HP", 1);
	globalVar_->AddItem(groupName, "kAcceleration", 1.8f);
	globalVar_->AddItem(groupName, "kMaxSpeed", 6.6f);
	globalVar_->AddItem(groupName, "kGravityAcceleration", 0.6f);
	globalVar_->AddItem(groupName, "kJumpAcceleration", 18.0f);
	globalVar_->AddItem(groupName, "PowerUpTime", 20.0f);
	globalVar_->AddItem(groupName, "PowerUpMultiplier", 2.0f);
	globalVar_->AddItem(groupName, "SpeedUpTime", 3.0f);
	globalVar_->AddItem(groupName, "SpeedBoost", 3.0f);


	globalVar_->SaveFile(groupName);
#endif // _DEBUG
}

void PlayerCharacter::ApplyGlobalVariables()
{
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "PlayerCharacter";
	kMaxHP_ = globalVar_->GetIntValue(groupName, "HP");
	kAcceleration = globalVar_->GetFloatValue(groupName, "kAcceleration");
	kMaxSpeed = globalVar_->GetFloatValue(groupName, "kMaxSpeed");
	kGravityAcceleration = globalVar_->GetFloatValue(groupName, "kGravityAcceleration");
	kJumpAcceleration = globalVar_->GetFloatValue(groupName, "kJumpAcceleration");
	kPowerUpTime_ = globalVar_->GetFloatValue(groupName, "PowerUpTime");
	kSpeedUpTime_ = globalVar_->GetFloatValue(groupName, "SpeedUpTime");
	speedBoost_ = globalVar_->GetFloatValue(groupName, "SpeedBoost");
}

void PlayerCharacter::BehaviorInitialize()
{
	if (behaviorRequest) {
		// 振る舞いを変更する
		behavior = behaviorRequest.value();
		// 各振る舞い事の初期化を実行
		switch (behavior) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kJump:
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest = std::nullopt;
	}
}

void PlayerCharacter::BehaviorUpdate()
{
	switch (behavior) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kJump:
		break;
	}
}

void PlayerCharacter::BehaviorRootInitialize()
{

}

void PlayerCharacter::BehaviorRootUpdate()
{
	CharMove();
	LaneSwap();
	if (isHeal_) {
		isHeal_ = false;
	}
	if (isTakeDamage_) {
		isTakeDamage_ = false;
		isBeforeTakeDamage_ = true;
	}
	else {
		isBeforeTakeDamage_ = false;
	}
}

void PlayerCharacter::YoyoOperation()
{
	// キーボード&Pad操作
	if (input->TriggerKey(DIK_D) || input->IsTriggerPadButton(PadButton::RShoulder)) {
		audio->SoundPlayWave(yoyoSE);
		yoyo->FrontShot();
		yoyo->IsPush();
	}
	else if (input->PushKey(DIK_D) || input->IsPressPadButton(PadButton::RShoulder)) {
		yoyo->IsPush();
	}

	if (input->TriggerKey(DIK_A) || input->IsTriggerPadButton(PadButton::LShoulder)) {
		audio->SoundPlayWave(yoyoSE);
		yoyo->BackShot();
		yoyo->IsPush();
	}
	else if (input->PushKey(DIK_A) || input->IsPressPadButton(PadButton::LShoulder)) {
		yoyo->IsPush();
	}

}

void PlayerCharacter::CharMove()
{
	// 自動移動
	Vector3 acceleration = {};
	acceleration.x += kAcceleration;
	// 加速／減速
	velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	velocity.z += acceleration.z;
	// 最大速度制限
	velocity.x = std::clamp(velocity.x, -kMaxSpeed, kMaxSpeed);

	SpeedUp();
	PowerUp();

	YoyoOperation();

	// コントローラー
	XINPUT_STATE joyState;
	bool controllerConnected = input->GetJoystickState(0, joyState);
	if (controllerConnected) {
		if (joyState.Gamepad.sThumbLX) {
			
		}
	}

	// ジャンプ入力
	bool keyboardJump = input->TriggerKey(DIK_SPACE);
	bool controllerJump = controllerConnected && input->IsTriggerPadButton(PadButton::A, 0);
	if (onGround && (keyboardJump || controllerJump)) {
		// ジャンプ初速
		velocity += Vector3(0, kJumpAcceleration, 0);
		isJumping = true;
		audio->SoundPlayWave(jumpSE);
		playerLandEffect->Reset();
	}
}

void PlayerCharacter::PowerUp()
{
	if (isPowerUp_) {
		powerUpTimer_ = kPowerUpTime_;
		isPowerUp_ = false;
	}
	if (0 < powerUpTimer_) {
		powerUpTimer_ -= DeltaTime();
		yoyo->IsPowerUp();
	}
	else {
		yoyo->NoPowerUp();
	}
}

void PlayerCharacter::SpeedUp()
{
	if (isSpeedUp_) {
		speedUpTimer_ = kSpeedUpTime_;
		isSpeedUp_ = false;
	}
	if (0 < speedUpTimer_) {
		speedUpTimer_ -= DeltaTime();
		velocity.x += speedBoost_;
	}
}

void PlayerCharacter::DebugStutsView()
{
#ifdef _DEBUG
	ImGui::Begin("PlayerStuts");
	ImGui::Text("HP : %d / %d", HP_, kMaxHP_);
	ImGui::Text("isAlive : %s", isAlive ? "true" : "false");
	ImGui::Text("Speed : %.2f, SpeedUp : %s", velocity.x, 0 < speedUpTimer_ ? "true" : "false");
	ImGui::Text("SpeedTimer : %.2fsec", speedUpTimer_);
	ImGui::Text("PowerUp : %s", 0 < powerUpTimer_ ? "true" : "false");
	ImGui::Text("PowerUpTimer : %.2fsec", powerUpTimer_);
	ImGui::End();
#endif // _DEBUG

}

void PlayerCharacter::AnimationUpdate()
{
	// アニメーション遷移
	if (nextAnimationRequest) {
		// 次のアニメーションを指定
		nextAnimation = nextAnimationRequest.value();
		nextAnimationRequest = std::nullopt;// リセット
	}
	// 現在のアニメーションを保存
	nowAnimation = static_cast<PlayerAnimation>(animation->animationIndex);
	// 現在のアニメーションと次のアニメーションが異なる場合、アニメーションが一周していたら切り替える
	if (nowAnimation != nextAnimation) {
		if (animation->isEnd || forcedTerminationAnimation) {// 一周していたら切り替える
			animation->animationIndex = static_cast<uint32_t>(nextAnimation);
		}
	}
	// 強制終了のフラグをリセット
	forcedTerminationAnimation = false;
}

void PlayerCharacter::OnCollision([[maybe_unused]] Collider* other)
{
	if(isAlive)
	{
		if (other->GetTypeID() == TypeID::kEnemy || other->GetTypeID() == TypeID::kObstacle) { // 敵や障害物に当たった時
			isTakeDamage_ = true;
			
			if (isBeforeTakeDamage_) {
				return;
			}
			audio->SoundPlayWave(playerDamageSE);
			if (HP_ > 0) {
				CameraShake::GetInstance()->SetShake // カメラシェイク
				(
					GlobalVariables::GetInstance()->GetVector3Value("Camera", "ShakeRange"), 
					GlobalVariables::GetInstance()->GetFloatValue("Camera", "ShakeTime")
				);

				HP_--;
				playerHit->SetPosition(transform->translation);
				playerHit->Start();
				if (boss_->isPowerUp()) { // ボスの攻撃力が上がってるとき追加で残機を削る
					HP_--;
				}
			}

			nextAnimationRequest = PlayerAnimation::kHit;
			forcedTerminationAnimation = true;

			if (HP_ <= 0) {
				isAlive = false;
			}
		}

		if (false) { // 落ちた時
			isAlive = false;
		}

		if (other->GetTypeID() == TypeID::kHP) { // 回復取った時(HP上限があるなら条件を追加)
			if(kMaxHP_ > HP_)
			{
				HP_++;
			}
		}

		if (other->GetTypeID() == TypeID::kPower) { // 攻撃力アップアイテム
			isPowerUp_ = true;
		}

		if (other->GetTypeID() == TypeID::kSpeed) { // 速度アップアイテム
			isSpeedUp_ = true;
		}
	}
}

#pragma region // マップとの判定
void PlayerCharacter::MapCollision(CollisionMapInfo& info)
{
	// 各方向衝突判定
	// 上方向
	TopCollision(info);
	// 下方向
	BottomCollision(info);
	// 右方向
	RightCollision(info);
	// 左方向
	LeftCollision(info);
}

void PlayerCharacter::HitMove(const CollisionMapInfo& info)
{
	//transform->translation += info.movement;
	velocity.x = info.movement.x / DeltaTime();
	velocity.y = info.movement.y / DeltaTime();
	velocity.z = info.movement.z / DeltaTime();
}

void PlayerCharacter::HitCeiling(const CollisionMapInfo& info)
{
	// 天井に当たった
	if (info.ceilingCollision_) {
		velocity.y = 0;
	}
}

void PlayerCharacter::HitWall(const CollisionMapInfo& info)
{
	// 壁接触による減衰
	if (info.wallCollision_) {
		velocity.x *= (1.0f - kAttenuationWall);
		nextAnimationRequest = PlayerAnimation::kStop;
		forcedTerminationAnimation = true;
	}
	else {
		nextAnimationRequest = PlayerAnimation::kRun;
	}
}

void PlayerCharacter::OnGround(const CollisionMapInfo& info)
{
	// 着地フラグ
	if (info.landingCollision_) {
		// 着地状態に切り替える（落下を止める）
		onGround = true;
		isJumping = false; // ジャンプ状態を解除
		// 着地時にX速度を減衰
		velocity.x *= (1.0f - kAttenuationLanding);
		// Y速度をゼロにする
		velocity.y = 0.0f;
		playerLandEffect->SetPosition(transform->translation);
		playerLandEffect->Start();
	}

	// ジャンプ開始
	if (velocity.y > 0.0f) {
		// 空中状態に以降
		onGround = false;
	}

	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transform->translation + info.movement, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;

	// 通常重力
		// 左下点の判定
	FieldBlock::IndexSet indexSet;
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kCorrectBlank, 0));
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// 右下点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kCorrectBlank, 0));
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}


	// 落下開始
	if (!hit) {
		// 空中状態に切り替える
		onGround = false;
	}

	if (!onGround) {// 空中状態
		// 落下速度
		velocity.x += 0.0f;
		velocity.y += -(kGravityAcceleration * DeltaTime());
		velocity.z += 0.0f;
		// 落下速度制御
		velocity.y = std::max(velocity.y, -kLimitFallSpeed);

		// 着地フラグ
		landing = false;

		// 地面との当たり判定
		// 下降中？
		if (velocity.y < 0) {
			// Y座標が地面以下になったら着地
			if (transform->translation.y <= 1.0f) {
				landing = true;
			}
		}
		if (landing) {
			// めり込み排斥
			transform->translation.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity.x *= (1.0f - kAttenuationLanding);
			// 下方向の速度をリセット
			velocity.y = 0.0f;
			// 設置状態に移行
			onGround = true;
		}
	}
	preLand = onGround;
}

void PlayerCharacter::TopCollision(CollisionMapInfo& info)
{
	// 上昇アリ？
	if (info.movement.y <= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transform->translation + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	FieldBlock::IndexSet indexSet;
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// 右上点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(0, info.movement.y, 0) + Vector3(0, kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		FieldBlock::Rect rect = fieldBlock->GetRectByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
		info.movement.y = std::max(0.0f, (rect.bottom - transform->translation.y) - ((kHeight / 2.0f) + kBlank));
		// 天井に当たったことを記録する
		info.ceilingCollision_ = true;

		info.landingCollision_ = true;
	}
}

void PlayerCharacter::BottomCollision(CollisionMapInfo& info)
{
	// 下降アリ？
	if (info.movement.y >= 0) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transform->translation + Vector3(0, info.movement.y, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	FieldBlock::IndexSet indexSet;
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// 右下点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(0, info.movement.y, 0) - Vector3(0, kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		FieldBlock::Rect rect = fieldBlock->GetRectByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
		info.movement.y = std::min(0.0f, (rect.top - transform->translation.y) + ((kHeight / 2.0f) + kBlank));
		// 地面に当たったことを記録する
		info.landingCollision_ = true;
	}
}

void PlayerCharacter::RightCollision(CollisionMapInfo& info)
{
	// 右移動あり？
	if (info.movement.x <= 0.0f) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transform->translation + info.movement, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 右の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	FieldBlock::IndexSet indexSet;
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// 右下点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(info.movement.x, 0, 0) + Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		FieldBlock::Rect rect = fieldBlock->GetRectByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
		info.movement.x = std::max(0.0f, (rect.left - transform->translation.x) - ((kWidth / 2.0f) + kBlank));
		// 右壁に当たったことを記録する
		info.wallCollision_ = true;
	}
}

void PlayerCharacter::LeftCollision(CollisionMapInfo& info)
{
	// 左移動あり？
	if (info.movement.x >= 0.0f) {
		return;
	}
	// 移動後４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(transform->translation + Vector3(info.movement.x, 0, 0), static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 左の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	FieldBlock::IndexSet indexSet;
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// 左下点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock||mapChipType==MapChipType::kBoard) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(info.movement.x, 0, 0) - Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		FieldBlock::Rect rect = fieldBlock->GetRectByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
		info.movement.x = std::min(0.0f, (rect.right - transform->translation.x) + ((kWidth / 2.0f) + kBlank));
		// 地面に当たったことを記録する
		info.wallCollision_ = true;
	}
}

Vector3 PlayerCharacter::CornerPosition(const Vector3& center, Corner corner) const
{
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom
		{-kWidth / 2.0f, -kHeight / 2.0f, 0},//kLeftBottom
		{+kWidth / 2.0f, +kHeight / 2.0f, 0},//kRightTop
		{-kWidth / 2.0f, +kHeight / 2.0f, 0},//kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
#pragma endregion

void PlayerCharacter::LaneSwap()
{
	velocity.z = 0;
	if(!isMoveLeft_ && !isMoveRight_ && onGround)
	{
		if (input->TriggerKey(DIK_Q) || input->GetStickValue(LR::LEFT).x > 0) {
			if (transform->translation.z < laneMoveLength_ / 2.0f)
			{
				isMoveLeft_ = true;
				// ジャンプ初速
				velocity += Vector3(0, kJumpAcceleration, 0);
				isJumping = true;
				audio->SoundPlayWave(jumpSE);
			}
		}

		if (input->TriggerKey(DIK_E) || input->GetStickValue(LR::LEFT).x < 0) {
			if (transform->translation.z > -laneMoveLength_ / 2.0f)
			{
				isMoveRight_ = true;
				// ジャンプ初速
				velocity += Vector3(0, kJumpAcceleration, 0);
				isJumping = true;
				audio->SoundPlayWave(jumpSE);
			}
		}
	}
	MoveLeft();
	MoveRight();
}

void PlayerCharacter::MoveLeft()
{
	if (isMoveLeft_) {
		if (kSwapTime_ > swapTimer_) {

			velocity.z += laneMoveLength_ / kSwapTime_;
			if (swapTimer_ > (kSwapTime_ / 2.0f) && !isLaneSwap_) {
				if (nowLane == Lane::kCenterLane) {
					nowLane = Lane::kLeftLane;
				}
				else {
					nowLane = Lane::kCenterLane;
				}
				isLaneSwap_ = true;
			}

			swapTimer_ += DeltaTime();
		}
		else {
			isMoveLeft_ = false;
			isLaneSwap_ = false;
			swapTimer_ = 0;
		}
	}
}

void PlayerCharacter::MoveRight()
{
	if (isMoveRight_) {
		if (kSwapTime_ > swapTimer_) {

			velocity.z -= laneMoveLength_ / kSwapTime_;
			if (swapTimer_ > (kSwapTime_ / 2.0f) && !isLaneSwap_) {
				if (nowLane == Lane::kCenterLane) {
					nowLane = Lane::kRightLane;
				}
				else {
					nowLane = Lane::kCenterLane;
				}
				isLaneSwap_ = true;
			}

			swapTimer_ += DeltaTime();
		}
		else {
			isMoveRight_ = false;
			isLaneSwap_ = false;
			swapTimer_ = 0;
		}
	}
}
