#include "PrecompiledHeader.h"
#include "DebugCamera.h"
#include"ChoMath.h"

#include"Input/InputManager.h"
#include"ECS/System/SystemManager/SystemManager.h"

void DebugCamera::Initialize(ComponentManager* componentManager,SystemManager* systemManager, InputManager* inputManager)
{
	componentManager_ = componentManager;
	systemManager_ = systemManager;
	inputManager_ = inputManager;

	componentManager->InitCameraComponent(&cameraComponent);
	systemManager_->DebugCameraUpdate(cameraComponent);
}

void DebugCamera::Update()
{
	// 各フラグ
	static bool isLeftClicked = false;
	static bool isWheelClicked = false;

	// 度数からラジアンに変換
	Vector3 radians = ChoMath::DegreesToRadians(cameraComponent.degrees);

	// 差分計算
	Vector3 diff = cameraComponent.preRot - radians;

	// 各軸のクオータニオンを作成
	Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), diff.x);
	Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), diff.y);
	Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), diff.z);

	// 同時回転を累積(順序は気にしなくていい)
	Quaternion rotation = qx * qy * qz * cameraComponent.rotation;

	// 回転を考慮する
	Matrix4 rotationMatrix = MakeRotateMatrix(rotation);
	Vector3 X = { 1.0f, 0.0f, 0.0f };
	Vector3 Y = { 0.0f, 1.0f, 0.0f };
	Vector3 Z = { 0.0f, 0.0f, -1.0f };

	Vector3 rotatedX = ChoMath::Transform(X, rotationMatrix);
	Vector3 rotatedY = ChoMath::Transform(Y, rotationMatrix);
	Vector3 rotatedZ = ChoMath::Transform(Z, rotationMatrix);

	/// ========カメラ操作========
	// カメラの回転を更新する
	if (inputManager_->IsPressMouse(Left)) {
		if (!isLeftClicked) {
			// マウスがクリックされたときに現在のマウス位置を保存する
			mousePos = inputManager_->GetMouseWindowPosition();
			//deltaMousePos.Initialize();
			isLeftClicked = true;
		}
		else {
			//float deltaX;
			//float deltaY;
			//// マウスがクリックされている間はカメラの回転を更新する
			//Vector2 pos;
			//if (deltaMousePos.x != 0.0f || deltaMousePos.y != 0.0f) {
			//	 pos= mousePos;
			//	 pos += deltaMousePos;
			//}
			//else {
			//	pos = inputManager_->GetMouseWindowPosition();
			//}

			// カメラ回転を更新
			cameraComponent.degrees.x += deltaMousePos.y * mouseSensitivity;
			cameraComponent.degrees.y += deltaMousePos.x * mouseSensitivity;
			
			//deltaX = static_cast<float>(pos.x-mousePos.x);
			//deltaY = static_cast<float>(pos.y-mousePos.y);

			//cameraComponent.degrees.x += deltaY * mouseSensitivity;
			//cameraComponent.degrees.y += deltaX * mouseSensitivity;

			//// 現在のマウス位置を保存する
			//mousePos =pos;
		}
	}
	else {
		// マウスがクリックされていない場合はフラグをリセットする
		isLeftClicked = false;
	}

	// カメラの位置を更新する
	if (inputManager_->IsPressMouse(Center)) {
		if (!isWheelClicked) {
			// マウスがクリックされたときに現在のマウス位置を保存する
			mousePos = inputManager_->GetMouseWindowPosition();
			//deltaMousePos.Initialize();
			isWheelClicked = true;
		}
		else {
			// マウスがクリックされている間はカメラの位置を更新する
			//Vector2 pos = inputManager_->GetMouseWindowPosition();
			////pos += deltaMousePos;
			///*if (pos.x != 0.0f || pos.y != 0.0f) {
			//	pos += deltaMousePos;
			//}*/

			//float deltaX = static_cast<float>(pos.x - mousePos.x);
			//float deltaY = static_cast<float>(pos.y - mousePos.y);

			//cameraComponent.translation -= rotatedX * deltaX * mouseSensitivity;
			//cameraComponent.translation += rotatedY * deltaY * mouseSensitivity;

			// カメラ回転を更新
			cameraComponent.translation -= rotatedX * deltaMousePos.x * mouseSensitivity;
			cameraComponent.translation += rotatedY * deltaMousePos.y * mouseSensitivity;

			// 現在のマウス位置を保存する
			//mousePos = pos;
		}
	}
	else {
		// マウスがクリックされていない場合はフラグをリセットする
		isWheelClicked = false;
	}

	// マウスホイールの移動量を取得する
	int32_t wheelDelta = -inputManager_->GetWheel();

	// マウスホイールの移動量に応じてカメラの移動を更新する
	cameraComponent.translation += rotatedZ * float(wheelDelta) * moveSpeed;
	/// =====================

	deltaMousePos.Initialize();

	systemManager_->DebugCameraUpdate(cameraComponent);
}
