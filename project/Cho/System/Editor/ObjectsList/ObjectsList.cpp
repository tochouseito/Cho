#include "PrecompiledHeader.h"
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
    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("GameObjectList",nullptr,windowFlags);

    selectObject(sceneManager_->GetGameObjects());// ベースオブジェクトを表示
    selectObject(sceneManager_->GetCameraObjects());// カメラオブジェクトを表示

    ImGui::End();
}

void ObjectsList::selectObject(const std::unordered_map<std::string, std::unique_ptr<GameObject>>& objectList)
{
    static bool editing = false;
    static char inputBuffer[128] = ""; // 入力用バッファ
    static std::string editingName;   // 現在編集中のオブジェクト名
    std::vector<std::pair<std::string, std::string>> renameRequests;
    static Type renameType = Type::Object;

    for (auto& pair : objectList) {
        const std::string& name = pair.first;

        if (editing && editingName == name) {
            // 編集モード: InputTextを表示
            if (ImGui::InputText("##edit", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::string newName(inputBuffer);

                // 名前が空でない場合に更新
                if (!newName.empty()) {
                    renameRequests.push_back({ name, newName });  // 後で変更するために記録
                    renameType = pair.second->GetObjectType();

                    editing = false;  // 編集終了
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                editing = false;
            }
        } else {
            // 通常モード: Selectableを表示
            if (ImGui::Selectable(name.c_str())) {
                editManager_->SetSelectedGOName(name);
                editManager_->SetSelectedGO(pair.second.get());  // 選択したオブジェクトを保持
            }

            // ダブルクリックで編集モードに切り替え
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                editing = true;
                editingName = name;                       // 編集中の名前を設定
                strncpy_s(inputBuffer, IM_ARRAYSIZE(inputBuffer), name.c_str(), _TRUNCATE); // 現在の名前をバッファにコピー
            }
        }
    }
    // ループ終了後に名前を変更
    for (const auto& request : renameRequests) {
        const std::string& oldName = request.first;
        const std::string& newName = request.second;

        std::string name;

        // 名前の変更処理
        switch (renameType)
        {
        case Object:
            name = sceneManager_->GameObjectListRename(newName, oldName);  // 名前変更
            editManager_->SetSelectedGOName(name);
            editManager_->SetSelectedGO(sceneManager_->GetGameObject(name));  // 選択したオブジェクトを保持
            break;
        case Camera:
            name = sceneManager_->CameraObjectListRename(newName, oldName);
            editManager_->SetSelectedGOName(name);
            editManager_->SetSelectedGO(sceneManager_->GetCameraObject(name));  // 選択したオブジェクトを保持
            break;
        case Light:
            break;
        default:
            break;
        }
        
    }
}

