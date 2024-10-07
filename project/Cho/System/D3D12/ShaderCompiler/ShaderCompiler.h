#pragma once

#include<d3d12shader.h>
#include <d3dcompiler.h>
#include<dxcapi.h>

// C++
#include<array>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

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

	/// <summary>
	/// hlslファイルの数取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetHLSLFilesSize()const { return static_cast<uint32_t>(hlslFiles_.size()); }

private:

	/// <summary>
	/// dxcCompilerを初期化
	/// <summary>
	void InitializeDxcCompiler();

	// 指定されたフォルダから .hlsl ファイルを探し、そのファイル名を vector に保存する関数
	std::vector<std::string> GetHLSLFilesFromFolder(const std::string& folderPath);

private:

	/*dxcCompiler*/
	Microsoft::WRL::ComPtr < IDxcUtils> dxcUtils_;
	Microsoft::WRL::ComPtr < IDxcCompiler3> dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;

	std::string folderPath_ = "Cho/Resources/Shader"; // 対象のフォルダパスを指定
	std::vector<std::string> hlslFiles_;

};

