#pragma once

#include<wrl.h>
#include<d3d12.h>

// C++
#include<array>
#include <vector>

// Utility
#include"BlendMode.h"

enum PSOMode {
	Demo=0,
	ScreenCopy,
	Sprite,
	Particle,
	Init,
	Emit,
	ParticleUpdate,
};

class D3DDevice;
class ShaderCompiler;
class Pipeline
{
public:
	struct PSOBlend {
		std::array<Microsoft::WRL::ComPtr <ID3D12PipelineState>,
			BlendMode::kCountOfBlendMode> Blend;
		Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
		std::vector<std::pair<uint32_t, std::string>> rootParm;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(D3DDevice* d3dDevice,ShaderCompiler* shaderCompiler);

	PSOBlend& GetPSO(const PSOMode& index) { return psoBlends[index]; }

private:

	void CreatePSODemo();

	void CreatePSOScreenCopy();

	void CreatePSOSprite();

	void CreatePSOParticle();

	void CreatePSOParticleInit();

	void CreatePSOEmit();

	void CreatePSOUpdate();

private:

	D3DDevice* d3dDevice_ = nullptr;

	std::vector<PSOBlend> psoBlends;
	
	ShaderCompiler* shaderCompiler_ = nullptr;
};

