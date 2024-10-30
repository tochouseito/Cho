#include "JsonFileLoader.h"

#include"imgui.h"

void JsonFileLoader::Initialize()
{
}

void JsonFileLoader::SaveStyle(const char* filename)
{
    ImGuiStyle& style = ImGui::GetStyle();
    nlohmann::json j;

    // ImGuiの全ての色をJSONオブジェクトに保存
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        ImVec4& color = style.Colors[i];
        j["Colors"][i] = { color.x, color.y, color.z, color.w };
    }
    // ウィンドウの角の丸みの設定も保存
    j["WindowRounding"] = style.WindowRounding;

    // ファイルに書き込む
    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); // インデント幅4でJSONを整形して保存
        file.close();
    }
}

void JsonFileLoader::LoadStyle(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return; // ファイルが開けない場合は何もしない

    nlohmann::json j;
    file >> j; // ファイルからJSONデータを読み込む
    file.close();

    ImGuiStyle& style = ImGui::GetStyle();
    // Colorsが含まれている場合、各色を設定
    if (j.contains("Colors")) {
        for (int i = 0; i < ImGuiCol_COUNT && i < j["Colors"].size(); ++i) {
            auto& color = j["Colors"][i];
            style.Colors[i] = ImVec4(color[0], color[1], color[2], color[3]);
        }
    }
    // WindowRoundingが含まれている場合、角の丸みを設定
    if (j.contains("WindowRounding")) {
        style.WindowRounding = j["WindowRounding"];
    }
}

void JsonFileLoader::SaveStyleToProject()
{
    std::filesystem::path styleDir = "Cho/Resources/ImGuiStyle";
    // ディレクトリが存在しない場合は作成
    if (!std::filesystem::exists(styleDir)) {
        std::filesystem::create_directories(styleDir);
    }
    // スタイルファイルのパスを設定
    std::filesystem::path styleFile = styleDir / "style.json";
    SaveStyle(styleFile.string().c_str()); // スタイルを保存
}

void JsonFileLoader::LoadStyleFromProject()
{
    std::filesystem::path styleFile = "Cho/Resources/ImGuiStyle/style.json";
    // スタイルファイルが存在する場合は読み込む
    if (std::filesystem::exists(styleFile)) {
        LoadStyle(styleFile.string().c_str());
    } else {
        // スタイルファイルが存在しない場合、新規に作成してデフォルトスタイルを保存
        SaveStyle(styleFile.string().c_str());
    }
}
