#include "PrecompiledHeader.h"
#include "PopupMenu.h"

// ImGui
#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void PopupMenu::Initialize(
	ResourceViewManager* rvManager,
	RTVManager* rtvManager,
	DrawExecution* drawExe,
	EntityManager* entityManager,
	ComponentManager* componentManager,
	SystemManager* systemManager,
	PrefabManager* prefabManager,
	SceneManager* sceneManager
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
}

void PopupMenu::Update(bool excludeRightClick)
{
    // メニューを開く位置を右クリックで検出
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && !excludeRightClick)
    {
        ImGui::OpenPopup("PopupMenu");  // 右クリックでメニューを開く
    }
    // ポップアップメニューの定義
    if (ImGui::BeginPopup("PopupMenu"))
    {
        // 「Add」メニュー項目の追加
        if (ImGui::BeginMenu("Create"))
        {
            // 「3DObject」サブメニューを追加
            if (ImGui::BeginMenu("3DObject"))
            {
                // 「3DObject」メニューの横にさらにリスト表示
                if (ImGui::MenuItem("BaseObject")) {
                    CreateObject();
                }

                ImGui::EndMenu(); // 「3DObject」サブメニューを終了
            }

            // 他のメニュー項目の追加
            if (ImGui::MenuItem("Camera")) { 
                CreateCamera();
            }
            if (ImGui::MenuItem("Item 3")) { /* Item 3の処理 */ }

            ImGui::EndMenu(); // 「Add」メニューを終了
        }
        ImGui::EndPopup();
    }
}

void PopupMenu::CreateObject()
{
    std::string name = "NewObject";
    name = sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
    // 初期コンポーネントのトランスフォームコンポーネントを付与
    TransformComponent TFComp;
    TFComp.Initialize();
    sceneManager_->GetGameObject(name)->AddComponent(TFComp);
}

void PopupMenu::CreateCamera()
{
    std::string name = "NewCamera";
    name = sceneManager_->AddCameraObject(name);
}
