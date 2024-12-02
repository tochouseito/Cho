#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <set>

namespace fs = std::filesystem;

class ScriptProject
{
public:
    static void GenerateScriptProject(
        const std::string& projectName, const std::string& outputPath
    );

    static void GenerateScriptTemplate(
        const std::string& scriptName, const std::string& outputPath
    );

    static void OpenVisualStudio();

    static void SyncScriptsWithProject(const std::string& projectName, const std::string& outputPath);

    static void UpdateVcxproj(const std::string& vcxprojPath, const std::set<std::string>& cppFiles, const std::set<std::string>& hFiles);

    static void UpdateFilters(const std::string& filtersPath, const std::set<std::string>& cppFiles, const std::set<std::string>& hFiles);

    static std::string FindSolutionPath();
};

