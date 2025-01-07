#include "PrecompiledHeader.h"
#include "Player.h"
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

	tf->translation.y = 0.0f;
	tf->scale *= 10.0f;

	std::string meshName = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->names[0];
	mesh->meshID = sceneManager_->GetRVManagerPtr()->GetModelData(modelName)->objects[meshName].meshIndex;
	mesh->meshModelName = modelName;
}

void Player::Update()
{
	// 入力処理
	Input();
}

void Player::Input()
{
	// 移動処理
	Move();

	// ジャンプ
	Jump();

	// 落下
	Fall();

	tf->translation += velocity;
}

void Player::Move()
{

}

void Player::Jump()
{
	if (input->TriggerKey(DIK_SPACE)&&!isJump) {
		isJump = true;
		velocity.y = kJumpFirstSpeed;
	}
}

void Player::Fall()
{
	// 加速度ベクトル
	float accelerationVector = -kGravityAcceleration;
	if (isJump) {
		velocity.y += accelerationVector;
	}
	if (tf->translation.y < 0.0f) {
		velocity.y = 0.0f;
		tf->translation.y = 0.0f;
		isJump = false;
	}
}
