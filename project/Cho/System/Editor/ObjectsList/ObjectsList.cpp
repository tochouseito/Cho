#include "ObjectsList.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

#include"Editor/EditorManager/EditorManager.h"

void ObjectsList::Initialize(
    ResourceViewManager* rvManager,
    RTVManager* rtvManager,
    DrawExecution* drawExe, 
    EntityManager* entityManager,
    ComponentManager* componentManager,
    SystemManager* systemManager,
    PrefabManager* prefabManager,
    SceneManager* sceneManager,
    EditorManager* editManager
)
{
    // D3D12
    rvManager_ = rvManager;
    rtvManager_ = rtvManager;
    drawExe_ = drawExe;

    // ECS
    entityManager_ = entityManager;
    componentManager_ = componentManager;
    systemManager_ = systemManager;
    prefabManager_ = prefabManager;

    // SceneManager
    sceneManager_ = sceneManager;

    editManager_ = editManager;
}

void ObjectsList::Update()
{
    ImGui::Begin("GameObjectList");

    for (auto& pair : sceneManager_->GetGameObjects()) {
        const std::string& name = pair.first;  // マップのキーがオブジェクトの名前と仮定

        if (ImGui::Selectable(name.c_str())) {
            editManager_->SetSelectedGOName(name);
            editManager_->SetSelectedGO(pair.second.get());  // 選択したオブジェクトを保持
        }
    }

    for (auto& pair : sceneManager_->GetCameraObjects()) {
        const std::string& name = pair.first;  // マップのキーがオブジェクトの名前と仮定

        if (ImGui::Selectable(name.c_str())) {
            editManager_->SetSelectedGOName(name);
            editManager_->SetSelectedGO(pair.second.get());  // 選択したオブジェクトを保持
        }
    }

    ImGui::End();
}
