#pragma once

//C++
#include<string>
#include<optional>

#include"ECS/ComponentManager/Components/Components.h"
class SceneManager;
class ComponentManager;
class InputManager;

class Player
{
public:
	enum class Behavior {
		kRoot,// 通常状態
		kAttack,// 攻撃中
		kJump, // ジャンプ
	};

	enum PlayerAnimation {
		kAttack,
		kFall,
		kIdle,
		kJump,
		kLand,
		kRun,
		kWalk,
	};

public:
	void Init(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);

	void Update();

private:

	void Input();

	void Move();

	void Jump();

	void Fall();

	void BehaviorInitialize();

	void BehaviorUpdate();

	// 通常行動更新
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	// 攻撃行動更新
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

	// ジャンプ行動初期化
	void BehaviorJumpInitialize();
	// ジャンプ行動更新
	void BehaviorJumpUpdate();

	void AnimationUpdate();

public:
	const std::string name = "Player";
private:
	SceneManager* sceneManager_ = nullptr;
	ComponentManager* compManager_ = nullptr;
	InputManager* input = nullptr;

	// Components
	TransformComponent* tf = nullptr;
	MeshComponent* mesh = nullptr;
	MaterialComponent* material = nullptr;
	AnimationComponent* animation = nullptr;

	const std::string modelName = "BaseHuman";

	// parm
	Vector3 velocity;
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	// ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;

	const float fallVelocity = 0.5f;

	// 重力加速度
	const float kGravityAcceleration = 0.05f;

	// flag
	bool isJump = false;

	Behavior behavior = Behavior::kRoot;
	PlayerAnimation nowAnimation = PlayerAnimation::kIdle;
	// 次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest = std::nullopt;
};

