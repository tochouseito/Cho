#include "PrecompiledHeader.h"
#include "ScriptProject.h"

std::unordered_map<std::string, std::vector<std::string>> ScriptProject::scripts;
HMODULE ScriptProject::scriptLibrary = nullptr;

void ScriptProject::Update(const std::string& scriptsPath) {

    std::string fullScriptsPath = fs::absolute(scriptsPath).string();

    // Scripts フォルダをスキャンしてリストを更新
    scripts["cpp"] = {};
    scripts["h"] = {};

    for (const auto& entry : fs::directory_iterator(fullScriptsPath)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (path.extension() == ".cpp") {
                scripts["cpp"].push_back(path.string());
            } else if (path.extension() == ".h") {
                scripts["h"].push_back(path.string());
            }
        }
    }

    std::cout << "Updated script list:\n";
    for (const auto& [type, files] : scripts) {
        std::cout << type << " files:\n";
        for (const auto& file : files) {
            std::cout << "  " << file << "\n";
        }
    }
}

void ScriptProject::GenerateSolutionAndProject(const std::string& projectName, const std::string& outputPath) {
    std::string slnPath = outputPath + "/" + projectName + ".sln";
    std::string vcxprojPath = outputPath + "/" + projectName + ".vcxproj";
    std::string filtersPath = vcxprojPath + ".filters";

    slnPath = fs::absolute(slnPath).string();
    vcxprojPath = fs::absolute(vcxprojPath).string();
    filtersPath = fs::absolute(filtersPath).string();

    std::string slnGUID = GenerateGUID();
    std::string projectGUID = GenerateGUID();

    // ソリューションファイルの生成
    if (!fs::exists(slnPath)) {
        std::ofstream slnFile(slnPath, std::ios::trunc);
        slnFile << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
        slnFile << "# Visual Studio Version 17\n";
        slnFile << "VisualStudioVersion = 17.0.31903.59\n";
        slnFile << "MinimumVisualStudioVersion = 10.0.40219.1\n";

        slnFile << "Project(\"" << slnGUID << "\") = \"" << projectName << "\", \"" << projectName << ".vcxproj\", \"" << projectGUID << "\"\n";
        slnFile << "EndProject\n";

        // ソリューション構成の追加
        slnFile << "Global\n";
        slnFile << "    GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
        slnFile << "        Debug|x64 = Debug|x64\n";
        slnFile << "        Release|x64 = Release|x64\n";
        slnFile << "    EndGlobalSection\n";
        slnFile << "    GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
        slnFile << "        " << projectGUID << ".Debug|x64.ActiveCfg = Debug|x64\n";
        slnFile << "        " << projectGUID << ".Debug|x64.Build.0 = Debug|x64\n";
        slnFile << "        " << projectGUID << ".Release|x64.ActiveCfg = Release|x64\n";
        slnFile << "        " << projectGUID << ".Release|x64.Build.0 = Release|x64\n";
        slnFile << "    EndGlobalSection\n";
        slnFile << "    GlobalSection(SolutionProperties) = preSolution\n";
        slnFile << "        HideSolutionNode = FALSE\n";
        slnFile << "    EndGlobalSection\n";
        slnFile << "EndGlobal\n";

        slnFile.close();
        std::cout << "Generated solution file: " << slnPath << "\n";
    } else {
        std::cout << "Solution file already exists: " << slnPath << "\n";
    }

    UpdateVcxproj(vcxprojPath,projectGUID,projectName);
    UpdateFilters(filtersPath);
}

void ScriptProject::OpenVisualStudio() {
    std::string slnPath = FindSolutionPath();
    if (fs::exists(slnPath)) {
        std::string vsPath = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe";
        HINSTANCE result = ShellExecuteA(nullptr, "open", vsPath.c_str(), slnPath.c_str(), nullptr, SW_SHOWNORMAL);
        if ((uintptr_t)result <= 32) {
            std::cerr << "Failed to open Visual Studio. Error code: " << (uintptr_t)result << "\n";
        } else {
            std::cout << "Opened Visual Studio with solution: " << slnPath << "\n";
        }
    } else {
        std::cerr << "Solution file not found: " << slnPath << "\n";
    }
}

void ScriptProject::GenerateScriptTemplate(const std::string& scriptName, const std::string& outputPath) {
    std::string cppPath = outputPath + "/Scripts/" + scriptName + ".cpp";
    std::string hPath = outputPath + "/Scripts/" + scriptName + ".h";

    // フルパスに変換
    cppPath = fs::absolute(cppPath).string();
    hPath = fs::absolute(hPath).string();

    // すでにスクリプトファイルが存在する場合はスキップ
    if (fs::exists(cppPath)) {
        std::cout << "CPP file already exists: " << cppPath << "\n";
    } else {
        std::ofstream cppFile(cppPath);
        cppFile << "#include \"" << scriptName << ".h\"\n\n";
        cppFile << "void " << scriptName << "::Start() {\n    // Initialization\n}\n\n";
        cppFile << "void " << scriptName << "::Update() {\n    // Update logic\n}\n";
        cppFile.close();
        std::cout << "Generated CPP file: " << cppPath << "\n";
    }

    if (fs::exists(hPath)) {
        std::cout << "Header file already exists: " << hPath << "\n";
    } else {
        std::ofstream hFile(hPath);
        hFile << "#pragma once\n\n";
        hFile << "class " << scriptName << " {\n";
        hFile << "public:\n";
        hFile << "    void Start();\n";
        hFile << "    void Update();\n";
        hFile << "};\n";
        hFile.close();
        std::cout << "Generated header file: " << hPath << "\n";
    }
}

void ScriptProject::LoadScriptDLL(const std::string& dllPath) {
    if (scriptLibrary) {
        FreeLibrary(scriptLibrary);  // 既存のDLLをアンロード
    }

    scriptLibrary = LoadLibraryA(dllPath.c_str());
    if (!scriptLibrary) {
        std::cerr << "Failed to load DLL: " << dllPath << "\n";
        return;
    }

    // DLL内の関数を取得して実行
    typedef void (*ScriptInitFunc)();
    ScriptInitFunc initFunc = (ScriptInitFunc)GetProcAddress(scriptLibrary, "Init");
    if (initFunc) {
        initFunc();
    } else {
        std::cerr << "Init function not found in DLL: " << dllPath << "\n";
    }
}

std::string ScriptProject::GenerateGUID() {
    GUID guid;
    HRESULT hr;
    hr=CoCreateGuid(&guid);
    if (FAILED(hr)) {
        std::cerr << "Failed to generate GUID. Error code: " << hr << "\n";
        return "";
    }

    std::ostringstream oss;
    oss << std::hex << std::uppercase;
    oss << "{"
        << std::setw(8) << std::setfill('0') << guid.Data1 << "-"
        << std::setw(4) << std::setfill('0') << guid.Data2 << "-"
        << std::setw(4) << std::setfill('0') << guid.Data3 << "-";

    for (int i = 0; i < 2; ++i) {
        oss << std::setw(2) << std::setfill('0') << static_cast<int>(guid.Data4[i]);
    }
    oss << "-";
    for (int i = 2; i < 8; ++i) {
        oss << std::setw(2) << std::setfill('0') << static_cast<int>(guid.Data4[i]);
    }
    oss << "}";

    return oss.str();
}

void ScriptProject::UpdateVcxproj(const std::string& vcxprojPath, const std::string& projectGuid, const std::string& projectName) {
    std::vector<std::string> scriptFiles;
    scriptFiles.insert(scriptFiles.end(), scripts["cpp"].begin(), scripts["cpp"].end());
    scriptFiles.insert(scriptFiles.end(), scripts["h"].begin(), scripts["h"].end());

    fs::path currentPath = fs::current_path();
    fs::path mathPath = "Cho/Utility/ChoMath";
    fs::path fullMathPath = currentPath / mathPath;
    fullMathPath.make_preferred();

    std::ofstream vcxFile(vcxprojPath, std::ios::trunc);
    vcxFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    vcxFile << "<Project DefaultTargets=\"Build\" ToolsVersion=\"17.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";

    // プロジェクト構成の定義
    vcxFile << "  <ItemGroup Label=\"ProjectConfigurations\">\n";
    vcxFile << "    <ProjectConfiguration Include=\"Debug|x64\">\n";
    vcxFile << "      <Configuration>Debug</Configuration>\n";
    vcxFile << "      <Platform>x64</Platform>\n";
    vcxFile << "    </ProjectConfiguration>\n";
    vcxFile << "    <ProjectConfiguration Include=\"Release|x64\">\n";
    vcxFile << "      <Configuration>Release</Configuration>\n";
    vcxFile << "      <Platform>x64</Platform>\n";
    vcxFile << "    </ProjectConfiguration>\n";
    vcxFile << "  </ItemGroup>\n";

    // グローバルプロパティ
    vcxFile << "  <PropertyGroup Label=\"Globals\">\n";
    vcxFile << "    <ProjectGuid>" << projectGuid << "</ProjectGuid>\n";
    vcxFile << "    <Keyword>Win32Proj</Keyword>\n";
    vcxFile << "    <RootNamespace>" << projectName << "</RootNamespace>\n";
    vcxFile << "  </PropertyGroup>\n";

    vcxFile << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n";

    // Debug 構成
    vcxFile << "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">\n";
    vcxFile << "    <ConfigurationType>DynamicLibrary</ConfigurationType>\n";
    vcxFile << "    <UseDebugLibraries>true</UseDebugLibraries>\n";
    vcxFile << "    <PlatformToolset>v143</PlatformToolset>\n"; // Visual Studio 2022 のツールセット
    vcxFile << "    <CharacterSet>Unicode</CharacterSet>\n";
    vcxFile << "  </PropertyGroup>\n";

    // Release 構成
    vcxFile << "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">\n";
    vcxFile << "    <ConfigurationType>DynamicLibrary</ConfigurationType>\n";
    vcxFile << "    <UseDebugLibraries>false</UseDebugLibraries>\n";
    vcxFile << "    <PlatformToolset>v143</PlatformToolset>\n";
    vcxFile << "    <WholeProgramOptimization>true</WholeProgramOptimization>\n";
    vcxFile << "    <CharacterSet>Unicode</CharacterSet>\n";
    vcxFile << "  </PropertyGroup>\n";

    vcxFile << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n";

    // 共通プロパティ
    vcxFile << "  <PropertyGroup>\n";
    vcxFile << "    <OutDir>$(SolutionDir)bin\\$(Configuration)\\</OutDir>\n";
    vcxFile << "    <IntDir>$(SolutionDir)bin\\Intermediate\\$(Configuration)\\</IntDir>\n";
    vcxFile << "    <TargetName>" << projectName << "</TargetName>\n";
    vcxFile << "  </PropertyGroup>\n";

    // Debug 用設定
    vcxFile << "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n";
    vcxFile << "    <ClCompile>\n";
    vcxFile << "      <WarningLevel>Level3</WarningLevel>\n";
    vcxFile << "      <Optimization>Disabled</Optimization>\n";
    vcxFile << "      <PreprocessorDefinitions>_DEBUG;EXPORT_SCRIPT_API;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
    vcxFile << "      <AdditionalIncludeDirectories>" << fullMathPath.string() << ";%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
    vcxFile << "    </ClCompile>\n";
    vcxFile << "    <Link>\n";
    vcxFile << "      <SubSystem>Windows</SubSystem>\n";
    vcxFile << "      <GenerateDebugInformation>true</GenerateDebugInformation>\n";
    vcxFile << "    </Link>\n";
    vcxFile << "  </ItemDefinitionGroup>\n";

    // Release 用設定
    vcxFile << "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\n";
    vcxFile << "    <ClCompile>\n";
    vcxFile << "      <WarningLevel>Level3</WarningLevel>\n";
    vcxFile << "      <Optimization>MaxSpeed</Optimization>\n";
    vcxFile << "      <PreprocessorDefinitions>NDEBUG;EXPORT_SCRIPT_API;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
    vcxFile << "      <AdditionalIncludeDirectories>" << fullMathPath.string() << ";%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n";
    vcxFile << "    </ClCompile>\n";
    vcxFile << "    <Link>\n";
    vcxFile << "      <SubSystem>Windows</SubSystem>\n";
    vcxFile << "      <EnableCOMDATFolding>true</EnableCOMDATFolding>\n";
    vcxFile << "      <OptimizeReferences>true</OptimizeReferences>\n";
    vcxFile << "      <GenerateDebugInformation>true</GenerateDebugInformation>\n";
    vcxFile << "    </Link>\n";
    vcxFile << "  </ItemDefinitionGroup>\n";

    // ソースファイルの登録
    vcxFile << "  <ItemGroup>\n";
    for (const auto& file : scriptFiles) {
        if (file.ends_with(".cpp")) {
            vcxFile << "    <ClCompile Include=\"" << file << "\" />\n";
        }
    }
    vcxFile << "  </ItemGroup>\n";

    // ヘッダーファイルの登録
    vcxFile << "  <ItemGroup>\n";
    for (const auto& file : scriptFiles) {
        if (file.ends_with(".h")) {
            vcxFile << "    <ClInclude Include=\"" << file << "\" />\n";
        }
    }
    vcxFile << "  </ItemGroup>\n";

    vcxFile << "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n";
    vcxFile << "</Project>\n";
    vcxFile.close();
}

void ScriptProject::UpdateFilters(const std::string& filtersPath) {
    std::vector<std::string> scriptFiles;
    scriptFiles.insert(scriptFiles.end(), scripts["cpp"].begin(), scripts["cpp"].end());
    scriptFiles.insert(scriptFiles.end(), scripts["h"].begin(), scripts["h"].end());
    std::ofstream filtersFile(filtersPath, std::ios::trunc);
    filtersFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    filtersFile << "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
    filtersFile << "  <ItemGroup>\n";
    for (const auto& file : scriptFiles) {
        if (file.ends_with(".cpp")) {
            filtersFile << "    <ClCompile Include=\"" << file << "\">\n";
            filtersFile << "      <Filter>Source Files</Filter>\n";
            filtersFile << "    </ClCompile>\n";
        } else if (file.ends_with(".h")) {
            filtersFile << "    <ClInclude Include=\"" << file << "\">\n";
            filtersFile << "      <Filter>Header Files</Filter>\n";
            filtersFile << "    </ClInclude>\n";
        }
    }
    filtersFile << "  </ItemGroup>\n";
    filtersFile << "</Project>\n";
    filtersFile.close();
}

std::string ScriptProject::FindSolutionPath()
{
    // 現在の作業ディレクトリを取得
    //std::string currentPath = fs::current_path().string();
    //std::string gameFolder = currentPath + "\\Game";
    std::string gameFolder = "C:/ChoGame";
    gameFolder = fs::absolute(gameFolder).string();

    // Gameフォルダ内を検索
    for (const auto& entry : fs::directory_iterator(gameFolder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".sln") {
            return entry.path().string(); // 最初に見つかった .sln ファイルを返す
        }
    }

    // 見つからなかった場合
    return "";
}

std::string ScriptProject::ConvertToWindowsPath(const std::string& path) {
    fs::path p = path;
    return p.make_preferred().string();
}
