#include "MainMenu.h"
#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void MainMenu::Initialize(ResourceViewManager* rvManager, RTVManager* rtvManager, DrawExecution* drawExe, EntityManager* entityManager, ComponentManager* componentManager, SystemManager* systemManager, PrefabManager* prefabManager, SceneManager* sceneManager)
{
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

void MainMenu::Update()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        BackWindow();
    }
}

void MainMenu::BackWindow()
{
    // ビューポート全体をカバーするドックスペースを作成
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos); // 次のウィンドウの位置をメインビューポートの位置に設定
    ImGui::SetNextWindowSize(viewport->Size); // 次のウィンドウのサイズをメインビューポートのサイズに設定
    ImGui::SetNextWindowViewport(viewport->ID); // ビューポートIDをメインビューポートに設定

    // タイトルバーを削除し、リサイズや移動を防止し、背景のみとするウィンドウフラグを設定
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar;

    // ウィンドウの丸みとボーダーをなくして、シームレスなドッキング外観にする
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpace Window", nullptr, window_flags); // ドックスペースとして機能する新しいウィンドウを開始
    ImGui::PopStyleVar(2); // 先ほどプッシュしたスタイル変数を2つポップする

    // メニューバー
    MenuBar();

    // ウィンドウ内にドックスペースを作成
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End(); // ドックスペースウィンドウを終了
}

void MainMenu::MenuBar()
{
    // メニューバー
    if (ImGui::BeginMenuBar()) {

        // ファイルメニュー
        FileMenu();

        // 編集メニュー
        EditMenu();

        // レイアウトメニューの追加(Imguiのレイアウト)
        LayoutMenu();

        // Engine情報を表示
        EngineInfoMenu();

        ImGui::EndMenuBar(); // メニューバーを終了
    }
}

void MainMenu::FileMenu()
{
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
        ImGui::EndMenu();
    }
}

void MainMenu::EditMenu()
{
    if (ImGui::BeginMenu("Edit")) {
        // 編集アクションをここに追加
        ImGui::EndMenu();
    }
}

void MainMenu::LayoutMenu()
{
    if (ImGui::BeginMenu("Layout")) {
        // スタイルエディタを表示
        ImGui::ShowStyleEditor();

        ImGui::EndMenu();
    }
}

void MainMenu::EngineInfoMenu()
{
    if (ImGui::BeginMenu("EngineInfo")) {

        // Viewの使用数
        ImGui::SeparatorText("Descriptor");

        ImGui::Text("S.U.View : %d / %d", rvManager_->GetNowIndex(), ResourceViewManager::GetMaxIndex());
        ImGui::Text("RTV : %d / %d", rtvManager_->GetNowIndex(), RTVManager::GetMaxIndex());


        ImGui::EndMenu();
    }
}
