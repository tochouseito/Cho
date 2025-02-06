#include "PrecompiledHeader.h"
#include "EnemyHit.h"

void EnemyHit::Initialize()
{
	// 最初のエフェクトの初期化
	effect->render.visible = true;
	effect->effectNodes[0].common.visible = true;
	effect->effectNodes[0].common.emitCount = 30;
	effect->effectNodes[0].common.lifeTime.randValue.first = 110.0f;
	effect->effectNodes[0].common.emitTime.randValue.first = 121.0f;
	effect->effectNodes[0].common.emitStartTime.randValue.first = 1.0f;
	effect->effectNodes[0].position.type = PositionType::PositionPVA;
	effect->effectNodes[0].position.pva.velocity.first.y = 0.7f;
	effect->effectNodes[0].position.pva.velocity.second = { 0.6f,0.4f };
	effect->effectNodes[0].position.pva.acceleration.first.y = -0.1f;
	effect->effectNodes[0].rotation.type = RotateType::RotatePVA;
	effect->effectNodes[0].rotation.pva.velocity.second = { 0.5f,0.4f,0.0f };
	effect->effectNodes[0].scale.type = ScaleType::ScalePVA;
	effect->effectNodes[0].scale.pva.value.first = Vector3(0.5f, 0.5f, 0.5f);
	effect->effectNodes[0].drawCommon.textureName = "White1x1.png";
	effect->effectNodes[0].draw.meshType = EffectMeshType::EffectMeshTypeModel;
	effect->effectNodes[0].draw.meshModel.meshIndex = 16;
	effect->effectNodes[0].draw.meshSprite.color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	effect->effectNodes[0].draw.meshSprite.placement = Placement::PlacementYAxisConst;
}

void EnemyHit::Update()
{
}
