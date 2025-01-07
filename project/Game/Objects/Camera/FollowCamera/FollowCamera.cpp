#include "PrecompiledHeader.h"
#include "FollowCamera.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"Scene/SceneManager/SceneManager.h"
#include"Input/InputManager.h"
#include"ChoMath.h"

void FollowCamera::Init(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;
	input = inputManager;

	comp = compManager_->GetCamera(sceneManager_->GetCameraObject(name)->GetEntityID());

	comp->translation.y = 10.0f;
	//comp->parent = &compManager_->GetTransform(sceneManager_->GetGameObject(targetName)->GetEntityID())->matWorld;
}

void FollowCamera::Update()
{
	// 入力
	Input();
}

void FollowCamera::Input()
{
	Move();
}

void FollowCamera::Move()
{
	//  ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	// ゲームパッド状態取得
	if (input->GetJoystickState(0, joyState)) {
		// 移動ベクトル
		Vector3 rotate = { 0, 0, 0 };

		// 移動速度
		const float kCharacterSpeed = 0.2f;
		rotate.y += -static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * kCharacterSpeed;
		// 座標移動（ベクトルの加算）
		Vector3 degree = ChoMath::RadiansToDegrees(rotate);
		comp->degrees += degree;
	}
	if (targetName!="") {
		targetOffset_ = { 0.0f, 16.0f, -30.0f };

		// 度数からラジアンに変換
		Vector3 radians = ChoMath::DegreesToRadians(comp->degrees);

		//// 変更がなければreturn
		//if (comp->translation == comp->prePos &&
		//	radians == comp->preRot) {
		//	return;
		//}

		// 差分計算
		Vector3 diff = comp->preRot - radians;

		// 各軸のクオータニオンを作成
		Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), diff.x);
		Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), diff.y);
		Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), diff.z);

		// 同時回転を累積(順序は気にしなくていい)
		Quaternion rotation = qx * qy * qz * comp->rotation;
		Matrix4 matRot_ = ChoMath::MakeRotateMatrix(rotation);
		
		// オフセットをカメラの回転に合わせて回転させる
		targetOffset_ = ChoMath::TransformNormal(targetOffset_, matRot_);
		// 追従対象とオフセットからカメラの座標を計算
		comp->translation = compManager_->GetTransform(sceneManager_->GetGameObject(targetName)->GetEntityID())->translation + targetOffset_;
	}
}
