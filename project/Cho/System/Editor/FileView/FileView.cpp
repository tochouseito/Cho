#include"FileView.h"
#include"imgui.h"

void FileView::Initialize()
{
    currentDirectory = fs::current_path().string();  // 現在のディレクトリに設定
    files = GetFilesInDirectory(currentDirectory);
    selectedFile = "";
}

// 毎フレーム呼ばれる更新処理
void FileView::Update() {
    ShowFileBrowserWithDirectories();
    ShowFileContent();
}

// ファイルブラウザを表示
void FileView::ShowFileBrowser() {
    ImGui::Begin("File Browser");

    for (const auto& file : files) {
        if (ImGui::Selectable(file.c_str())) {
            selectedFile = file;  // ファイルを選択
        }
    }

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

// 指定されたファイルの内容を読み込み
std::string FileView::ReadFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "Error: Could not open file.";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 選択されたファイルの内容を表示
void FileView::ShowFileContent() {
    ImGui::Begin("File Content");

    if (!selectedFile.empty()) {
        std::string content = ReadFileContent(selectedFile);
        ImGui::TextUnformatted(content.c_str());
    } else {
        ImGui::Text("No file selected.");
    }

    ImGui::End();
}

// ディレクトリ対応のファイルブラウザを表示
void FileView::ShowFileBrowserWithDirectories() {
    ImGui::Begin("File Browser");

    // 親ディレクトリに戻るボタン
    if (ImGui::Button("..") && currentDirectory != "/") {
        currentDirectory = fs::path(currentDirectory).parent_path().string();
        files = GetFilesInDirectory(currentDirectory);  // ディレクトリの内容を更新
    }

    std::string newDirectory;  // 新しいディレクトリの移動先
    bool shouldChangeDirectory = false;

    // ディレクトリとファイルをリスト表示
    for (const auto& file : files) {
        fs::path filePath(file);  // fs::pathを使ってファイルパスを扱う

        if (fs::is_directory(filePath)) {
            // ディレクトリの表示と選択処理
            if (ImGui::Selectable((filePath.filename().string() + "/").c_str())) {
                newDirectory = filePath.string();  // 新しいディレクトリパスを保存
                shouldChangeDirectory = true;  // ディレクトリ変更フラグを設定
            }
        } else {
            // ファイルの表示と選択処理
            if (ImGui::Selectable(filePath.filename().string().c_str())) {
                selectedFile = filePath.string();  // ファイルを選択
            }
        }
    }

    // ディレクトリ変更が必要なら処理を行う
    if (shouldChangeDirectory) {
        currentDirectory = newDirectory;
        files = GetFilesInDirectory(currentDirectory);  // ディレクトリの内容を更新
    }

    ImGui::End();
}