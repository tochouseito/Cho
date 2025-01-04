#pragma once
#include"Vector3.h"
#include<cstdint>
//struct AccelerationField {
//	Vector3 acceleration;//!<加速度
//	AABB area;			 //!<範囲
//};
//struct Particle {
//	Vector3 translation;
//	Vector3 rotation;
//	Vector3 scale;
//	float lifeTime;
//	Vector3 velocity;
//	float currentTime;
//	Color color;
//	int isAlive;
//};
//
//struct PerFrame
//{
//	float time;
//	float deltaTime;
//};
//struct ParticleGroup {
//	std::list<Particle> particles;
//	//Emitter emitter;
//	EmitterSphere* emitterSphere = nullptr;
//	AccelerationField accelerationField;
//	std::string textureHandle;
//	//std::unique_ptr<Mesh> mesh;
//	Microsoft::WRL::ComPtr<ID3D12Resource> particleResource;
//	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>particleSrvHandle;
//	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>particleUavHandle;
//	uint32_t srvIndex;
//	uint32_t uavIndex;
//	Microsoft::WRL::ComPtr<ID3D12Resource> emitResource;
//	Microsoft::WRL::ComPtr<ID3D12Resource> frameResource;
//	PerFrame* perFrame = nullptr;
//	Microsoft::WRL::ComPtr<ID3D12Resource> counterResource;
//	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>counterUavHandle;
//	uint32_t counterUavIndex;
//	Microsoft::WRL::ComPtr<ID3D12Resource> listResource;
//	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>listUavHandle;
//	uint32_t listUavIndex;
//	Microsoft::WRL::ComPtr<ID3D12Resource> listIndexResource;
//	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>listIndexUavHandle;
//	uint32_t listIndexUavIndex;
//};

// 定数バッファデータ
struct ConstBufferDataParticle {

};