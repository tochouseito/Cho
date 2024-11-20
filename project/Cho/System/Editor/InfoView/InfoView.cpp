#include "PrecompiledHeader.h"
#include "InfoView.h"

#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

#include"Editor/EditorManager/EditorManager.h"

void InfoView::Initialize(
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

void InfoView::Update()
{
    // 移動を無効にするフラグ
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoMove;
    ImGui::Begin("ObjectInfo",nullptr,windowFlags);
    GameObject* selectGO = editManager_->GetSelectedGO();
    if (selectGO) {
        // コンポーネント追加
        static bool isAdd = false;

        // 名前と EntityID を表示
        ImGui::Text("Name: %s EntityID: %d", editManager_->GetSelectedGOName().c_str(), selectGO->GetEntityID());

        switch (selectGO->GetObjectType())
        {
        case Object:

            // コンポーネントがあれば表示
            if (componentManager_->GetTransform(selectGO->GetEntityID())) {
                TransformComponent& TFCompo = *componentManager_->GetTransform(selectGO->GetEntityID());
                Quaternion q;
                // Transformを表示
                ImGui::SeparatorText("Transform"); // ラインとテキスト表示

                // 平行移動の操作
                ImGui::DragFloat3("Translation", &TFCompo.translation.x, 0.01f);

                // 回転の操作
                //Vector3 eulerAngles = ChoMath::ToEulerAngles(TFCompo.rotation);
                if (ImGui::DragFloat3("Rotation", &q.x, 0.01f)) {
                    // オイラー角からクォータニオンを生成し、絶対回転として設定
                    //TFCompo.rotation = ChoMath::FromEulerAngles(eulerAngles);
                    TFCompo.rotation = q * TFCompo.rotation;
                }

                // スケールの操作
                ImGui::DragFloat3("Scale", &TFCompo.scale.x, 0.01f);
            }

            if (componentManager_->GetMesh(selectGO->GetEntityID())) {
                MeshComponent& meshComp = *componentManager_->GetMesh(selectGO->GetEntityID());
                meshComp;
                // メッシュ情報を表示
                ImGui::SeparatorText("MeshPattern");

                // 現在のメッシュ形状の初期選択用変数
                static MeshPattern selectedMeshPattern =MeshPattern::Cube;

                // コンボボックスでメッシュ形状を選択
                const char* meshOptions[] = { "Cube", "Plane", "Sphere" };
                if (ImGui::Combo("Mesh Type", (int*)&selectedMeshPattern, meshOptions, IM_ARRAYSIZE(meshOptions)))
                {
                    // 選択されたメッシュの形状をセット
                    meshComp.SetMeshID(static_cast<uint32_t>(selectedMeshPattern));
                }
            }

            if (componentManager_->GetMaterial(selectGO->GetEntityID())) {
                MaterialComponent& materialComp = *componentManager_->GetMaterial(selectGO->GetEntityID());

                // マテリアル情報を表示
                ImGui::SeparatorText("Material");

                ImGui::Text("Texture : %s", materialComp.textureID.c_str());
            }
            
            if (isAdd) {
                if (!selectGO->GetMesh()) {
                    if (ImGui::Selectable("MeshComponent")) {
                        isAdd = false;
                        MeshComponent MeshCompo;

                        selectGO->AddComponent(MeshCompo);
                    }
                }
                if (!selectGO->GetTransform()) {
                    if (ImGui::Selectable("TransformComponent")) {
                        isAdd = false;
                        TransformComponent TFCompo;
                        TFCompo.Initialize();
                        selectGO->AddComponent(TFCompo);
                    }
                }
                if (!selectGO->GetRender()) {
                    if (ImGui::Selectable("RenderComponent")) {
                        isAdd = false;
                        RenderComponent RenderCompo;

                        selectGO->AddComponent(RenderCompo);
                    }
                }
                if (!selectGO->GetMaterial()) {
                    if (ImGui::Selectable("MaterialComponent")) {
                        isAdd = false;
                        MaterialComponent MaterialComp;

                        selectGO->AddComponent(MaterialComp);
                    }
                }
            } else
            {
                if (ImGui::Button("AddComponent")) {
                    isAdd = true;
                }
            }
            break;
        case Camera:

            // コンポーネントがあれば表示
            if (componentManager_->GetCamera(selectGO->GetEntityID())) {
                CameraComponent& cameraCompo = *componentManager_->GetCamera(selectGO->GetEntityID());

                // Transformを表示
                ImGui::SeparatorText("Transform");// ラインとテキスト表示
                ImGui::DragFloat3("translation", &cameraCompo.translation.x, 0.01f);
                ImGui::DragFloat3("Rotation", &cameraCompo.rotation.x, 0.01f);
            }

            if (isAdd) {
                if (!selectGO->GetCamera()) {
                    if (ImGui::Selectable("CameraComponent")) {
                        isAdd = false;
                        CameraComponent cameraCompo;
                        cameraCompo.Initialize();
                        selectGO->AddComponent(cameraCompo);
                    }
                }
            } else
            {
                if (ImGui::Button("AddComponent")) {
                    isAdd = true;
                }
            }
            break;
        default:
            break;
        }
    }
    ImGui::End();
}

void InfoView::AddComponents()
{

}
