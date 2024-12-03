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

namespace fs = std::filesystem;

class ScriptProject {
public:
    static void Update(const std::string& scriptsPath);                     // スクリプトの更新
    static void GenerateSolutionAndProject(const std::string& projectName, const std::string& outputPath);
    static void OpenVisualStudio();                                        // Visual Studio を開く
    static void LoadScriptAssembly(const std::string& dllPath);            // スクリプト DLL のロード

private:
    static std::string GenerateGUID();                                     // GUID 生成
    static void UpdateVcxproj(const std::string& vcxprojPath, const std::string& projectGuid,const std::string& projectName);
    static void UpdateFilters(const std::string& filtersPath);
    static std::string FindSolutionPath();

    static std::unordered_map<std::string, std::vector<std::string>> scripts;  // 更新されたスクリプトのリスト
};

