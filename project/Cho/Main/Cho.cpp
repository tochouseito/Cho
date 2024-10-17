#include "Cho.h"

// System
#include"WinApp/WinApp.h"
#include"D3D12/ResourceLeakChecker/ResourceLeakChecker.h"
#include"FrameRate/FrameRate.h"
#include"D3D12/DXGIFactory/DXGIFactory.h"
#include"D3D12/D3DDevice/D3DDevice.h"
#include"D3D12/D3DFence/D3DFence.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/D3DSwapChain/D3DSwapChain.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DSVManager/DSVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// GraphicsSystem
#include"Graphics/GraphicsSystem/GraphicsSystem.h"

// Context
#include"UI/GameContext/GameContext.h"

// ECS
#include"ECS/EntityManager/EntityManager.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"ECS/System/SystemManager/SystemManager.h"
#include"ECS/PrefabManager/PrefabManager.h"

// Scene
#include"Scene/SceneManager/SceneManager.h"

// 汎用機能
#include"UI/ImGuiManager/ImGuiManager.h"

// Editor
#include"Editor/EditorManager/EditorManager.h"

#pragma region 静的メンバー変数の定義
std::unique_ptr <WinApp> Cho::win = nullptr;
std::unique_ptr <ResourceLeakChecker> Cho::resourceLeakChecker = nullptr;
std::unique_ptr <FrameRate> Cho::frameRate = nullptr;
std::unique_ptr <DXGIFactory> Cho::dxgiFactory = nullptr;
std::unique_ptr <D3DDevice> Cho::d3dDevice = nullptr;
std::unique_ptr<D3DFence> Cho::d3dFence = nullptr;
std::unique_ptr<D3DCommand> Cho::d3dCommand = nullptr;
std::unique_ptr<D3DSwapChain> Cho::d3dSwapChain = nullptr;
std::unique_ptr<ResourceViewManager>Cho::resourceViewManager = nullptr;
std::unique_ptr<RTVManager>Cho::rtvManager = nullptr;
std::unique_ptr<DSVManager> Cho::dsvManager = nullptr;
std::unique_ptr<DrawExecution> Cho::drawExecution = nullptr;

// GraphicsSystem
std::unique_ptr<GraphicsSystem> Cho::graphicsSystem = nullptr;

// GameContext
std::unique_ptr<GameContext> Cho::gameContext = nullptr;

// ECS
std::unique_ptr<EntityManager>Cho::entityManager = nullptr;
std::unique_ptr<ComponentManager> Cho::componentManager = nullptr;
std::unique_ptr<SystemManager> Cho::systemManager = nullptr;
std::unique_ptr<PrefabManager> Cho::prefabManager = nullptr;

// Scene
std::unique_ptr<SceneManager> Cho::sceneManager = nullptr;

// 汎用機能
std::unique_ptr<ImGuiManager> Cho::imguiManager = nullptr;

// Editor
std::unique_ptr<EditorManager> Cho::editorManager = nullptr;

#pragma endregion

void Cho::Initialize()
{
	// リソースリークチェッカー
	resourceLeakChecker = std::make_unique<ResourceLeakChecker>();

	// ウィンドウの作成
	win = std::make_unique<WinApp>();
	win->CreateGameWindow();

	// GameContext
	gameContext = std::make_unique<GameContext>();
	gameContext->Initialize();

	// フレーム
	frameRate = std::make_unique<FrameRate>();
	frameRate->Initialize(gameContext.get());

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

#pragma region グラフィックスシステム

	// GraphicsSystem
	graphicsSystem = std::make_unique<GraphicsSystem>();
	graphicsSystem->Initialize(d3dDevice.get());

#pragma endregion

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

#pragma region ECS

	// EntityManager
	entityManager = std::make_unique<EntityManager>();

	// ComponentManager
	componentManager = std::make_unique<ComponentManager>();

	// SystemManager
	systemManager = std::make_unique<SystemManager>();

	// PrefabManager
	prefabManager = std::make_unique<PrefabManager>();

#pragma endregion

	// SceneManager
	sceneManager = std::make_unique<SceneManager>();
	sceneManager->Initialize(
		entityManager.get(),
		componentManager.get(),
		systemManager.get(),
		prefabManager.get()
		);

#pragma region プロジェクトデータの読み込み

#pragma endregion

#pragma region エディター

	// EditorManager
	editorManager = std::make_unique<EditorManager>();
	editorManager->Initialize(
		entityManager.get(),
		componentManager.get(),
		systemManager.get(),
		prefabManager.get(),
		sceneManager.get()
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
		if (win->ProcessMessage()) {
			break;
		}
		if (!win->IsEndApp()) {
			/*毎フレーム更新*/
			Update();
			/*描画前処理*/
			PreDraw();
			/*描画*/
			Draw();
			/*描画後処理*/
			PostDraw();
			/*フレーム更新*/
			frameRate->Update();
		}
	}
	/*終了処理*/
	Finalize();
}

void Cho::Update()
{
	// ImGui受付開始
	imguiManager->Begin();

	// エディタを更新
	editorManager->Update();

	// シーンを更新
	sceneManager->Update();

	systemManager->Update(*entityManager.get(), *componentManager.get(),0.01f);

	// SystemUIの更新
	gameContext->Update();
}

void Cho::PreDraw()
{
	// ImGui受付終了
	imguiManager->End();

	// コマンドリスト記録開始
	d3dCommand->Reset();

	// 使うディスクリプタヒープをセット
	resourceViewManager->SetDescriptorHeap(d3dCommand->GetCommandList());

	// 描画前処理
	drawExecution->PreDraw();
}

void Cho::Draw()
{
	// シーン描画
	sceneManager->Draw();

	systemManager->Draw(*entityManager.get(), *componentManager.get());
}

void Cho::PostDraw()
{
	// 描画後処理
	drawExecution->PostDraw();

	// ImGui描画
	imguiManager->Draw();

	// 描画終了処理
	drawExecution->End();
}
