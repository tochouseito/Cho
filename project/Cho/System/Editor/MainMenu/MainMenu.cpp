#include "MainMenu.h"
#include"imgui.h"

void MainMenu::Initialize()
{

}

void MainMenu::Update()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
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
        if (ImGui::BeginMenuBar())
        {
            // ファイルメニューに「Open」と「Save」オプションを追加
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Open action */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Save action */ }
                ImGui::EndMenu();
            }
            // 編集メニューの追加（編集アクションはここに追加可能）
            if (ImGui::BeginMenu("Edit"))
            {
                // 編集アクションをここに追加
                ImGui::EndMenu();
            }
            // レイアウトメニューの追加(Imguiのレイアウト)
            if (ImGui::BeginMenu("Layout"))
            {
                // スタイルエディタを表示
                ImGui::ShowStyleEditor();

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar(); // メニューバーを終了
        }

        // ウィンドウ内にドックスペースを作成
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        ImGui::End(); // ドックスペースウィンドウを終了

    }
}
