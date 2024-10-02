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
std::unique_ptr<ResourceViewManager>Cho::resourceViewManager = nullptr;
std::unique_ptr<RTVManager>Cho::rtvManager = nullptr;
std::unique_ptr<DSVManager> Cho::dsvManager = nullptr;
std::unique_ptr<DrawExecution> Cho::drawExecution = nullptr;

// Scene
std::unique_ptr<SceneManager> Cho::sceneManager = nullptr;

// 汎用機能
std::unique_ptr<ImGuiManager> Cho::imguiManager = nullptr;

// System
#include"WinApp/WinApp.h"
#include"D3D12/ResourceLeakChecker/ResourceLeakChecker.h"
#include"Framelate/Framelate.h"
#include"D3D12/DXGIFactory/DXGIFactory.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/D3DFence/D3DFence.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DSVManager/DSVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// Scene
#include"Scene/SceneManager/SceneManager.h"

// 汎用機能
#include"Editor/UI/ImGuiManager/ImGuiManager.h"

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
	d3dCommand->Initialize(*d3dDevice->GetDevice(),d3dFence.get());

	// SwapChain
	d3dSwapChain = std::make_unique<D3DSwapChain>();
	d3dSwapChain->Initialize(
		win.get(),
		*dxgiFactory->GetDXGIFactory(),
		*d3dCommand->GetCommandQueue()
	);

	// ResourceViewManager
	resourceViewManager = std::make_unique<ResourceViewManager>();
	resourceViewManager->Initialize(d3dDevice.get());

	// RTVManager
	rtvManager = std::make_unique<RTVManager>();
	rtvManager->Initialize(
		d3dDevice.get(),
		d3dSwapChain.get()
	);

	// DSVManager
	dsvManager = std::make_unique<DSVManager>();
	dsvManager->Initialize(
		win.get(),
		d3dDevice.get()
	);

	// DrawExecution
	drawExecution = std::make_unique<DrawExecution>();
	drawExecution->Initialize(
		d3dCommand.get(),
		d3dSwapChain.get(),
		resourceViewManager.get(),
		rtvManager.get(),
		dsvManager.get()
	);


#pragma endregion

	// SceneManager
	sceneManager = std::make_unique<SceneManager>();
	sceneManager->Initialize();

#pragma region 汎用機能初期化

	// ImGuiManager
	imguiManager = std::make_unique<ImGuiManager>();
	imguiManager->Initialize(
		win.get(),
		d3dDevice.get(),
		d3dCommand.get(),
		resourceViewManager.get()
	);

#pragma endregion

	// 最初のシーンを作成
	sceneManager->ChangeScene("MainScene");

}

void Cho::Finalize()
{
	// シーン解放
	sceneManager->Finalize();
	// ImGui解放
	imguiManager->Finalize();
	/*フェンスの終了*/
	d3dFence->Finalize();
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
	// ImGui受付開始
	imguiManager->Begin();
	// シーンを更新
	sceneManager->Update();
}

void Cho::PreDraw()
{
	// ImGui受付終了
	imguiManager->End();

	resourceViewManager->SetDescriptorHeap(d3dCommand->GetCommandList());

	drawExecution->PreDraw();
}

void Cho::Draw()
{
	sceneManager->Draw();
}

void Cho::PostDraw()
{

	drawExecution->PostDraw();

	// ImGui描画
	imguiManager->Draw();

	drawExecution->End();

	d3dCommand->Reset();
}

bool Cho::IsEndRequest()
{
	if (endRequest_) {
		return true;
	}
	return false;
}
