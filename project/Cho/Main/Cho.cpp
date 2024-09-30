#include "Cho.h"

// 静的メンバー変数の定義
bool Cho::endRequest_ = false;
std::unique_ptr <WinApp> Cho::win = nullptr;
std::unique_ptr <ResourceLeakChecker> Cho::resourceLeakChecker = nullptr;
std::unique_ptr <Framelate> Cho::framelate = nullptr;
std::unique_ptr <DXGIFactory> Cho::dxgiFactory = nullptr;
std::unique_ptr <D3DDevice> Cho::d3dDevice = nullptr;
std::unique_ptr<D3DFence> Cho::d3dFence = nullptr;
std::unique_ptr<D3DCommand> Cho::d3dCommand = nullptr;
std::unique_ptr<D3DSwapChain> Cho::d3dSwapChain = nullptr;

// System
#include"WinApp/WinApp.h"
#include"D3D12/ResourceLeakChecker/ResourceLeakChecker.h"
#include"Framelate/Framelate.h"
#include"D3D12/DXGIFactory/DXGIFactory.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/D3DFence/D3DFence.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"

void Cho::Initialize()
{
	// ウィンドウの作成
	win = std::make_unique<WinApp>();
	win->CreateGameWindow();

	// リソースリークチェッカー
	resourceLeakChecker = std::make_unique<ResourceLeakChecker>();

	// フレーム
	framelate = std::make_unique<Framelate>();
	framelate->Initialize();

#pragma region DirectX

	// DXGIファクトリー
	dxgiFactory = std::make_unique<DXGIFactory>();
	dxgiFactory->Initialize(true);

	// Device
	d3dDevice = std::make_unique<D3DDevice>();
	d3dDevice->Initialize(*dxgiFactory->GetDXGIFactory());

	// Fence
	d3dFence = std::make_unique<D3DFence>();
	d3dFence->Initialize(*d3dDevice->GetDevice());

	// Command
	d3dCommand = std::make_unique<D3DCommand>();
	d3dCommand->Initialize(*d3dDevice->GetDevice());

	// SwapChain
	d3dSwapChain = std::make_unique<D3DSwapChain>();
	d3dSwapChain->Initialize(
		win.get(),
		*dxgiFactory->GetDXGIFactory(),
		*d3dCommand->GetCommandQueue()
	);

#pragma endregion
}

void Cho::Finalize()
{
	// ウィンドウの破棄
	win->TerminateWindow();
}

void Cho::Operation()
{
	/*初期化*/
	Initialize();
	/*メインループ*/
	while (true) {
		/*ウィンドウ終了リクエスト*/
		if (IsEndRequest()) {
			break;
		}
		/*毎フレーム更新*/
		Update();
		/*描画前処理*/
		PreDraw();
		/*描画*/
		Draw();
		/*描画後処理*/
		PostDraw();
		/*フレーム更新*/
		framelate->Update();
	}
	/*終了処理*/
	Finalize();
}

void Cho::Update()
{
	if (win->ProcessMessage()) {
		endRequest_ = true;
	}
}

void Cho::PreDraw()
{
}

void Cho::Draw()
{
}

void Cho::PostDraw()
{
}

bool Cho::IsEndRequest()
{
	if (endRequest_) {
		return true;
	}
	return false;
}
