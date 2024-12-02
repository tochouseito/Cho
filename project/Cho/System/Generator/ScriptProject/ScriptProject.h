#pragma once

#include <iostream>
#include <fstream>

class ScriptProject
{
public:
    void GenerateScriptProject(
        const std::string& projectName, const std::string& outputPath
    );

    void GenerateScriptTemplate(
        const std::string& scriptName, const std::string& outputPath
    );

    void OpenVisualStudio(const std::string& slnPath);
};

