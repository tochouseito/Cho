#include "PrecompiledHeader.h"
#include "ScriptProject.h"

void ScriptProject::GenerateScriptProject(const std::string& projectName, const std::string& outputPath) {
    // .sln ファイルの生成
    std::ofstream slnFile(outputPath + "/" + projectName + ".sln");
    slnFile << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
    // 必要な内容をここに追加
    slnFile.close();

    // .vcxproj ファイルの生成
    std::ofstream vcxFile(outputPath + "/Scripts/" + projectName + ".vcxproj");
    vcxFile << "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
    // 必要な内容をここに追加
    vcxFile.close();
}

void ScriptProject::GenerateScriptTemplate(const std::string& scriptName, const std::string& outputPath) {
    std::ofstream cppFile(outputPath + "/Scripts/" + scriptName + ".cpp");
    cppFile << "#include \"" << scriptName << ".h\"\n\n";
    cppFile << "void " << scriptName << "::OnStart() {\n    // Initialization\n}\n\n";
    cppFile << "void " << scriptName << "::OnUpdate(float deltaTime) {\n    // Update logic\n}\n";
    cppFile.close();

    std::ofstream hFile(outputPath + "/Scripts/" + scriptName + ".h");
    hFile << "#pragma once\n\n";
    hFile << "#include \"YourEngine/Component.h\"\n\n";
    hFile << "class " << scriptName << " : public Component {\n";
    hFile << "public:\n";
    hFile << "    void OnStart() override;\n";
    hFile << "    void OnUpdate(float deltaTime) override;\n";
    hFile << "};\n";
    hFile.close();
}

void ScriptProject::OpenVisualStudio(const std::string& slnPath) {
    std::string command =
        "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\devenv.exe\" ";
    command += "\"" + slnPath + "\"";
    system(command.c_str());
}
