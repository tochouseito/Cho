#include "PrecompiledHeader.h"
#include "Player.h"
#include"ChoMath.h"
#include"Scene/SceneManager/SceneManager.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Input/InputManager.h"

void Player::Init(SceneManager* sceneManager, ComponentManager* compManager,InputManager* inputManager)
{
	compManager_ = compManager;

	sceneManager_ = sceneManager;
	compManager_ = compManager;
	input = inputManager;

	tf = compManager_->GetTransform(sceneManager_->GetGameObject(name)->GetEntityID());
	mesh = compManager_->GetMesh(sceneManager_->GetGameObject(name)->GetEntityID());
	material = compManager_->GetMaterial(sceneManager_->GetGameObject(name)->GetEntityID());
	animation = compManager_->GetAnimation(sceneManager_->GetGameObject(name)->GetEntityID());

	tf->translation.y = 0.0f;
	tf->scale *= 10.0f;

	std::string meshName = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->names[0];
	mesh->meshID = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->objects[meshName].meshIndex;
	mesh->meshModelName = modelName;

	material->textureID = "White1x1.png";
}

void Player::Update()
{
	// 振る舞いの初期化
	BehaviorInitialize();

	// 入力処理
	Input();

	// 振る舞いの更新
	BehaviorUpdate();

	// アニメーション更新
	AnimationUpdate();

	// 座標移動（ベクトルの加算）
	velocity.y = fallVelocity;
	tf->translation += velocity;
}

void Player::Input()
{
	// 移動処理
	Move();

	// ジャンプ
	Jump();

	// 落下
	Fall();

	//  ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		if (input->IsTriggerPadButton(PadButton::B) && !isJump) {
			//behaviorRequest = Behavior::kJump;
			isAttack = true;
		}
	}
}

void Player::Move()
{
	//  ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.sThumbLX || joyState.Gamepad.sThumbLY) {

			// キャラクターの移動ベクトル
			velocity = {
				static_cast<float>(joyState.Gamepad.sThumbLX), 0,
				static_cast<float>(joyState.Gamepad.sThumbLY) 
			};
			// 移動ベクトルの長さを1に正規化
			velocity.Normalize();
			velocity = velocity * kCharacterSpeed;
			Matrix4 matRot = (MakeRotateMatrix(compManager_->GetCamera(sceneManager_->GetCameraObject(followCameraName)->GetEntityID())->rotation));
				//(MakeRotateYMatrix(ChoMath::DegreesToRadians(compManager_->GetCamera(sceneManager_->GetCameraObject(followCameraName)->GetEntityID())->degrees.y)));
			velocity = ChoMath::TransformNormal(velocity, matRot);
			// 移動ベクトルの長さがゼロでないことを確認
			if (velocity.x != 0 || velocity.z != 0) {
				// 進行方向の角度を計算（Y軸回りの回転角度）
				float angle = std::atan2(velocity.x, velocity.z);
				// キャラクターの向きを更新
				float degreeY = ChoMath::RadiansToDegrees(angle);
				tf->degrees = { 0.0f, degreeY, 0.0f };
			}
		}
		else {
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
}

void Player::Jump()
{	
	//  ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		if (input->IsTriggerPadButton(PadButton::A)&&!isJump) {
			//behaviorRequest = Behavior::kJump;
			isJump = true;
			fallVelocity = kJumpFirstSpeed;
		}
	}
	if (input->TriggerKey(DIK_SPACE) && !isJump) {
		isJump = true;
		fallVelocity = kJumpFirstSpeed;
	}
}

void Player::Fall()
{
	// 加速度ベクトル
	float accelerationVector = -kGravityAcceleration;
	if (isJump) {
		fallVelocity += accelerationVector;
	}
	if (tf->translation.y < 0.0f) {
		fallVelocity = 0.0f;
		velocity.y = 0.0f;
		tf->translation.y = 0.0f;
		isJump = false;
	}
}

void Player::BehaviorInitialize()
{
	if (behaviorRequest) {
		// 振る舞いを変更する
		behavior = behaviorRequest.value();
		// 各振る舞い事の初期化を実行
		switch (behavior) {
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Player::Behavior::kJump:
			BehaviorJumpInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest = std::nullopt;
	}
}

void Player::BehaviorUpdate()
{
	switch (behavior) {
	case Player::Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Player::Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}
}

void Player::BehaviorRootInitialize()
{
}

void Player::BehaviorRootUpdate()
{

}

void Player::BehaviorAttackInitialize()
{
}

void Player::BehaviorAttackUpdate()
{
}

void Player::BehaviorJumpInitialize()
{
}

void Player::BehaviorJumpUpdate()
{
}

void Player::AnimationUpdate()
{
	// アニメーション遷移
	static Vector3 preVelocity = { 0.0f, 0.0f, 0.0f };
	if (velocity.y > 0.0f&&isJump) {
		nowAnimation = kFall;
	}
	else if (velocity.y < 0.0f && isJump) {
		nowAnimation = kFall;
	}
	else if (velocity.y==0.0f&&preVelocity.y<0.0f)
	{
		nowAnimation = kLand;
		if (animation->isLoop)
		{
			nowAnimation = kIdle;
		}
	}

	if (!isJump && nowAnimation != kFall) {
		if (velocity.x != 0.0f || velocity.z != 0.0f)
		{
			nowAnimation = kRun;
		}
		else {
			nowAnimation = kIdle;
		}
	}
	
	if (isAttack)
	{
		nowAnimation = kAttack;
		if (animation->isLoop)
		{
			isAttack=false;
		}
	}

	// AnimationIndex更新
	animation->animationIndex = static_cast<uint32_t>(nowAnimation);

	preVelocity = velocity;
}
