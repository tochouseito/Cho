#pragma once

#include"Vector3.h"
#include<cstdint>

struct EmitterSphere {
	Vector3 translate;// 位置
	float radius;// 射出半径
	uint32_t count;// 射出数
	float frequency;// 射出間隔
	float frequencyTime;// 射出間隔調整用時間
	uint32_t emit;// 射出許可
};

struct ConstBufferDataEmitter {

};