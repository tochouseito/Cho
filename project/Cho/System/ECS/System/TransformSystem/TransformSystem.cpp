#include "PrecompiledHeader.h"
#include "TransformSystem.h"

void TransformSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        TransformComponent* tfComp = componentManager.GetTransform(entity);

        if (tfComp) {
            InitMatrix(tfComp);
        }
    }
}

void TransformSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		TransformComponent* tfComp = componentManager.GetTransform(entity);

		if (tfComp) {
			UpdateMatrix(tfComp);
		}
	}
}

void TransformSystem::UpdateEditor(EntityManager& entityManager, ComponentManager& componentManager)
{
    for (Entity entity : entityManager.GetActiveEntities()) {
        TransformComponent* tfComp = componentManager.GetTransform(entity);

        if (tfComp) {
            InitMatrix(tfComp);
        }
    }
}

void TransformSystem::InitMatrix(TransformComponent* tfComp)
{
    // 度数からラジアンに変換
    Vector3 radians = ChoMath::DegreesToRadians(tfComp->degrees);

    // 各軸のクオータニオンを作成
    Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), radians.x);
    Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), radians.y);
    Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), radians.z);

    // 同時回転を累積
    tfComp->rotation = qx * qy * qz;

    // 精度を維持するための正規化
    tfComp->rotation.Normalize();

    // アフィン変換
    tfComp->matWorld = ChoMath::MakeAffineMatrix(tfComp->scale, tfComp->rotation, tfComp->translation);

    // 次のフレーム用に保存する
    tfComp->prePos = tfComp->translation;
    tfComp->preRot = radians;
    tfComp->preScale = tfComp->scale;

    // 親があれば親のワールド行列を掛ける
    if (tfComp->parent) {
        tfComp->matWorld = Multiply(tfComp->matWorld, *tfComp->parent);
    }

    // 行列の転送
    TransferMatrix(tfComp);
}

void TransformSystem::UpdateMatrix(TransformComponent* tfComp) {
    
    // 度数からラジアンに変換
    Vector3 radians = ChoMath::DegreesToRadians(tfComp->degrees);

    // 変更がなければreturn
    if (tfComp->translation==tfComp->prePos&&
        radians == tfComp->preRot&&
        tfComp->scale==tfComp->preScale) {
        return;
    }
    
    // 差分計算
    Vector3 diff = radians-tfComp->preRot;

    // 各軸のクオータニオンを作成
    Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), diff.x);
    Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), diff.y);
    Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), diff.z);

    // 同時回転を累積
    tfComp->rotation = qx * qy * qz * tfComp->rotation;

    // 精度を維持するための正規化
    tfComp->rotation.Normalize();
    
    // アフィン変換
    tfComp->matWorld = ChoMath::MakeAffineMatrix(tfComp->scale, tfComp->rotation, tfComp->translation);

    // 次のフレーム用に保存する
    tfComp->prePos = tfComp->translation;
    tfComp->preRot = radians;
    tfComp->preScale = tfComp->scale;

    // 行列の転送
    TransferMatrix(tfComp);
}

void TransformSystem::TransferMatrix(TransformComponent* tfComp) {
    tfComp->constData->matWorld = tfComp->matWorld;
    tfComp->constData->worldInverse = Transpose(Matrix4::Inverse(tfComp->matWorld));
    tfComp->constData->rootNode = tfComp->rootMatrix;
}
