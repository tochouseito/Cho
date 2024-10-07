#include "PrecompiledHeader.h"
#include "ShaderCompiler.h"

void ShaderCompiler::Initialize()
{
	// DXコンパイラー初期化
	InitializeDxcCompiler();

	// HLSLファイルの名前を読み込み
	hlslFiles_ = GetHLSLFilesFromFolder(folderPath_);
}

IDxcBlob* ShaderCompiler::CompilerShader(
	const std::wstring& filePath,
	const wchar_t* profile
)
{
	// hlslファイルを読む
	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompilerShader,path:{},profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_.Get()->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;// UTF8の文字コードであることを通知
	LPCWSTR arguments[] = {
		filePath.c_str(),       //コンパイル対象のhlslファイル名
		L"-E",L"main",          // エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,          // ShaderPrifileの設定
		L"-Zi",L"-Qembed_debug",// デバッグ用の情報を埋め込む
		L"-Od",                 // 最適化を外しておく
		L"-Zpr",                // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_.Get()->Compile(
		&shaderSourceBuffer,       // 読み込んだファイル
		arguments,                 // コンパイルオプション
		_countof(arguments),       // コンパイル結果
		includeHandler_,     // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	// 警告、エラーがでてたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlobUtf16* shaderOutputName;// 出力名用の変数
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), &shaderOutputName);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// エラー警告ダメゼッタイ
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &shaderOutputName);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	shaderError->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}

Microsoft::WRL::ComPtr < ID3D12ShaderReflection> ShaderCompiler::ReflectShader(IDxcBlob* shaderBlob)
{
	Microsoft::WRL::ComPtr<IDxcContainerReflection> pContainerReflection;
	HRESULT hr = DxcCreateInstance(CLSID_DxcContainerReflection, IID_PPV_ARGS(&pContainerReflection));
	assert(SUCCEEDED(hr));

	// コンテナにシェーダーブロブをロード
	hr = pContainerReflection->Load(shaderBlob);
	assert(SUCCEEDED(hr));

	// DXIL部分のインデックスを取得
	UINT32 shaderIdx;
	hr = pContainerReflection->FindFirstPartKind(DXC_PART_DXIL, &shaderIdx);
	assert(SUCCEEDED(hr));

	// シェーダーリフレクションインターフェースを取得
	Microsoft::WRL::ComPtr<ID3D12ShaderReflection> pReflector;
	hr = pContainerReflection->GetPartReflection(shaderIdx, IID_PPV_ARGS(&pReflector));
	assert(SUCCEEDED(hr));

	return pReflector;
}

void ShaderCompiler::InitializeDxcCompiler()
{
	HRESULT hr;
	// dxcCompilerを初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	// 現時点でincludeはしないが、includeに対応するための設定を作っておく
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

std::vector<std::string> ShaderCompiler::GetHLSLFilesFromFolder(const std::string& folderPath)
{
	std::vector<std::string> hlslFiles;

	// ディレクトリの存在を確認
	if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
		//std::cerr << "フォルダが存在しないか、ディレクトリではありません: " << folderPath << std::endl;
		return hlslFiles;
	}

	// ディレクトリを走査
	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".hlsl") {
			// ファイル名のみを保存 (拡張子を含むファイル名)
			hlslFiles.push_back(entry.path().filename().string());
		}
	}

	return hlslFiles;
}
