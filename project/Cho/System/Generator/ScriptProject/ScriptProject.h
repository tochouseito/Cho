#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <filesystem>
#include <windows.h>

namespace fs = std::filesystem;

class ScriptProject {
public:
    static void Update(const std::string& scriptsPath);                     // スクリプトの更新
    static void GenerateSolutionAndProject(const std::string& projectName, const std::string& outputPath);
    static void OpenVisualStudio();                                        // Visual Studio を開く
    static void GenerateScriptTemplate(const std::string& scriptName, const std::string& outputPath);
    static void LoadScriptDLL(const std::string& dllPath);
    static HMODULE GetDLL() { return scriptLibrary; }

private:
    static std::string GenerateGUID();                                     // GUID 生成
    static void UpdateVcxproj(const std::string& vcxprojPath, const std::string& projectGuid,const std::string& projectName);
    static void UpdateFilters(const std::string& filtersPath);
    static std::string FindSolutionPath();

    static std::unordered_map<std::string, std::vector<std::string>> scripts;  // 更新されたスクリプトのリスト
    static HMODULE scriptLibrary;
    static std::string ConvertToWindowsPath(const std::string& path);
};

