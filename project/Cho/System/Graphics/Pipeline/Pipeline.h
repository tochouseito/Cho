#pragma once

#include<wrl.h>
#include<d3d12.h>

// C++
#include<array>
#include <vector>

// Utility
#include"BlendMode.h"

class Pipeline
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:
	std::vector<
		std::array<
		Microsoft::WRL::ComPtr <ID3D12PipelineState>,
		BlendMode::kCountOfBlendMode>> PSO;
	
};

