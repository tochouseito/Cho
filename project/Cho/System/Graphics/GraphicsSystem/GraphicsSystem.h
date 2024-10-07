#pragma once

// ShaderCompiler
#include"D3D12/ShaderCompiler/ShaderCompiler.h"

// Pipeline
#include"Graphics/Pipeline/Pipeline.h"

// C++
#include<memory>

class GraphicsSystem
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:

	// ShaderCompiler
	std::unique_ptr<ShaderCompiler> shaderCompiler;

	// Pipeline
	std::unique_ptr<Pipeline> pipeline;

};

