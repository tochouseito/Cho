#pragma once
class IScript {
public:
    virtual ~IScript() = default;

    // スクリプトの初期化
    virtual void Start() = 0;

    // スクリプトの毎フレーム処理
    virtual void Update() = 0;
};

// ファクトリ関数のシンボル名
extern "C" __declspec(dllexport) IScript* CreateScript();

