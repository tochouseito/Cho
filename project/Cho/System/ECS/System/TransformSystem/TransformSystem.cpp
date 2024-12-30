#include "PrecompiledHeader.h"
#include "TransformSystem.h"

void TransformSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
    entityManager;
    componentManager;
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

void TransformSystem::UpdateMatrix(TransformComponent* tfComp) {
    
    // 度数からラジアンに変換
    Vector3 radians = ChoMath::DegreesToRadians(tfComp->degrees);

    // 変更がなければreturn
    if (radians == tfComp->preRot) {
        return;
    }

    // 差分計算
    Vector3 diff = tfComp->preRot - radians;

    // 各軸のクオータニオンを作成
    Quaternion qx = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.0f, 0.0f), diff.x);
    Quaternion qy = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 1.0f, 0.0f), diff.y);
    Quaternion qz = ChoMath::MakeRotateAxisAngleQuaternion(Vector3(0.0f, 0.0f, 1.0f), diff.z);

    // 同時回転を累積（順序は気にしなくていい)
    tfComp->rotation = qx * qy * qz * tfComp->rotation;

    // 精度を維持するための正規化
    tfComp->rotation.Normalize();
    
    // アフィン変換
    tfComp->matWorld = ChoMath::MakeAffineMatrix(tfComp->scale, tfComp->rotation, tfComp->translation);

    // 次のフレーム用に保存する
    tfComp->preRot = radians;

    // 行列の転送
    TransferMatrix(tfComp);
}

void TransformSystem::TransferMatrix(TransformComponent* tfComp) {
    tfComp->constData->matWorld = tfComp->matWorld;
    tfComp->constData->worldInverse = Transpose(Matrix4::Inverse(tfComp->matWorld));
    tfComp->constData->rootNode = tfComp->rootMatrix;
}
