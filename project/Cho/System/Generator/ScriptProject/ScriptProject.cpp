#include "PrecompiledHeader.h"
#include "ScriptProject.h"
#include <windows.h>

void ScriptProject::GenerateScriptProject(const std::string& projectName, const std::string& outputPath) {
    std::string slnPath = outputPath + "/" + projectName + ".sln";
    std::string vcxprojPath = outputPath + "/" + projectName + ".vcxproj";
    std::string filtersPath = vcxprojPath + ".filters";

    // スクリプト用ディレクトリを作成
    //fs::create_directories(outputPath + "/Scripts");

    // .sln ファイルの生成または更新
    if (!fs::exists(slnPath)) {
        std::ofstream slnFile(slnPath);
        slnFile << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
        slnFile << "# Visual Studio Version 17\n";
        slnFile << "VisualStudioVersion = 17.0.31903.59\n";
        slnFile << "MinimumVisualStudioVersion = 10.0.40219.1\n";
        slnFile << "Project(\"{GUID}\") = \"" << projectName << "\", \"Scripts\\" << projectName << ".vcxproj\", \"{GUID2}\"\n";
        slnFile << "EndProject\n";
        slnFile.close();
        std::cout << "Generated solution file: " << slnPath << "\n";
    } else {
        std::cout << "Solution file already exists: " << slnPath << "\n";
    }

    // .vcxproj ファイルの生成または更新
    if (!fs::exists(vcxprojPath)) {
        std::ofstream vcxFile(vcxprojPath);
        vcxFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        vcxFile << "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
        vcxFile << "  <ItemGroup>\n";
        vcxFile << "    <ClCompile Include=\"NewScript.cpp\" />\n";
        vcxFile << "  </ItemGroup>\n";
        vcxFile << "  <ItemGroup>\n";
        vcxFile << "    <ClInclude Include=\"NewScript.h\" />\n";
        vcxFile << "  </ItemGroup>\n";
        vcxFile << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n";
        vcxFile << "  <PropertyGroup Label=\"Globals\">\n";
        vcxFile << "    <ProjectGuid>{GUID2}</ProjectGuid>\n";
        vcxFile << "    <Keyword>Win32Proj</Keyword>\n";
        vcxFile << "  </PropertyGroup>\n";
        vcxFile << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n";
        vcxFile << "</Project>\n";
        vcxFile.close();
        std::cout << "Generated project file: " << vcxprojPath << "\n";
    } else {
        std::cout << "Project file already exists: " << vcxprojPath << "\n";
    }

    // .filters ファイルの生成
    if (!fs::exists(filtersPath)) {
        std::ofstream filtersFile(filtersPath);
        filtersFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
        filtersFile << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
        filtersFile << "  <ItemGroup>\n";
        filtersFile << "    <ClCompile Include=\"NewScript.cpp\">\n";
        filtersFile << "      <Filter>Source Files</Filter>\n";
        filtersFile << "    </ClCompile>\n";
        filtersFile << "    <ClInclude Include=\"NewScript.h\">\n";
        filtersFile << "      <Filter>Header Files</Filter>\n";
        filtersFile << "    </ClInclude>\n";
        filtersFile << "  </ItemGroup>\n";
        filtersFile << "</Project>\n";
        filtersFile.close();
        std::cout << "Generated filters file: " << filtersPath << "\n";
    }
}

void ScriptProject::GenerateScriptTemplate(const std::string& scriptName, const std::string& outputPath) {
    std::string cppPath = outputPath + "/Scripts/" + scriptName + ".cpp";
    std::string hPath = outputPath + "/Scripts/" + scriptName + ".h";

    if (!fs::exists(cppPath)) {
        std::ofstream cppFile(cppPath);
        cppFile << "#include \"" << scriptName << ".h\"\n\n";
        cppFile << "void " << scriptName << "::OnStart() {\n    // Initialization\n}\n\n";
        cppFile << "void " << scriptName << "::OnUpdate() {\n    // Update logic\n}\n";
        cppFile.close();
        std::cout << "Generated CPP file: " << cppPath << "\n";
    }

    if (!fs::exists(hPath)) {
        std::ofstream hFile(hPath);
        hFile << "#pragma once\n\n";
        hFile << "#include \"YourEngine/Component.h\"\n\n";
        hFile << "class " << scriptName << " {\n";
        hFile << "public:\n";
        hFile << "    void OnStart();\n";
        hFile << "    void OnUpdate();\n";
        hFile << "};\n";
        hFile.close();
        std::cout << "Generated header file: " << hPath << "\n";
    }
}

void ScriptProject::OpenVisualStudio() {
    // ソリューションファイルを検索
    std::string slnPath = FindSolutionPath();

    if (fs::exists(slnPath)) {
        // Visual Studio の実行ファイルパス
        std::string vsPath = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe";

        // ShellExecuteA を使用してプロセスを起動
        HINSTANCE result = ShellExecuteA(
            nullptr,           // 親ウィンドウのハンドル（nullptrでデフォルト）
            "open",            // 動作 ("open" でファイルを開く)
            vsPath.c_str(),    // 実行するファイル
            slnPath.c_str(),   // コマンドライン引数
            nullptr,           // 作業ディレクトリ（nullptrでカレントディレクトリ）
            SW_SHOWNORMAL      // ウィンドウ表示スタイル
        );

        if ((uintptr_t)result <= 32) {
            std::cerr << "Failed to open Visual Studio. Error code: " << (uintptr_t)result << "\n";
            DWORD error = GetLastError(); // Windowsエラーコードを取得
            std::cerr << "System error code: " << error << "\n";
        } else {
            std::cout << "Opened Visual Studio with solution: " << slnPath << "\n";
        }
    } else {
        std::cerr << "Solution file does not exist: " << slnPath << "\n";
    }
}

void ScriptProject::SyncScriptsWithProject(const std::string& projectName, const std::string& outputPath) {
    std::string vcxprojPath = outputPath + "/Assets/Scripts/" + projectName + ".vcxproj";
    std::string filtersPath = vcxprojPath + ".filters";
    std::string scriptsPath = outputPath + "/Assets/Scripts";

    // スクリプトディレクトリをスキャン
    std::set<std::string> cppFiles;
    std::set<std::string> hFiles;

    for (const auto& entry : fs::directory_iterator(scriptsPath)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (path.extension() == ".cpp") {
                cppFiles.insert(path.filename().string());
            } else if (path.extension() == ".h") {
                hFiles.insert(path.filename().string());
            }
        }
    }

    vcxprojPath = outputPath + "/" + projectName + ".vcxproj";
    filtersPath = vcxprojPath + ".filters";

    // プロジェクトファイルを更新
    UpdateVcxproj(vcxprojPath, cppFiles, hFiles);

    // フィルターファイルを更新
    UpdateFilters(filtersPath, cppFiles, hFiles);
}

void ScriptProject::UpdateVcxproj(const std::string& vcxprojPath, const std::set<std::string>& cppFiles, const std::set<std::string>& hFiles) {
    std::ofstream vcxFile(vcxprojPath, std::ios::trunc); // 上書きモードで開く
    vcxFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    vcxFile << "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";

    // ソースファイルの登録
    vcxFile << "  <ItemGroup>\n";
    for (const auto& file : cppFiles) {
        vcxFile << "    <ClCompile Include=\"" << file << "\" />\n";
    }
    vcxFile << "  </ItemGroup>\n";

    // ヘッダーファイルの登録
    vcxFile << "  <ItemGroup>\n";
    for (const auto& file : hFiles) {
        vcxFile << "    <ClInclude Include=\"" << file << "\" />\n";
    }
    vcxFile << "  </ItemGroup>\n";

    vcxFile << "</Project>\n";
    vcxFile.close();
    std::cout << "Updated project file: " << vcxprojPath << "\n";
}

void ScriptProject::UpdateFilters(const std::string& filtersPath, const std::set<std::string>& cppFiles, const std::set<std::string>& hFiles) {
    std::ofstream filtersFile(filtersPath, std::ios::trunc); // 上書きモードで開く
    filtersFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    filtersFile << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";

    // ソースファイルのフィルター登録
    filtersFile << "  <ItemGroup>\n";
    for (const auto& file : cppFiles) {
        filtersFile << "    <ClCompile Include=\"" << file << "\">\n";
        filtersFile << "      <Filter>Source Files</Filter>\n";
        filtersFile << "    </ClCompile>\n";
    }
    filtersFile << "  </ItemGroup>\n";

    // ヘッダーファイルのフィルター登録
    filtersFile << "  <ItemGroup>\n";
    for (const auto& file : hFiles) {
        filtersFile << "    <ClInclude Include=\"" << file << "\">\n";
        filtersFile << "      <Filter>Header Files</Filter>\n";
        filtersFile << "    </ClInclude>\n";
    }
    filtersFile << "  </ItemGroup>\n";

    filtersFile << "</Project>\n";
    filtersFile.close();
    std::cout << "Updated filters file: " << filtersPath << "\n";
}

std::string ScriptProject::FindSolutionPath() {
    // 現在の作業ディレクトリを取得
    std::string currentPath = fs::current_path().string();
    std::string gameFolder = currentPath + "\\Game";

    // Gameフォルダ内を検索
    for (const auto& entry : fs::directory_iterator(gameFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".sln") {
            return entry.path().string(); // 最初に見つかった .sln ファイルを返す
        }
    }

    // 見つからなかった場合
    return "";
}
