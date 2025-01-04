#include "PrecompiledHeader.h"
#include "EmitterSystem.h"
#include"SystemState/SystemState.h"

void EmitterSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetParticleEntites()) {
		EmitterComponent* comp = componentManager.GetEmitter(entity);

		if (comp) {
			UpdateMatrix(comp);
		}
	}
}

void EmitterSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetParticleEntites()) {
		EmitterComponent* comp = componentManager.GetEmitter(entity);

		if (comp) {
			UpdateMatrix(comp);
		}
	}
}

void EmitterSystem::UpdateMatrix(EmitterComponent* comp)
{
	comp->frequencyTime += DeltaTime();// タイム加算

	// 射出間隔を上回ったら射出許可を出して時間を調整
	if (comp->frequency <= comp->frequencyTime) {
		comp->frequencyTime = 0.0f;
		comp->emit = 1;
	}
	else {
		// 射出間隔を上回ってないので、許可は出せない
		comp->emit = 0;
	}

	// 転送
	TransferMatrix(comp);
}

void EmitterSystem::TransferMatrix(EmitterComponent* comp)
{
	comp->constData->position = comp->position;
	comp->constData->radius = comp->radius;
	comp->constData->count = comp->count;
	comp->constData->frequency = comp->frequency;
	comp->constData->frequencyTime = comp->frequencyTime;
	comp->constData->emit = comp->emit;
}
