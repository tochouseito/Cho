#include "PrecompiledHeader.h"
#include "Boss.h"
#include "Game/Scripts/GlobalVariables/GlobalVariables.h"
#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Boss::~Boss()
{
	audio->SoundUnLord(&bossDamageSE);
	audio->SoundUnLord(&bossStartSE);
	//BaseObject::~BaseObject();
}

void Boss::Initialize()
{
	SetModel("enemy");
	SetTypeID(TypeID::kEnemy);
	transform->translation = { 1.0f, 1.0f, 0.0f };
	transform->scale = { 0.8f,0.8f,0.8f };
	transform->degrees.y = -90.0f;
	material->enableLighting = 1;
	isAlive_ = true;
	AddGlobalVariables();
	ApplyGlobalVariables();
	HP_ = kMaxHP_;

	bossDamageSE = audio->SoundLordWave("Game/Assets/Sounds/boss_damage.wav");
	bossStartSE = audio->SoundLordWave("Game/Assets/Sounds/boss_start.wav");

	//// EnemyBackHit
	//enemyBackHit = std::make_unique<EnemyBackHit>();
	//enemyBackHit->Create("EnemyBackHit", sceneManager_, compManager_, input);
	//enemyBackHit->Initialize();

	// EnemyHit
	enemyHit = std::make_unique<EnemyHit>();
	enemyHit->Create("EnemyHit", sceneManager_, compManager_, input);
	enemyHit->CreateEffectResource();
	enemyHit->Initialize();
}

void Boss::Update()
{
#ifdef _DEBUG
	// 変更した速度を適用
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	SetDelayTime(globalVar_->GetFloatValue(groupName, "DelayTime"));
	kSpeed = globalVar_->GetFloatValue(groupName, "kSpeed");

#endif // _DEBUG

	velocity_ = {};

	transform->scale = { 0.8f,0.8f,0.8f };

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;

	// 振る舞いの初期化
	BehaviorInitialize();
	// 振る舞いの更新
	BehaviorUpdate();

	// 移動量に速度の値をコピー
	collisionMapInfo.movement = velocity_ * DeltaTime();
	// 移動量を加味して衝突判定する
	MapCollision(collisionMapInfo);


	if(isAlive_)
	{
		Move();
	}
	// デバッグ用ステータス表示
	DebugStutsView(collisionMapInfo);
}

void Boss::Reset()
{
	isAlive_ = true;
	transform->translation = { 1.0f, 1.0f, 0.0f };
	HP_ = kMaxHP_;
	DelayReset();
	ApplyGlobalVariables();
}

void Boss::TakeDamage(float Damage)
{
	if(isAlive_)
	{
		audio->SoundPlayWave(bossDamageSE);
		if (HP_ > 0.0f) {
			HP_ -= Damage;
			transform->scale = { 0.65f, 0.9f, 0.9f };
			behaviorRequest = EnemyBehavior::kKnockback;
		}
		if (HP_ <= 0.0f) {
			isAlive_ = false;
		}
	}
}

void Boss::BossStart()
{
	audio->SoundPlayWave(bossStartSE);
}

void Boss::AddGlobalVariables()
{
#ifdef _DEBUG
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	globalVar_->CreateGroup(groupName);
	globalVar_->AddItem(groupName, "HP", 10.0f);
	globalVar_->AddItem(groupName, "DelayTime", 1.5f);
	globalVar_->AddItem(groupName, "kSpeed", 7.0f);
	globalVar_->AddItem(groupName, "kKnockbackSpeed", 7.0f);
	globalVar_->AddItem(groupName, "kKnockbackTime", 0.2f);
	globalVar_->AddItem(groupName, "healingValue", 1.0f);
	globalVar_->AddItem(groupName, "PowerUpTime", 10.0f);
	globalVar_->AddItem(groupName, "SpeedUpTime", 3.0f);
	globalVar_->AddItem(groupName, "SpeedBoost", 0.0f);


	globalVar_->SaveFile(groupName);
#endif // _DEBUG
}

void Boss::ApplyGlobalVariables()
{
	GlobalVariables* globalVar_ = GlobalVariables::GetInstance();
	const char* groupName = "Boss";
	kMaxHP_ = globalVar_->GetFloatValue(groupName, "HP");
	SetDelayTime(globalVar_->GetFloatValue(groupName, "DelayTime"));
	kSpeed = globalVar_->GetFloatValue(groupName, "kSpeed");
	kKnockbackSpeed_ = globalVar_->GetFloatValue(groupName, "kKnockbackSpeed");
	kKnockbackTIme_ = globalVar_->GetFloatValue(groupName, "kKnockbackTime");
	healingValue_ = globalVar_->GetFloatValue(groupName, "healingValue");
	kPowerUpTime_ = globalVar_->GetFloatValue(groupName, "PowerUpTime");
	kSpeedUpTime_ = globalVar_->GetFloatValue(groupName, "SpeedUpTime");
	speedBoost_ = globalVar_->GetFloatValue(groupName, "SpeedBoost");
}

void Boss::BehaviorInitialize()
{
	if (behaviorRequest) {
		// 振る舞いを変更する
		behavior = behaviorRequest.value();
		// 各振る舞い毎の初期化を実行
		switch (behavior) {
		case EnemyBehavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case EnemyBehavior::kKnockback:
			BehaviorKnockbackInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest = std::nullopt;
	}
}

void Boss::BehaviorUpdate()
{
	// 各振る舞い毎の初期化を実行
	switch (behavior) {
	case EnemyBehavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case EnemyBehavior::kKnockback:
		BehaviorKnockbackUpdate();
		break;
	}
}

void Boss::BehaviorRootInitialize()
{
}

void Boss::BehaviorRootUpdate()
{
	if (attackedPlayer_) {
		float length = playerX_ - transform->translation.x;
		if (length >= 10.0f) { // デフォルトのプレイヤーとエネミーの距離
			attackedPlayer_ = false;
		}
	}

	if (transform->translation.x < playerX_ - 1.0f && !attackedPlayer_) // 仮の処理
	{
		velocity_.x += kSpeed;
		SpeedUp();
	}
}

void Boss::BehaviorKnockbackInitialize()
{
	knockbackTimer_ = 0;
}

void Boss::BehaviorKnockbackUpdate()
{
	if (kKnockbackTIme_ > knockbackTimer_) {
		knockbackTimer_ += DeltaTime();
		velocity_.x -= kKnockbackSpeed_;
	}
	else {
		behaviorRequest = EnemyBehavior::kRoot;
	}
}

void Boss::Move()
{

	transform->translation += velocity_ * DeltaTime();

	// Zを遅らせて追いかけさせる
	if (isAlive_) {
		transform->translation.z = getDelayedPosition().z;
	}
	CheckNowLane();
}

bool Boss::isPowerUp()
{
	if (isPowerUp_) {
		powerUpTimer_ = kPowerUpTime_;
		isPowerUp_ = false;
	}
	if (0 < powerUpTimer_) {
		powerUpTimer_ -= DeltaTime();
		return true;
	}
	return false;
}

void Boss::SpeedUp()
{
	if (isSpeedUp_) {
		speedUpTimer_ = kSpeedUpTime_;
		isSpeedUp_ = false;
	}
	if (0 < speedUpTimer_) {
		speedUpTimer_ -= DeltaTime();
		velocity_.x += speedBoost_;
	}
}

void Boss::DebugStutsView(CollisionMapInfo& info)
{
	(void)info;
#ifdef _DEBUG
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking;
	ImGui::Begin("BossStuts",nullptr,flags);
	ImGui::Text("HP : %.2f / %.2f", HP_, kMaxHP_);
	ImGui::Text("isAlive : %s", isAlive_ ? "true" : "false");
	ImGui::Text("Speed : %.2f, SpeedUp : %s", velocity_.x, 0 < speedUpTimer_ ? "true" : "false");
	ImGui::Text("SpeedTimer : %.2fsec", speedUpTimer_);
	ImGui::Text("PowerUp : %s", 0 < powerUpTimer_ ? "true" : "false");
	ImGui::Text("PowerUpTimer : %.2fsec", powerUpTimer_);
	ImGui::Text("wallCollision : %s", info.wallCollision_ ? "true" : "false");
	ImGui::End();
#endif // _DEBUG
}

void Boss::OnCollision([[maybe_unused]] Collider* other)
{
	if (other->GetTypeID() == TypeID::kYoyo) { // ヨーヨーに当たった時(ダメージ判定はヨーヨー側で呼び出す)
		enemyHit->SetPosition(transform->translation);
		enemyHit->Start();
	}

	if (other->GetTypeID() == TypeID::kHP) { // 回復取った時(HP上限があるなら条件を追加&回復量を決める)
		if (kMaxHP_ > HP_)
		{
			HP_ += healingValue_;
			if (kMaxHP_ < HP_) {
				HP_ = kMaxHP_;
			}
		}
	}

	if (other->GetTypeID() == TypeID::kPower) { // 攻撃力アップアイテム
		isPowerUp_ = true;
	}

	if (other->GetTypeID() == TypeID::kSpeed) { // 速度アップアイテム
		isSpeedUp_ = true;
	}

	if (other->GetTypeID() == TypeID::kPlayer && !attackedPlayer_) { // プレイヤーと当たった時
		attackedPlayer_ = true;
	}
}

void Boss::MapCollision(CollisionMapInfo& info)
{
	// 右方向
	RightCollision(info);
	// 左方向
	LeftCollision(info);
}

void Boss::RightCollision(CollisionMapInfo& info)
{
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
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBoard) {
		hit = true;
	}
	// 右下点の判定
	indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBoard) {
		hit = true;
	}
	// ブロックにヒット？
	if (hit) {
		// めり込み先ブロックを設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(info.movement.x, 0, 0) + Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		fieldBlock->HitEnemyToWall(mapChipType, indexSet.xIndex, indexSet.yIndex, nowLane);
		// 右壁に当たったことを記録する
		info.wallCollision_ = true;
	}
}

void Boss::LeftCollision(CollisionMapInfo& info)
{
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
	if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBoard) {
		hit = true;
	}
	// 左下点の判定
	if(!hit)
	{
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		mapChipType = fieldBlock->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex, nowLane);
		if (mapChipType == MapChipType::kBlock || mapChipType == MapChipType::kBoard) {
			hit = true;
		}
	}
	// ブロックにヒット？
	if (hit) {
		// めり込み先ブロックを設定する
		indexSet = fieldBlock->GetMapChipIndexSetByPosition(transform->translation + Vector3(info.movement.x, 0, 0) - Vector3(kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲矩形
		fieldBlock->HitEnemyToWall(mapChipType, indexSet.xIndex, indexSet.yIndex, nowLane);
		// 左壁に当たったことを記録する
		info.wallCollision_ = true;
	}
}

Vector3 Boss::CornerPosition(const Vector3& center, Corner corner) const
{
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom
		{-kWidth / 2.0f, -kHeight / 2.0f, 0},//kLeftBottom
		{+kWidth / 2.0f, +kHeight / 2.0f, 0},//kRightTop
		{-kWidth / 2.0f, +kHeight / 2.0f, 0},//kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Boss::CheckNowLane()
{
	if (transform->translation.z > 1.0f) {
		nowLane = Lane::kLeftLane;
	}
	else if (transform->translation.z < -1.0f) {
		nowLane = Lane::kRightLane;
	}
	else {
		nowLane = Lane::kCenterLane;
	}
}
