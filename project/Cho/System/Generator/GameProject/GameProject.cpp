#include "PrecompiledHeader.h"
#include "GameProject.h"

void GameProject::CreateGameFolder(const std::string& projectName) {
    std::string gameFolderPath = "C:\\ChoProject";
    gameFolderPath = gameFolderPath + "\\" + projectName;
    std::string assetsFolderPath = gameFolderPath + "\\Assets";
    std::string textureFolderPath = assetsFolderPath + "\\Texture";
    std::string scriptsFolderPath = assetsFolderPath + "\\Scripts";
    std::string scenesFolderPath = assetsFolderPath + "\\Scenes";

    try {
        // Gameフォルダ作成
        if (!fs::exists(gameFolderPath)) {
            fs::create_directory(gameFolderPath);
            std::cout << "Game folder created at: " << gameFolderPath << "\n";
        } else {
            std::cout << "Game folder already exists at: " << gameFolderPath << "\n";
        }

        // Assetsフォルダ作成
        if (!fs::exists(assetsFolderPath)) {
            fs::create_directory(assetsFolderPath);
            std::cout << "Assets folder created at: " << assetsFolderPath << "\n";
        } else {
            std::cout << "Assets folder already exists at: " << assetsFolderPath << "\n";
        }

        // Textureフォルダ作成
        if (!fs::exists(textureFolderPath)) {
            fs::create_directory(textureFolderPath);
            std::cout << "Texture folder created at: " << textureFolderPath << "\n";
        } else {
            std::cout << "Texture folder already exists at: " << textureFolderPath << "\n";
        }

        // Scriptsフォルダ作成
        if (!fs::exists(scriptsFolderPath)) {
            fs::create_directory(scriptsFolderPath);
            std::cout << "Scripts folder created at: " << scriptsFolderPath << "\n";
        } else {
            std::cout << "Scripts folder already exists at: " << scriptsFolderPath << "\n";
        }

        // Scenesフォルダ作成
        if (!fs::exists(scenesFolderPath)) {
            fs::create_directory(scenesFolderPath);
            std::cout << "Scenes folder created at: " << scenesFolderPath << "\n";
        } else {
            std::cout << "Scenes folder already exists at: " << scenesFolderPath << "\n";
        }

    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating directories: " << e.what() << "\n";
    }
}
