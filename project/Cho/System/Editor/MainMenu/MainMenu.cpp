#include "PrecompiledHeader.h"
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

    // ポップアップウィンドウ
    PopUp();
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
    if (ImGui::BeginMenu("Edit"))
    {
        // 編集アクションをここに追加

        // 「Add」メニュー項目の追加
        if (ImGui::BeginMenu("Add"))
        {
            // 「3DObject」サブメニューを追加
            if (ImGui::BeginMenu("3DObject"))
            {
                // 「3DObject」メニューの横にさらにリスト表示
                if (ImGui::MenuItem("BaseObject")) {
                    // Cubeの処理
                    popWindow = PopType::BaseObject;
                }
                //if (ImGui::MenuItem("Sphere")) {
                //    // Sphereの処理
                //    popWindow = PopType::Add3DObject;
                //}
                //if (ImGui::MenuItem("Cylinder")) {
                //    // Cylinderの処理
                //    popWindow = PopType::Add3DObject;
                //}

                ImGui::EndMenu(); // 「3DObject」サブメニューを終了
            }

            // 他のメニュー項目の追加
            if (ImGui::MenuItem("Item 2")) { /* Item 2の処理 */ }
            if (ImGui::MenuItem("Item 3")) { /* Item 3の処理 */ }

            ImGui::EndMenu(); // 「Add」メニューを終了
        }

        ImGui::EndMenu(); // 「Edit」メニューを終了
    }
}

void MainMenu::LayoutMenu()
{
    static bool showStyleEditor = false;

    if (ImGui::BeginMenu("Layout")) {
        if (ImGui::MenuItem("Show Style Editor")) {
            showStyleEditor = true;
        }
        ImGui::EndMenu();
    }

    if (showStyleEditor) {
        ImGui::Begin("Style Editor", &showStyleEditor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
}

void MainMenu::EngineInfoMenu()
{
    if (ImGui::BeginMenu("EngineInfo")) {

        // Viewの使用数
        ImGui::SeparatorText("Descriptor");

        ImGui::Text("S.U.View : %d / %d", rvManager_->GetNowIndex(), ResourceViewManager::GetMaxIndex());
        ImGui::Text("RTV : %d / %d", rtvManager_->GetNowIndex(), RTVManager::GetMaxIndex());
        // フレームレートを表示
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("fps: %.1f", NowFrameRate());
        ImGui::Text("DeltaTime: %.5f", DeltaTime());

        ImGui::EndMenu();
    }
}

void MainMenu::Add()
{
    ImGui::OpenPopup("Input Name");
    static char nameBuffer[128] = "";  // 名前を入力するバッファ

    // モーダルウィンドウの表示処理
    if (ImGui::BeginPopupModal("Input Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));  // 名前を入力するフィールド

        if (ImGui::Button("OK")) {
            std::string name(nameBuffer);
            sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
            memset(nameBuffer, 0, sizeof(nameBuffer));// バッファクリア
            switch (popWindow)
            {
            case None:
                // ここに来るはずはない
                break;
            case BaseObject:
                // 初期コンポーネントのトランスフォームコンポーネントを付与
                TransformComponent TFComp;
                TFComp.Initialize();
                sceneManager_->GetGameObject(name)->AddComponent(TFComp);
                break;
            }
            popWindow = PopType::None;
            ImGui::CloseCurrentPopup();  // ポップアップを閉じる
        }

        ImGui::SameLine();  // 次のボタンを同じ行に配置する

        if (ImGui::Button("Cancel")) {
            popWindow = PopType::None;
            ImGui::CloseCurrentPopup();  // ポップアップを閉じる
        }

        ImGui::EndPopup(); // モーダルポップアップを終了
    }
}

void MainMenu::PopUp()
{
    switch (popWindow)
    {
    case None:

        // 何もしない
        break;

    case BaseObject:

        Add();

        break;
    default:
        break;
    }
}
