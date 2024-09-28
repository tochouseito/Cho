#include "Cho.h"

// 静的メンバー変数の定義
bool Cho::endRequest_ = false;
std::unique_ptr <WinApp> Cho::win = nullptr;
std::unique_ptr <ResourceLeakChecker> Cho::resourceLeakChecker = nullptr;
std::unique_ptr <Framelate> Cho::framelate = nullptr;
std::unique_ptr <DXGIFactory> Cho::dxgiFactory = nullptr;
std::unique_ptr <D3DDevice> Cho::d3dDevice = nullptr;

// System
#include"WinApp/WinApp.h"
#include"D3D12/ResourceLeakChecker/ResourceLeakChecker.h"
#include"Framelate/Framelate.h"
#include"D3D12/DXGIFactory/DXGIFactory.h"
#include"D3D12/D3DDevice/D3DDevice.h"

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

	// D3DDevice
	d3dDevice = std::make_unique<D3DDevice>();
	d3dDevice->Initialize(*dxgiFactory->GetDXGIFactory());

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
