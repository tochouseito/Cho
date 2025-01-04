#pragma once
#include"ConstantData/EmitterData.h"

struct EmitterComponent {

	ConstBufferDataEmitter* constData = nullptr;

	uint32_t cbvIndex = 0;

};