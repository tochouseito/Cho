#include "PrecompiledHeader.h"
#include "Pipeline.h"
#include"D3D12/ShaderCompiler/ShaderCompiler.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include<assert.h>

void Pipeline::Initialize(D3DDevice* d3dDevice, ShaderCompiler* shaderCompiler)
{
	d3dDevice_ = d3dDevice;

    shaderCompiler_ = shaderCompiler;

	CreatePSODemo();
	CreatePSOScreenCopy();
	CreatePSOSprite();
}

void Pipeline::CreatePSODemo()
{
	HRESULT hr;
	PSOBlend psoBlend;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/DemoOBJ.VS.hlsl",
			L"vs_6_5");
	assert(vertexShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> vertexShaderReflection = nullptr;
	vertexShaderReflection = shaderCompiler_->ReflectShader(vertexShaderBlob.Get());

	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/DemoOBJ.PS.hlsl",
			L"ps_6_5");
	assert(pixelShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> pixelShaderReflection = nullptr;
	pixelShaderReflection = shaderCompiler_->ReflectShader(pixelShaderBlob.Get());

	// RootSignature作成
	// copy用
	D3D12_ROOT_SIGNATURE_DESC descriptionRooTSignature{};
	descriptionRooTSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	std::vector< D3D12_ROOT_PARAMETER>rootParameter;

	std::vector<D3D12_DESCRIPTOR_RANGE>descriptorRange;

	std::vector<std::pair<uint32_t, std::string>> rootParms;

	rootParms =
		shaderCompiler_->CreateRootParameters(vertexShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_VERTEX);

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		psoBlend.rootParm.push_back(rootParm);
	}

	//vertexShaderReflection->Release();

	rootParms =
		shaderCompiler_->CreateRootParameters(pixelShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

	uint32_t offset = static_cast<uint32_t>(rootParms.size());

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		rootParm.first += offset;
		psoBlend.rootParm.push_back(rootParm);
	}

	//pixelShaderReflection->Release();

	for (int32_t i = 0; i < rootParameter.size(); ++i) {
		rootParameter[i].DescriptorTable.pDescriptorRanges = &descriptorRange[i];
	}


	descriptionRooTSignature.pParameters = rootParameter.data();// ルートパラメータ配列へのポインタ
	descriptionRooTSignature.NumParameters = static_cast<UINT>(rootParameter.size());// 配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;// レジスタ番号を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// PixelShaderで使う
	descriptionRooTSignature.pStaticSamplers = staticSamplers;
	descriptionRooTSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRooTSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(SUCCEEDED(hr));
	}

	// バイナリをもとに生成
	hr = d3dDevice_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&psoBlend.rootSignature));
	assert(SUCCEEDED(hr));

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);



	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しな
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みをします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = psoBlend.rootSignature.Get();  // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;   // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };                       // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };                        // PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
// どのように画面に色を打ち込むかの設定（気にしなくてもいい）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 実際に生成
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNone]));
	assert(SUCCEEDED(hr));

	// ここからブレンドPSOの各設定
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//!< 通常のaブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNormal]));
	assert(SUCCEEDED(hr));

	//!< 加算。Src * SrcA + Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeAdd]));
	assert(SUCCEEDED(hr));

	//!< 減算。Dest * 1 - Src * SrcA
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeSubtract]));
	assert(SUCCEEDED(hr));

	//!< 乗算。Src * 0 + Dest * Src
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeMultily]));
	assert(SUCCEEDED(hr));

	//!< スクリーン。Src * (1 - Dest) *Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeScreen]));
	assert(SUCCEEDED(hr));

	psoBlends.push_back(psoBlend);
}

void Pipeline::CreatePSOScreenCopy()
{
	HRESULT hr;
	PSOBlend psoBlend;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/FullScreen.VS.hlsl",
			L"vs_6_5");
	assert(vertexShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> vertexShaderReflection = nullptr;
	vertexShaderReflection = shaderCompiler_->ReflectShader(vertexShaderBlob.Get());

	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/FullScreen.PS.hlsl",
			L"ps_6_5");
	assert(pixelShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> pixelShaderReflection = nullptr;
	pixelShaderReflection = shaderCompiler_->ReflectShader(pixelShaderBlob.Get());

	// RootSignature作成
	// copy用
	D3D12_ROOT_SIGNATURE_DESC descriptionRooTSignature{};
	descriptionRooTSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	std::vector< D3D12_ROOT_PARAMETER>rootParameter;

	std::vector<D3D12_DESCRIPTOR_RANGE>descriptorRange;

	std::vector<std::pair<uint32_t, std::string>> rootParms;

	rootParms =
		shaderCompiler_->CreateRootParameters(vertexShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_VERTEX);

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		psoBlend.rootParm.push_back(rootParm);
	}

	//vertexShaderReflection->Release();

	rootParms =
		shaderCompiler_->CreateRootParameters(pixelShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

	uint32_t offset = static_cast<uint32_t>(rootParms.size());

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		rootParm.first += offset;
		psoBlend.rootParm.push_back(rootParm);
	}

	//pixelShaderReflection->Release();

	for (int32_t i = 0; i < rootParameter.size(); ++i) {
		rootParameter[i].DescriptorTable.pDescriptorRanges = &descriptorRange[i];
	}


	descriptionRooTSignature.pParameters = rootParameter.data();// ルートパラメータ配列へのポインタ
	descriptionRooTSignature.NumParameters = static_cast<UINT>(rootParameter.size());// 配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;// レジスタ番号を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// PixelShaderで使う
	descriptionRooTSignature.pStaticSamplers = staticSamplers;
	descriptionRooTSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRooTSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(SUCCEEDED(hr));
	}

	// バイナリをもとに生成
	hr = d3dDevice_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&psoBlend.rootSignature));
	assert(SUCCEEDED(hr));

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;




	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しな
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を無効化
	depthStencilDesc.DepthEnable = false;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = psoBlend.rootSignature.Get();  // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;   // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };                       // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };                        // PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
// どのように画面に色を打ち込むかの設定（気にしなくてもいい）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 実際に生成
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNone]));
	assert(SUCCEEDED(hr));

	// ここからブレンドPSOの各設定
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//!< 通常のaブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNormal]));
	assert(SUCCEEDED(hr));

	//!< 加算。Src * SrcA + Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeAdd]));
	assert(SUCCEEDED(hr));

	//!< 減算。Dest * 1 - Src * SrcA
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeSubtract]));
	assert(SUCCEEDED(hr));

	//!< 乗算。Src * 0 + Dest * Src
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeMultily]));
	assert(SUCCEEDED(hr));

	//!< スクリーン。Src * (1 - Dest) *Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeScreen]));
	assert(SUCCEEDED(hr));

	psoBlends.push_back(psoBlend);
}

void Pipeline::CreatePSOSprite()
{
	HRESULT hr;
	PSOBlend psoBlend;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/Sprite.VS.hlsl",
			L"vs_6_5");
	assert(vertexShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> vertexShaderReflection = nullptr;
	vertexShaderReflection = shaderCompiler_->ReflectShader(vertexShaderBlob.Get());

	Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob =
		shaderCompiler_->CompilerShader(
			L"Cho/Resources/Shader/Sprite.PS.hlsl",
			L"ps_6_5");
	assert(pixelShaderBlob);

	Microsoft::WRL::ComPtr < ID3D12ShaderReflection> pixelShaderReflection = nullptr;
	pixelShaderReflection = shaderCompiler_->ReflectShader(pixelShaderBlob.Get());

	// RootSignature作成
	// copy用
	D3D12_ROOT_SIGNATURE_DESC descriptionRooTSignature{};
	descriptionRooTSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform
	std::vector< D3D12_ROOT_PARAMETER>rootParameter;

	std::vector<D3D12_DESCRIPTOR_RANGE>descriptorRange;

	std::vector<std::pair<uint32_t, std::string>> rootParms;

	rootParms =
		shaderCompiler_->CreateRootParameters(vertexShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_VERTEX);

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		psoBlend.rootParm.push_back(rootParm);
	}

	//vertexShaderReflection->Release();

	rootParms =
		shaderCompiler_->CreateRootParameters(pixelShaderReflection.Get(), rootParameter, descriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

	uint32_t offset = static_cast<uint32_t>(rootParms.size());

	for (std::pair<uint32_t, std::string> rootParm : rootParms) {
		rootParm.first += offset;
		psoBlend.rootParm.push_back(rootParm);
	}

	//pixelShaderReflection->Release();

	for (int32_t i = 0; i < rootParameter.size(); ++i) {
		rootParameter[i].DescriptorTable.pDescriptorRanges = &descriptorRange[i];
	}


	descriptionRooTSignature.pParameters = rootParameter.data();// ルートパラメータ配列へのポインタ
	descriptionRooTSignature.NumParameters = static_cast<UINT>(rootParameter.size());// 配列の長さ

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;// 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;// ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0;// レジスタ番号を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// PixelShaderで使う
	descriptionRooTSignature.pStaticSamplers = staticSamplers;
	descriptionRooTSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRooTSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(SUCCEEDED(hr));
	}

	// バイナリをもとに生成
	hr = d3dDevice_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&psoBlend.rootSignature));
	assert(SUCCEEDED(hr));

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);



	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	// 裏面（時計回り）を表示しな
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みをします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = psoBlend.rootSignature.Get();  // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;   // InputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };                       // VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };                        // PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//RasterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
// どのように画面に色を打ち込むかの設定（気にしなくてもいい）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 実際に生成
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNone]));
	assert(SUCCEEDED(hr));

	// ここからブレンドPSOの各設定
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//!< 通常のaブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeNormal]));
	assert(SUCCEEDED(hr));

	//!< 加算。Src * SrcA + Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeAdd]));
	assert(SUCCEEDED(hr));

	//!< 減算。Dest * 1 - Src * SrcA
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeSubtract]));
	assert(SUCCEEDED(hr));

	//!< 乗算。Src * 0 + Dest * Src
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeMultily]));
	assert(SUCCEEDED(hr));

	//!< スクリーン。Src * (1 - Dest) *Dest * 1
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	graphicsPipelineStateDesc.BlendState = blendDesc;          // BlendState
	hr = d3dDevice_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&psoBlend.Blend[kBlendModeScreen]));
	assert(SUCCEEDED(hr));

	psoBlends.push_back(psoBlend);
}

