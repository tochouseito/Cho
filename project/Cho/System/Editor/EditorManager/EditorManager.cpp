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
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager,
    PrefabManager* prefabManager,
    SceneManager* sceneManager
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
        this
    );

	// FileView
	fileView = std::make_unique<FileView>();
    fileView->Initialize(this, rvManager, texLoader);

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
        this
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
    
    //// 個別のドッカブルウィンドウのセットアップ
    //ImGui::Begin("Memory Editor", nullptr, ImGuiWindowFlags_NoCollapse);
    //// メモリエディタのUI要素をここに追加
    //ImGui::End(); // メモリエディタウィンドウを終了

    //ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoCollapse);
    //// ツールのUI要素をここに追加
    //ImGui::End(); // ツールウィンドウを終了

    //ImGui::Begin("Dear ImGui Test Engine", nullptr, ImGuiWindowFlags_NoCollapse);
    //// メインシーンビューのUI要素を追加
    //ImGui::Text("Orthographic"); // シーンビューの例としてテキスト要素を追加
    //// ツールバーやビューポートオプションなどの要素を追加
    //ImGui::End(); // テストエンジンウィンドウを終了

    //ImGui::Begin("Stock Prices", nullptr, ImGuiWindowFlags_NoCollapse);
    //// 株価グラフの要素をここに追加
    //ImGui::End(); // 株価ウィンドウを終了

    //// 画像に基づくその他のドッカブルな要素
    //ImGui::Begin("Tools Item Picker", nullptr, ImGuiWindowFlags_NoCollapse);
    //// 「Show windows」や「Show mesh wh」などのオプションを追加
    //ImGui::End(); // ツールアイテムピッカーウィンドウを終了
}

void EditorManager::CreateObject()
{
    std::string name = "NewObject";
    name = sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
    SetSelectedGOName(name);
    SetSelectedGO(sceneManager_->GetGameObject(name));
    // 初期コンポーネントのトランスフォームコンポーネントを付与
    TransformComponent TFComp;
    TFComp.Initialize();
    sceneManager_->GetGameObject(name)->AddComponent(TFComp);
}

void EditorManager::CreateCamera()
{
    std::string name = "NewCamera";
    name = sceneManager_->AddCameraObject(name);
    SetSelectedGOName(name);
    SetSelectedGO(sceneManager_->GetCameraObject(name));
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
