#pragma once

#include"ConstantData/ParticleData.h"

static const uint32_t kMaxParticle = 1024;// 最大パーティクル

struct ParticleComponent {

	uint32_t count = 512;// パーティクル数

	ConstBufferDataParticle* constData = nullptr;

	uint32_t uavIndex = 0;
};