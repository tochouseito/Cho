#include "PrecompiledHeader.h"
#include "EditorManager.h"

// ImGui
#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void EditorManager::Initialize(
    ResourceViewManager* rvManager,
    RTVManager* rtvManager,
    DrawExecution* drawExe,
    TextureLoader* texLoader,
    ModelLoader* modelLoader,
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager,
    PrefabManager* prefabManager,
    SceneManager* sceneManager,
    ScriptManager* scriptManager
)
{
	// MainMenu
	mainMenu = std::make_unique<MainMenu>();
	mainMenu->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager,
        this
        );

    // SceneView
    sceneView = std::make_unique<SceneView>();
    sceneView->Initialize(drawExe->GetDebugRenTexInd(), rvManager);

    // PopupMenu
    popupMenu = std::make_unique<PopupMenu>();
    popupMenu->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager,
        this,
        scriptManager
    );

	// FileView
	fileView = std::make_unique<FileView>();
    fileView->Initialize(this, rvManager, texLoader,modelLoader);

    // InfoView
    infoView = std::make_unique<InfoView>();
    infoView->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager,
        this,
        scriptManager
    );

    // ObjectsList
    objectsList = std::make_unique<ObjectsList>();
    objectsList->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager,
        this
    );

    toolBar = std::make_unique<ToolBar>();
    toolBar->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager,
        this,
        scriptManager
    );

    // D3D12
    rvManager_ = rvManager;
    rtvManager_ = rtvManager;
    drawExe_ = drawExe;

    // ECS
    entityManager_ = entityManager;
    componentManager_ = componentManager;
    systemManager_ = systemManager;
    prefabManager_ = prefabManager;

    // SceneManager
    sceneManager_ = sceneManager;

    // ScriptManager
    scriptManager_ = scriptManager;

    // デバッグカメラを作成
    debugCamera = std::make_unique<DebugCamera>();
    debugCamera->Initialize(systemManager);
}

void EditorManager::Update()
{
    // 全体のImGuiウィンドウ
    //UpdateMainWindow();

    // MainMenu
    mainMenu->Update();

    // SceneView
    sceneView->Update(drawExe_->GetRenderTexIndex());

    // PopupMenu
    popupMenu->Update(sceneView->IsWindowHovered());

    // FileView
    fileView->Update();

    // InfoView
    infoView->Update();

    // ObjectsList
    objectsList->Update();

    // ToolBar
    toolBar->Update();

    // DebugCamera
    //debugCamera->Update();
}

void EditorManager::CreateObject()
{
    std::string name = "NewObject";
    name = sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
    SetSelectedGOName(name);
    SetSelectedGO(sceneManager_->GetGameObject(name));
    // 初期コンポーネントのトランスフォームコンポーネントを付与
    TransformComponent TFComp;
    sceneManager_->GetGameObject(name)->AddComponent(TFComp);
}

void EditorManager::CreateCamera()
{
    std::string name = "NewCamera";
    name = sceneManager_->AddCameraObject(name);
    SetSelectedGOName(name);
    SetSelectedGO(sceneManager_->GetCameraObject(name));
}

void EditorManager::UpdateFiles()
{
    fileView->UpdateFiles();
}

void EditorManager::UpdateMainWindow()
{
	// ウィンドウの位置を指定する
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	// ウィンドウのサイズを指定する
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

	// フラグでウィンドウがフォーカスされても前面に持ってこないように設定
	ImGui::Begin(
		"Cho Editor",
		nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
	);
	// ウィンドウの中に表示するUI要素
	ImGui::End();
}
