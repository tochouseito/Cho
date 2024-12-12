#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <functional>
#include"Generator/ScriptProject/ScriptProject.h"
#include"Script/IScript/IScript.h"
#include"Script/ScriptStatus.h"
#include"SystemState/SystemState.h"

struct ScriptComponent {
public:
    HMODULE dllHandle;

    uint32_t id;
    uint32_t type;
    ScriptStatus status;
    ComponentManager* ptr = nullptr;

    using ScriptFunc = std::function<void(uint32_t, uint32_t, ComponentManager*)>; // スクリプト関数型

    ScriptFunc startFunc;  // Start関数
    ScriptFunc updateFunc; // Update関数
    std::function<void()> cleanupFunc; // 解放関数

    bool isScript = false;

public:
    void SetGOInfo(uint32_t ID, uint32_t Type, ComponentManager* Ptr) {
        id = ID;
        type = Type;
        ptr = Ptr;
    }

    bool LoadDLLFunc() {
        std::string projectName = ProjectName();
        std::string projectRoot = ProjectRoot();
        std::string dllPath = projectRoot + "\\" + projectName + "\\" + "bin\\Debug\\" + projectName + ".dll";

        // DLLをロード
        dllHandle = LoadLibraryA(dllPath.c_str());
        if (!dllHandle) {
            std::cerr << "Failed to load DLL: " << dllPath << "\n";
            return false;
        }
        std::string funcName = "Create" + status.name + "Script";
        // CreateScript関数を取得
        typedef IScript* (*CreateScriptFunc)();
        CreateScriptFunc createScript = (CreateScriptFunc)GetProcAddress(dllHandle, funcName.c_str());
        if (!createScript) {
            std::cerr << "CreateScript function not found in DLL: " << dllPath << "\n";
            FreeLibrary(dllHandle);
            return false;
        }

        // スクリプトインスタンスを生成
        IScript* scriptInstance = createScript();
        if (!scriptInstance) {
            std::cerr << "Failed to create script instance.\n";
            FreeLibrary(dllHandle);
            return false;
        }

        // スクリプトのStart関数とUpdate関数をラップ
        startFunc = [scriptInstance](uint32_t id, uint32_t type, ComponentManager* ptr) {
            std::cout << "Script Start: ID=" << id << ", Type=" << type << "\n";
            ptr;
            scriptInstance->Start();
            };
        updateFunc = [scriptInstance](uint32_t id, uint32_t type, ComponentManager* ptr) {
            std::cout << "Script Update: ID=" << id << ", Type=" << type << "\n";
            ptr;
            scriptInstance->SetEntityInfo(id, type, ptr);
            scriptInstance->Update();
            };

        // インスタンスの解放用のクロージャを設定
        cleanupFunc = [scriptInstance, this]() {
            delete scriptInstance;
            };

        return true;
    }

    void Start() {
        if (startFunc) {
            startFunc(id, type, ptr);
        }
    }

    void Update() {
        if (updateFunc) {
            updateFunc(id, type, ptr);
        }
    }

    void Cleanup() {
        if (cleanupFunc) {
            cleanupFunc();
        }
    }

    ~ScriptComponent() {
        Cleanup();
    }
};

