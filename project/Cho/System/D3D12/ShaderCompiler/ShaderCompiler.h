#pragma once

#include<d3d12shader.h>
#include <d3dcompiler.h>
#include<dxcapi.h>

#include<string>

class ShaderCompiler
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// shaderのコンパイル
	/// </summary>
	IDxcBlob* CompilerShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile
	);

	/// <summary>
	/// シェーダーリフレクション
	/// </summary>
	/// <param name="shaderBlob"></param>
	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> ReflectShader(IDxcBlob* shaderBlob);

private:

	/// <summary>
	/// dxcCompilerを初期化
	/// <summary>
	void InitializeDxcCompiler();

private:

	/*dxcCompiler*/
	Microsoft::WRL::ComPtr < IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr < IDxcCompiler3> dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;

};

