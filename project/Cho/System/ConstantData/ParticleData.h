#pragma once
#include"Vector3.h"
#include<cstdint>

// 定数バッファデータ
struct ConstBufferDataPerFrame {
	float time;
	float deltaTime;
};
struct ConstBufferDataCounter {
	int32_t counter;
};
struct ConstBufferDataFreeList {
	uint32_t list;
};
struct ConstBufferDataFreeListIndex {
	int32_t index;
};
struct ConstBufferDataParticle {
	Vector3 translation;
	Vector3 rotation;
	Vector3 scale;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Color color;
	int isAlive;
};