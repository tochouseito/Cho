#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

class FileView {
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

private:
    /// <summary>
    /// ファイルブラウザ表示
    /// </summary>
    void ShowFileBrowser();

    /// <summary>
    /// ディレクトリ内のファイルを取得
    /// </summary>
    std::vector<std::string> GetFilesInDirectory(const std::string& directory);

    /// <summary>
    /// ファイルの内容を読み込み
    /// </summary>
    std::string ReadFileContent(const std::string& filename);

    /// <summary>
    /// ファイルの内容表示
    /// </summary>
    void ShowFileContent();

    /// <summary>
    /// ファイルブラウザをディレクトリ対応で表示
    /// </summary>
    void ShowFileBrowserWithDirectories();

    // メンバ変数
    std::string currentDirectory;      // 現在のディレクトリ
    std::vector<std::string> files;    // ディレクトリ内のファイルリスト
    std::string selectedFile;          // 選択されたファイル
};

