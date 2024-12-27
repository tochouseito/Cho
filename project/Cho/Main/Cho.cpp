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

// Loader
#include"Load/TextureLoader/TextureLoader.h"
#include"Load/MeshLoader/MeshLoader.h"
#include"Load/ModelLoader/ModelLoader.h"

// Script
#include "Script/ScriptManager/ScriptManager.h"
#include"Generator/ScriptProject/ScriptProject.h"

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

// StartSetting
#include"StartSetting/StartSetting.h"

// Json
#include"Load/JsonFileLoader/JsonFileLoader.h"

// Save
#include"SaveManager/SaveManager.h"

#pragma region 静的メンバー変数の定義
SystemState& Cho::systemState = SystemState::GetInstance();

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

// Loader
std::unique_ptr<TextureLoader> Cho::textureLoader = nullptr;
std::unique_ptr<MeshLoader> Cho::meshLoader = nullptr;
std::unique_ptr<ModelLoader> Cho::modelLoader = nullptr;

// Script
std::unique_ptr<ScriptManager> Cho::scriptManager = nullptr;

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

/*Starter*/
std::unique_ptr<StartSetting> Cho::startSetting = nullptr;

/*Json*/
std::unique_ptr<JsonFileLoader> Cho::jsonFileLoader = nullptr;

/*Save*/
std::unique_ptr<SaveManager> Cho::saveManager = nullptr;

#pragma endregion

void Cho::Initialize()
{
	// リソースリークチェッカー
	resourceLeakChecker = std::make_unique<ResourceLeakChecker>();

	// 共有クラスの初期化
	systemState.Initialize();

	// ウィンドウの作成
	win = std::make_unique<WinApp>();
	win->CreateGameWindow();

	// フレーム
	frameRate = std::make_unique<FrameRate>();
	frameRate->Initialize();

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
	resourceViewManager->Initialize(d3dDevice.get(),d3dCommand.get());

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

#pragma endregion

#pragma region グラフィックスシステム

	// GraphicsSystem
	graphicsSystem = std::make_unique<GraphicsSystem>();
	graphicsSystem->Initialize(d3dDevice.get());

	// DrawExecution
	drawExecution = std::make_unique<DrawExecution>();
	drawExecution->Initialize(
		d3dCommand.get(),
		d3dSwapChain.get(),
		resourceViewManager.get(),
		rtvManager.get(),
		dsvManager.get(),
		graphicsSystem.get()
	);

#pragma endregion

#pragma region 汎用機能初期化

	// TextureLoader
	textureLoader = std::make_unique<TextureLoader>();
	textureLoader->Initialize(
		d3dDevice.get(),
		d3dCommand.get(),
		resourceViewManager.get()
	);

	// MeshLoader
	meshLoader = std::make_unique<MeshLoader>();
	meshLoader->Initialize();
	
	// ModelLoader
	modelLoader = std::make_unique<ModelLoader>();
	modelLoader->Initialize(meshLoader.get());

	// ScriptManager
	scriptManager = std::make_unique<ScriptManager>();
	scriptManager->Initialize();

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
	componentManager->SetRVManager(resourceViewManager.get());

	// SystemManager
	systemManager = std::make_unique<SystemManager>();
	systemManager->Initialize(entityManager.get(), componentManager.get());

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
		resourceViewManager.get(),
		rtvManager.get(),
		drawExecution.get(),
		textureLoader.get(),
		entityManager.get(),
		componentManager.get(),
		systemManager.get(),
		prefabManager.get(),
		sceneManager.get(),
		scriptManager.get()
		);

#pragma endregion

#pragma region JSON

	// Json
	jsonFileLoader = std::make_unique<JsonFileLoader>();
	jsonFileLoader->Initialize(
		scriptManager.get(),
		entityManager.get(),
		componentManager.get(),
		prefabManager.get(),
		sceneManager.get()
	);

#pragma endregion

	// スタート設定
	startSetting = std::make_unique<StartSetting>();

	/*読み込み*/
	Load();

	// デフォルトメッシュ生成
	resourceViewManager->CreateMeshPattern();

}

void Cho::Finalize()
{
	// セーブ
	Save();
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
		frameRate->StartFrame();
		/*ウィンドウ終了リクエスト*/
		if (win->ProcessMessage()) {
			break;
		}
		SystemStateEvent();
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
		frameRate->EndFrame();
	}
	/*終了処理*/
	Finalize();
}

void Cho::Update()
{
	// ImGui受付開始
	imguiManager->Begin();

	SelectGameProject();

	// エディタを更新
	editorManager->Update();

	// シーンを更新
	sceneManager->Update();

	systemManager->Update(*entityManager.get(), *componentManager.get(),0.01f);
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

	systemManager->Draw(
		*entityManager.get(),
		*componentManager.get(),
		d3dCommand.get(),
		resourceViewManager.get(),
		graphicsSystem.get(),
		textureLoader.get()
	);

	drawExecution->DebugPreDraw();
	// デバッグ用
	systemManager->DebugDraw(
		*entityManager.get(),
		*componentManager.get(),
		d3dCommand.get(),
		resourceViewManager.get(),
		graphicsSystem.get(),
		textureLoader.get()
	);
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

void Cho::Load()
{
	/*ImGuiのスタイル読み込み*/
	jsonFileLoader->LoadStyleFromProject();

	/*テクスチャリソースの読み込み*/
	textureLoader->FirstResourceLoad("Cho/Resources/Texture/");
}

void Cho::StartSetUp()
{
	startSetting->Initialize(
		resourceViewManager.get(),
		rtvManager.get(),
		drawExecution.get(),
		entityManager.get(),
		componentManager.get(),
		systemManager.get(),
		prefabManager.get(),
		sceneManager.get(),
		editorManager.get()
	);

	/*テクスチャリソースの読み込み*/
	std::string textureRoot = startSetting->GetProjectRoot();
	std::string projectName = ProjectName();
	textureRoot = textureRoot + "\\" + projectName + "\\" + "Assets\\Texture\\";
	textureLoader->FirstResourceLoad(textureRoot);
}

void Cho::Save()
{
	saveManager = std::make_unique<SaveManager>();
	saveManager->Initialize(
		scriptManager.get(),
		entityManager.get(),
		componentManager.get(),
		prefabManager.get(),
		sceneManager.get()
	);

	saveManager->Save(jsonFileLoader.get());

	/*ImGuiのスタイルを保存*/
	jsonFileLoader->SaveStyleToProject();

	saveManager.reset();
	saveManager = nullptr;
}

void Cho::SystemStateEvent()
{
	// ウィンドウサイズ変更時スワップチェーン、RTVのリサイズ
	if (SystemState::GetInstance().WindowResize()) {
		d3dCommand->Signal();

		// 既存のリソースの解放,再作成
		dsvManager->Resize();
		drawExecution->ResizeOffscreenRenderTex();
		rtvManager->ResizeSwapChain();
	}
}

void Cho::SelectGameProject()
{
	static bool end = false;
	if (end) {
		return;
	}
	{
		// プロジェクトの選択、作成
		if (!startSetting->IsProject()) {
			startSetting->SelectedProject();
		} else
		{
			if (startSetting->IsNew()) {
				SystemState::GetInstance().SetProjectName(startSetting->GetProjectName());
				SystemState::GetInstance().SetProjectRoot(startSetting->GetProjectRoot());

				// 最初のシーンを作成
				sceneManager->ChangeScene("MainScene");

				// 新プロジェクトの時のみ
				/*スターター*/
				StartSetUp();

				//startSetting->CreateProject();

				ScriptProject::GenerateSolutionAndProject();

				// 解放
				startSetting.reset();

				end = true;
			} else {
				SystemState::GetInstance().SetProjectName(startSetting->GetProjectName());
				SystemState::GetInstance().SetProjectRoot(startSetting->GetProjectRoot());

				// 最初のシーンを作成
				sceneManager->ChangeScene("MainScene");

				startSetting->LoadProject(jsonFileLoader.get());

				// 解放
				startSetting.reset();

				end = true;
			}
		}
	}
}
