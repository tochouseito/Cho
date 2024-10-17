#include "EditorManager.h"

// ImGui
#include"imgui.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void EditorManager::Initialize(
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager,
    PrefabManager* prefabManager,
    SceneManager* sceneManager
)
{
	// MainMenu
	mainMenu = std::make_unique<MainMenu>();
	mainMenu->Initialize();

	// FileView
	fileView = std::make_unique<FileView>();
	fileView->Initialize();

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
    ImGui::Begin("AddObject");

    static char nameBuffer[128] = "";  // 名前を入力するバッファ
    static bool inputActive = false;   // 入力がアクティブかどうかを管理するフラグ

    if (ImGui::Button("Input Name")) {
        inputActive = true;  // ボタンを押したら入力を開始
    }

    if (inputActive) {
        ImGui::Begin("Input Name");  // 名前の入力用のウィンドウを作成

        ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));  // 名前を入力するフィールド

        if (ImGui::Button("OK")) {
            std::string name(nameBuffer);
            sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
            inputActive = false;  // 入力終了
        }

        ImGui::SameLine();  // 次のボタンを同じ行に配置する

        if (ImGui::Button("Cancel")) {
            inputActive = false;  // キャンセルして入力終了
        }

        ImGui::End();  // ウィンドウを閉じる
    }

    ImGui::End();

    ImGui::Begin("GameObjectList");
    
    for (auto& pair : sceneManager_->GetGameObjects()) {
        const std::string& name = pair.first;  // マップのキーがオブジェクトの名前と仮定

        if (ImGui::Selectable(name.c_str())) {
            selectedGamaObjectName_ = name;
            selectedGameObject_ = pair.second.get();  // 選択したオブジェクトを保持
        }
    }

    ImGui::End();

    ImGui::Begin("ObjectInfo");
    if (selectedGameObject_) {
        // 名前と EntityID を表示
        ImGui::Text("Name: %s EntityID: %d", selectedGamaObjectName_.c_str(), selectedGameObject_->GetEntityID());
        static bool isAdd = false;
        if (isAdd) {
        } else
        {
            if (ImGui::Button("AddComponent")) {

            }
        }
    }
    ImGui::End();

	// 全体のImGuiウィンドウ
	//UpdateMainWindow();

	// MainMenu
	mainMenu->Update();

	// FileView
	//fileView->Update();

    //ImGuiIO& io = ImGui::GetIO();
    //if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    //{
    //    // ビューポート全体をカバーするドックスペースを作成
    //    ImGuiViewport* viewport = ImGui::GetMainViewport();
    //    ImGui::SetNextWindowPos(viewport->Pos); // 次のウィンドウの位置をメインビューポートの位置に設定
    //    ImGui::SetNextWindowSize(viewport->Size); // 次のウィンドウのサイズをメインビューポートのサイズに設定
    //    ImGui::SetNextWindowViewport(viewport->ID); // ビューポートIDをメインビューポートに設定

    //    // タイトルバーを削除し、リサイズや移動を防止し、背景のみとするウィンドウフラグを設定
    //    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

    //    // ウィンドウの丸みとボーダーをなくして、シームレスなドッキング外観にする
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    //    ImGui::Begin("DockSpace Window", nullptr, window_flags); // ドックスペースとして機能する新しいウィンドウを開始
    //    ImGui::PopStyleVar(2); // 先ほどプッシュしたスタイル変数を2つポップする

    //    // メニューバー
    //    if (ImGui::BeginMenuBar())
    //    {
    //        // ファイルメニューに「Open」と「Save」オプションを追加
    //        if (ImGui::BeginMenu("File"))
    //        {
    //            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
    //            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
    //            ImGui::EndMenu();
    //        }
    //        // 編集メニューの追加（編集アクションはここに追加可能）
    //        if (ImGui::BeginMenu("Edit"))
    //        {
    //            // 編集アクションをここに追加
    //            ImGui::EndMenu();
    //        }
    //        ImGui::EndMenuBar(); // メニューバーを終了
    //    }

    //    // ウィンドウ内にドックスペースを作成
    //    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    //    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    //    ImGui::End(); // ドックスペースウィンドウを終了
    //}

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
