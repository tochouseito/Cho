#include "PrecompiledHeader.h"
#include "InfoView.h"

#include"imgui.h"
#include<memory>

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

#include"ECS/System/SystemManager/SystemManager.h"

#include"Editor/EditorManager/EditorManager.h"

#include"Generator/ScriptProject/ScriptProject.h"

#include"Script/ScriptManager/ScriptManager.h"

void InfoView::Initialize(
	ResourceViewManager* rvManager,
	RTVManager* rtvManager,
	DrawExecution* drawExe,
	EntityManager* entityManager,
	ComponentManager* componentManager,
	SystemManager* systemManager,
	PrefabManager* prefabManager,
	SceneManager* sceneManager,
    EditorManager* editManager,
    ScriptManager* scriptManager
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

    scriptManager_ = scriptManager;
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
        case ObjectType::Object:

            // コンポーネントがあれば表示
            if (componentManager_->GetTransform(selectGO->GetEntityID())) {
                TransformComponent& TFCompo = *componentManager_->GetTransform(selectGO->GetEntityID());

                // Transformを表示
                ImGui::SeparatorText("Transform"); // ラインとテキスト表示

                // 平行移動の操作
                ColoredDragFloat3("Translation", &TFCompo.translation.x, 0.01f, 0.0f, 0.0f, "%.1f");

                // 回転の操作
                ColoredDragFloat3("Rotation", &TFCompo.degrees.x, 0.1f, 0.0f, 0.0f, "%.1f°");

                // スケールの操作
                ColoredDragFloat3("Scale", &TFCompo.scale.x, 0.01f, 0.0f, 0.0f, "%.1f");
            }

            if (componentManager_->GetMesh(selectGO->GetEntityID())) {
                MeshComponent& meshComp = *componentManager_->GetMesh(selectGO->GetEntityID());
                meshComp;
                // メッシュ情報を表示
                ImGui::SeparatorText("MeshPattern");

                // 現在のメッシュ形状の初期選択用変数
                MeshPattern selectedMeshPattern =static_cast<MeshPattern>(meshComp.meshID);

                // コンボボックスでメッシュ形状を選択
                const char* meshOptions[] = { "Cube", "Plane", "Sphere" };
                if (ImGui::Combo("Mesh Type", (int*)&selectedMeshPattern, meshOptions, IM_ARRAYSIZE(meshOptions)))
                {
                    // 選択されたメッシュの形状をセット
                    meshComp.SetMeshID(static_cast<uint32_t>(selectedMeshPattern));
                }

                static uint32_t selectModelMesh = 0;
                std::vector<const char*> modelOptions;
                for (auto& pair : rvManager_->GetModels()) {
                    modelOptions.push_back(pair.first.c_str());
                }
                if (!modelOptions.empty()) {
                    if (ImGui::Combo("Model Mesh", (int*)&selectModelMesh, modelOptions.data(), static_cast<int>(modelOptions.size()))) {
                        // 選択されたモデルのメッシュIDをセット
                        std::string modelName = modelOptions[selectModelMesh];
                        for (const std::string& meshName : rvManager_->GetModelData(modelName)->names) {
                            uint32_t index = rvManager_->GetModelData(modelName)->objects[meshName].meshIndex;
                            meshComp.SetMeshID(index);
							meshComp.meshModelName = modelName;
                        }
                    }
                }
            }

            if (componentManager_->GetMaterial(selectGO->GetEntityID())) {
                MaterialComponent& materialComp = *componentManager_->GetMaterial(selectGO->GetEntityID());

                // マテリアル情報を表示
                ImGui::SeparatorText("Material");

                ImGui::Text("Texture : %s", materialComp.textureID.c_str());
            }

            if (componentManager_->GetRender(selectGO->GetEntityID())) {
                RenderComponent& render = *componentManager_->GetRender(selectGO->GetEntityID());

                ImGui::Checkbox("Visible", &render.visible);
                ImGui::Checkbox("Wireframe", &render.wireframe);
            }

            if (componentManager_->GetScript(selectGO->GetEntityID(),selectGO->GetObjectType())) {
                ScriptComponent& scriptComp = *componentManager_->GetScript(selectGO->GetEntityID(),selectGO->GetObjectType());
                
                ImGui::SeparatorText("Script");

                // スクリプトのマップを取得
                std::unordered_map<ObjectType, std::unordered_map<std::string, ScriptStatus>> scripts = scriptManager_->GetScripts();
                // 対応するObjectTypeのスクリプトを検索
                auto it = scripts.find(static_cast<ObjectType>(scriptComp.type));
                if (it == scripts.end()) {
                    // 対応するスクリプトが見つからない場合
                    ImGui::Text("No scripts available for this ObjectType");
                }

                // 内部マップ（名前 -> ScriptStatus）を取得
                const auto& scriptMap = it->second;

                // 名前リストを構築
                std::vector<const char*> scriptNames;
                int currentIndex = -1;  // 現在選択中のスクリプトのインデックス

                for (const auto& pair : scriptMap) {
                    scriptNames.push_back(pair.first.c_str());  // 名前をリストに追加
                    if (scriptComp.status.name == pair.first) {
                        currentIndex = static_cast<int>(scriptNames.size()) - 1;  // 現在のスクリプト名に一致するインデックスを保存
                    }
                }

                if (scriptNames.empty()) {
                    ImGui::Text("No scripts available");
                } else {
                    // コンボボックスを表示
                    if (ImGui::Combo("ScriptName", &currentIndex, scriptNames.data(), static_cast<int>(scriptNames.size()))) {
                        // 選択が変更された場合、スクリプトコンポーネントを更新
                        scriptComp.status.name = scriptNames[currentIndex];
                        scriptComp.isScript = true;  // スクリプトが設定されているフラグを更新
                    }
                }
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
                if (!selectGO->GetScript()) {
                    if (ImGui::Selectable("ScriptComponent")) {
                        isAdd = false;
                        ScriptComponent scriptComp;
                        scriptComp.id = selectGO->GetEntityID();
                        scriptComp.type = static_cast<uint32_t>(selectGO->GetObjectType());
                        scriptComp.ptr = componentManager_;
                        
                        selectGO->AddComponent(scriptComp);
                    }
                }
                if (!selectGO->GetAnimation()) {
					if (ImGui::Selectable("AnimationComponent")) {
						isAdd = false;
						AnimationComponent animationComp;
						selectGO->AddComponent(animationComp);
					}
                }
            } else
            {
                if (ImGui::Button("AddComponent")) {
                    isAdd = true;
                }
            }
            break;
        case ObjectType::Camera:

            // コンポーネントがあれば表示
            if (componentManager_->GetCamera(selectGO->GetEntityID())) {
                CameraComponent& cameraCompo = *componentManager_->GetCamera(selectGO->GetEntityID());

                // Transformを表示
                ImGui::SeparatorText("Transform");// ラインとテキスト表示

                // 平行移動の操作
                ColoredDragFloat3("Translation", &cameraCompo.translation.x, 0.01f, 0.0f, 0.0f, "%.1f");

                // 回転の操作
                ColoredDragFloat3("Rotation", &cameraCompo.degrees.x, 0.01f, 0.0f, 0.0f, "%.1f°");
            }

            if (isAdd) {
                if (!selectGO->GetCamera()) {
                    if (ImGui::Selectable("CameraComponent")) {
                        isAdd = false;
                        CameraComponent cameraCompo;
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
        case ObjectType::Light:
            break;
        case ObjectType::Sprite:
            // コンポーネントがあれば表示
            if (componentManager_->GetSprite(selectGO->GetEntityID())) {
                SpriteComponent& spriteCompo = *componentManager_->GetSprite(selectGO->GetEntityID());

                // Transformを表示
                ImGui::SeparatorText("Transform");// ラインとテキスト表示

                // 平行移動の操作
                ImGui::DragFloat2("Position", &spriteCompo.position.x, 0.1f);

                // 回転の操作
                ImGui::DragFloat("ZRotate", &spriteCompo.rotation, 0.01f);

                // スケールの操作
                ImGui::DragFloat2("Scale", &spriteCompo.scale.x, 0.01f);

                ImGui::Checkbox("visible", &spriteCompo.render.visible);
            }

            if (isAdd) {
                
                if (!selectGO->GetSprite()) {
                    if (ImGui::Selectable("SpriteComponent")) {
                        isAdd = false;
                        SpriteComponent spriteCompo;
                        selectGO->AddComponent(spriteCompo);
                    }
                }
            }
            else
            {
                if (ImGui::Button("AddComponent")) {
                    isAdd = true;
                }
            }
            break;
        case ObjectType::Particle:
            if (isAdd) {

                if (!selectGO->GetParticle()) {
                    if (ImGui::Selectable("ParticleComponent")) {
                        isAdd = false;
                        ParticleComponent particleCompo;
                        selectGO->AddComponent(particleCompo);
                        systemManager_->Start(*entityManager_, *componentManager_);
                    }
                }

                if (!selectGO->GetEmitter()) {
                    if (ImGui::Selectable("EmitterComponent")) {
                        isAdd = false;
                        EmitterComponent emitterComp;
                        selectGO->AddComponent(emitterComp);
                    }
                }
            }
            else
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

bool InfoView::ColoredDragFloat3(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format) {
    
    ImGui::PushID(label); // 識別子をプッシュ（同じ名前のコントロールが競合しないようにする）

    float item_width = ImGui::CalcItemWidth(); // 現在の項目幅を取得
    float single_item_width = (item_width - ImGui::GetStyle().ItemSpacing.x * 2) / 3; // 3分割

    // X軸（赤背景）
    ImGui::PushItemWidth(single_item_width);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.2f, 0.2f, 1.0f)); // 赤背景
    bool x_changed = ImGui::DragFloat("##X", &v[0], v_speed, v_min, v_max, format);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine(); // 横並び

    // Y軸（緑背景）
    ImGui::PushItemWidth(single_item_width);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.5f, 0.2f, 1.0f)); // 緑背景
    bool y_changed = ImGui::DragFloat("##Y", &v[1], v_speed, v_min, v_max, format);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine(); // 横並び

    // Z軸（青背景）
    ImGui::PushItemWidth(single_item_width);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.5f, 1.0f)); // 青背景
    bool z_changed = ImGui::DragFloat("##Z", &v[2], v_speed, v_min, v_max, format);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();        // ラベルとスライダーを横並びにする
    ImGui::Text("%s", label); // ラベルを表示

    ImGui::PopID(); // 識別子をポップ

    return x_changed || y_changed || z_changed;
}
