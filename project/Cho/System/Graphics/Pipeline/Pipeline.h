#pragma once

#include<wrl.h>
#include<d3d12.h>

// C++
#include<array>
#include <vector>

// Utility
#include"BlendMode.h"

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

	void CreatePSODemo();

	void CreatePSOScreenCopy();

	PSOBlend& GetPSO(uint32_t index) { return psoBlends[index]; }

private:

	D3DDevice* d3dDevice_ = nullptr;

	/*std::vector<
		std::array<
		Microsoft::WRL::ComPtr <ID3D12PipelineState>,
		BlendMode::kCountOfBlendMode>> PSO;*/
	std::vector<PSOBlend> psoBlends;
	
	ShaderCompiler* shaderCompiler_ = nullptr;
};

