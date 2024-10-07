#include "PrecompiledHeader.h"
#include "GraphicsSystem.h"

void GraphicsSystem::Initialize()
{
	// シェーダコンパイラーの初期化
	shaderCompiler = std::make_unique<ShaderCompiler>();
	shaderCompiler->Initialize();
}
