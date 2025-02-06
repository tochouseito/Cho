#include "PrecompiledHeader.h"
#include "PlayerLandEffect.h"

void PlayerLandEffect::Initialize()
{
	// 最初のエフェクトの初期化
	effect->render.visible = true;
	effect->effectNodes[0].common.visible = true;
	effect->effectNodes[0].common.emitCount = 30;
	effect->effectNodes[0].common.lifeTime.randValue.first = 15.0f;
	effect->effectNodes[0].common.emitTime.randValue.first = 121.0f;
	effect->effectNodes[0].common.emitStartTime.randValue.first = 1.0f;
	effect->effectNodes[0].position.type = PositionType::PositionPVA;
	effect->effectNodes[0].position.pva.velocity.second.x = 0.2f;
	effect->effectNodes[0].scale.type = ScaleType::ScalePVA;
	effect->effectNodes[0].scale.pva.value.first = Vector3(1.0f, 1.0f, 1.0f);
	effect->effectNodes[0].scale.pva.velocity.first = { -0.05f,-0.05f,0.0f };
	effect->effectNodes[0].scale.pva.velocity.second = { 0.04f,0.04f,0.0f };
	effect->effectNodes[0].drawCommon.textureName = "50.png";
	effect->effectNodes[0].draw.meshType = EffectMeshType::EffectMeshTypeSprite;
	effect->effectNodes[0].draw.meshSprite.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
}

void PlayerLandEffect::Update()
{
}
