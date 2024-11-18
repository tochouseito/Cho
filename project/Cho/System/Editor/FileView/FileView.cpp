#include "PrecompiledHeader.h"
#include"FileView.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Load/TextureLoader/TextureLoader.h"
#include"imgui.h"
#include"Editor/EditorManager/EditorManager.h"

void FileView::Initialize(EditorManager* editManager, ResourceViewManager* rvManager, TextureLoader* texLoader)
{
    rvManager_ = rvManager;
    texLoader_ = texLoader;
    editManager_ = editManager;

    currentDirectory = fs::current_path().string();  // 現在のディレクトリに設定
    currentDirectory = currentDirectory + "\\Game\\Assets\\Texture";
    files = GetFilesInDirectory(currentDirectory);
    selectedFile = "";
}

// 毎フレーム呼ばれる更新処理
void FileView::Update() {

    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("File Browser",nullptr,windowFlags);

    ShowFileBrowserWithDirectories();
    
    ImGui::End();
}

// 指定されたディレクトリ内のファイルを取得
std::vector<std::string> FileView::GetFilesInDirectory(const std::string& directory) {
    std::vector<std::string> fileList;
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            fileList.push_back(entry.path().string());
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return fileList;
}
// ディレクトリ対応のファイルブラウザを表示
void FileView::ShowFileBrowserWithDirectories() {

    // 親ディレクトリに戻るボタン
    if (ImGui::Button("..") && currentDirectory != fs::path(currentDirectory).root_path().string())
    {
        currentDirectory = fs::path(currentDirectory).parent_path().string();
        files = GetFilesInDirectory(currentDirectory);  // ディレクトリの内容を更新
    }

    std::string newDirectory;  // 新しいディレクトリの移動先
    bool shouldChangeDirectory = false;

    const float iconSize = 64.0f;   // アイコンのサイズ
    int itemsPerRow = 4;            // 1行に表示するアイコンの数
    int itemIndex = 0;              // アイテムのインデックス

    // ディレクトリとファイルをアイコンで表示
    for (const auto& file : files)
    {
        fs::path filePath(file);
        std::string fileName = filePath.filename().string();  // ファイル名のみを取得

        // 1行に表示するアイコン数に合わせて配置
        if (itemIndex > 0 && itemIndex % itemsPerRow != 0)
        {
            ImGui::SameLine();
        }

        ImGui::BeginGroup(); // アイコンとファイル名を一つのグループとしてまとめる

        if (fs::is_directory(filePath))
        {
            // ディレクトリ用アイコン (単純にボタンとして表示)
            if (ImGui::Button((fileName + "/").c_str(), ImVec2(iconSize, iconSize)))
            {
                newDirectory = filePath.string();  // 新しいディレクトリパスを保存
                shouldChangeDirectory = true;      // ディレクトリ変更フラグを設定
            }
        } else
        {
            // ファイル用アイコンの表示
            auto handle = rvManager_->GetHandle(texLoader_->GetTexture(fileName).rvIndex).GPUHandle;

            if (handle.ptr != 0)
            {
                // テクスチャアイコンを表示
                ImGui::Image((void*)(intptr_t)handle.ptr, ImVec2(iconSize, iconSize));

                // アイコンをクリックした際の処理
                if (ImGui::IsItemClicked())
                {
                    selectedFile = filePath.string();  // フルパスでファイルを選択
                }
            } else
            {
                // テクスチャがない場合はボタンを代替表示
                if (ImGui::Button(fileName.c_str(), ImVec2(iconSize, iconSize)))
                {
                    selectedFile = filePath.string();  // ファイルを選択
                }
            }
        }

        // アイコンの下にファイル名を表示
        ImGui::TextWrapped("%s", fileName.c_str());

        ImGui::EndGroup(); // グループを終了

        itemIndex++;
    }

    // ディレクトリ変更が必要なら処理を行う
    if (shouldChangeDirectory)
    {
        currentDirectory = newDirectory;
        files = GetFilesInDirectory(currentDirectory);  // ディレクトリの内容を更新
    }

}
