#include "PrecompiledHeader.h"
#include "CameraSystem.h"

void CameraSystem::InitMatrix(CameraComponent& compo)
{
	// 度数からラジアンに変換
	Vector3 radians = ChoMath::DegreesToRadians(compo.degrees);

	// 変更がなければreturn
	if (compo.translation == compo.prePos &&
		radians == compo.preRot) {
		return;
	}

	// 各軸のクオータニオンを作成
	Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), radians.x);
	Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), radians.y);
	Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), radians.z);

	// 同時回転を累積
	compo.rotation = qx * qy * qz;

	// アフィン変換
	compo.matWorld = MakeAffineMatrix(Scale(1.0f, 1.0f, 1.0f), compo.rotation, compo.translation);

	// 次のフレーム用に保存する
	compo.prePos = compo.translation;
	compo.preRot = radians;

	// 行列の転送
	TransferMatrix(compo);
}

void CameraSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetCameraEntities()) {
		CameraComponent* cameraCompo = componentManager.GetCamera(entity);

		if (cameraCompo) {
			InitMatrix(*cameraCompo);
		}
	}
}

void CameraSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetCameraEntities()) {
		CameraComponent* cameraCompo = componentManager.GetCamera(entity);

		if (cameraCompo) {
			UpdateMatrix(*cameraCompo);
		}
	}
}

void CameraSystem::UpdateEditor(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetCameraEntities()) {
		CameraComponent* cameraCompo = componentManager.GetCamera(entity);

		if (cameraCompo) {
			InitMatrix(*cameraCompo);
		}
	}
}

void CameraSystem::UpdateMatrix(CameraComponent& compo) {

	// 度数からラジアンに変換
	Vector3 radians = ChoMath::DegreesToRadians(compo.degrees);

	// 変更がなければreturn
	if (compo.translation==compo.prePos&&
		radians == compo.preRot) {
		return;
	}

	// 差分計算
	Vector3 diff = compo.preRot - radians;

	// 各軸のクオータニオンを作成
	Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), diff.x);
	Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), diff.y);
	Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), diff.z);

	// 同時回転を累積(順序は気にしなくていい)
	compo.rotation = qx * qy * qz * compo.rotation;
	
	// アフィン変換
	compo.matWorld = MakeAffineMatrix(Scale(1.0f, 1.0f, 1.0f), compo.rotation, compo.translation);

	// 次のフレーム用に保存する
	compo.prePos = compo.translation;
	compo.preRot = radians;

	// 親があれば親のワールド行列を掛ける
	if (compo.parent) {
		compo.matWorld = Multiply(compo.matWorld, *compo.parent);
	}

	// 行列の転送
	TransferMatrix(compo);
}

void CameraSystem::TransferMatrix(CameraComponent& compo) {
	compo.constData->matWorld = compo.matWorld;
	compo.constData->view = Matrix4::Inverse(compo.matWorld);
	//float width = static_cast<float>(WindowWidth());
	//float height = static_cast<float>(WindowHeight());
	compo.constData->projection = MakePerspectiveFovMatrix(
		compo.fovAngleY, compo.aspectRatio, compo.nearZ, compo.farZ
	);
	compo.constData->cameraPosition = compo.translation;
}

void CameraSystem::UpdateDebugCamera(CameraComponent& camera)
{
	InitMatrix(camera);
}
