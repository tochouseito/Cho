#include "EditorManager.h"

// ImGui
#include"imgui.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void EditorManager::Initialize(
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
	// MainMenu
	mainMenu = std::make_unique<MainMenu>();
	mainMenu->Initialize(
        rvManager,
        rtvManager,
        drawExe,
        entityManager,
        componentManager,
        systemManager,
        prefabManager,
        sceneManager
        );

	// FileView
	fileView = std::make_unique<FileView>();
	fileView->Initialize();

    // SceneView
    sceneView = std::make_unique<SceneView>();
    sceneView->Initialize(drawExe->GetDebugRenTexInd(), rvManager);

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

void EditorManager::Update()
{
    // 全体のImGuiウィンドウ
    //UpdateMainWindow();

    // MainMenu
    mainMenu->Update();

    // FileView
    fileView->Update();

    // SceneView
    sceneView->Update(drawExe_->GetRenderTexIndex());

    ImGui::Begin("AddObject");

    static char nameBuffer[128] = "";  // 名前を入力するバッファ
    static bool inputActive = false;   // 入力がアクティブかどうかを管理するフラグ

    if (ImGui::Button("Input Name")) {
        inputActive = true;  // ボタンを押したら入力を開始
    }

    if (inputActive) {
        ImGui::Begin("Input Name");  // 名前の入力用のウィンドウを作成

        ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));  // 名前を入力するフィールド

        if (ImGui::Button("OK")) {
            std::string name(nameBuffer);
            sceneManager_->AddGameObject(name);  // 入力された名前を使用してオブジェクトを追加
            inputActive = false;  // 入力終了
            memset(nameBuffer, 0, sizeof(nameBuffer));
        }

        ImGui::SameLine();  // 次のボタンを同じ行に配置する

        if (ImGui::Button("Cancel")) {
            inputActive = false;  // キャンセルして入力終了
        }


        ImGui::End();  // ウィンドウを閉じる
    }

    ImGui::End();

    //　消す   
    //if (ImGui::Button("Camera")) {
    static bool one = true;
    if (one) {

        std::string name = "DebugCamera";
        sceneManager_->AddCameraObject(name);
        one = false;
        name = "SceneCamera";
        sceneManager_->AddCameraObject(name);
        entityManager_->SetCameraID(1);
    }
    //}
    
    ImGui::Begin("GameObjectList");
    
    for (auto& pair : sceneManager_->GetGameObjects()) {
        const std::string& name = pair.first;  // マップのキーがオブジェクトの名前と仮定

        if (ImGui::Selectable(name.c_str())) {
            selectedGamaObjectName_ = name;
            selectedGameObject_ = pair.second.get();  // 選択したオブジェクトを保持
        }
    }

    for (auto& pair : sceneManager_->GetCameraObjects()) {
        const std::string& name = pair.first;  // マップのキーがオブジェクトの名前と仮定

        if (ImGui::Selectable(name.c_str())) {
            selectedGamaObjectName_ = name;
            selectedGameObject_ = pair.second.get();  // 選択したオブジェクトを保持
        }
    }

    ImGui::End();

    ImGui::Begin("ObjectInfo");
    if (selectedGameObject_) {
        // コンポーネント追加
        static bool isAdd = false;
        switch (selectedGameObject_->GetObjectType())
        {
        case Object:
            // 名前と EntityID を表示
            ImGui::Text("Name: %s EntityID: %d", selectedGamaObjectName_.c_str(), selectedGameObject_->GetEntityID());

            // コンポーネントがあれば表示
            if (componentManager_->GetTransform(selectedGameObject_->GetEntityID())) {
                TransformComponent& TFCompo = *componentManager_->GetTransform(selectedGameObject_->GetEntityID());
                // Transformを表示
                if (ImGui::CollapsingHeader("Transform")) {
                    ImGui::DragFloat3("Position", &TFCompo.position.x, 0.01f);
                    ImGui::DragFloat3("Rotation", &TFCompo.rotation.x, 0.01f);
                    ImGui::DragFloat3("Scale", &TFCompo.scale.x, 0.01f);
                }
            }

            
            if (isAdd) {
                if (!selectedGameObject_->GetMesh()) {
                    if (ImGui::Selectable("MeshComponent")) {
                        isAdd = false;
                        MeshComponent MeshCompo;

                        selectedGameObject_->AddComponent(MeshCompo);
                    }
                }
                if (!selectedGameObject_->GetTransform()) {
                    if (ImGui::Selectable("TransformComponent")) {
                        isAdd = false;
                        TransformComponent TFCompo;
                        TFCompo.Initialize();
                        selectedGameObject_->AddComponent(TFCompo);
                    }
                }
                if (!selectedGameObject_->GetRender()) {
                    if (ImGui::Selectable("RenderComponent")) {
                        isAdd = false;
                        RenderComponent RenderCompo;

                        selectedGameObject_->AddComponent(RenderCompo);
                    }
                }
                if (!selectedGameObject_->GetCamera()) {
                    if (ImGui::Selectable("CameraComponent")) {
                        isAdd = false;
                        CameraComponent cameraCompo;
                        cameraCompo.Initialize();
                        selectedGameObject_->AddComponent(cameraCompo);
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
            // 名前と EntityID を表示
            ImGui::Text("Name: %s EntityID: %d", selectedGamaObjectName_.c_str(), selectedGameObject_->GetEntityID());

            // コンポーネントがあれば表示
            if (componentManager_->GetCamera(selectedGameObject_->GetEntityID())) {
                CameraComponent& cameraCompo = *componentManager_->GetCamera(selectedGameObject_->GetEntityID());
                // Transformを表示
                if (ImGui::CollapsingHeader("Transform")) {
                    ImGui::DragFloat3("Position", &cameraCompo.position.x, 0.01f);
                    ImGui::DragFloat3("Rotation", &cameraCompo.rotation.x, 0.01f);
                }
            }

            if (isAdd) {
                if (!selectedGameObject_->GetCamera()) {
                    if (ImGui::Selectable("CameraComponent")) {
                        isAdd = false;
                        CameraComponent cameraCompo;
                        cameraCompo.Initialize();
                        selectedGameObject_->AddComponent(cameraCompo);
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
    if (selectedGameObject_) {

    }
    ImGui::End();

    //// 個別のドッカブルウィンドウのセットアップ
    //ImGui::Begin("Memory Editor", nullptr, ImGuiWindowFlags_NoCollapse);
    //// メモリエディタのUI要素をここに追加
    //ImGui::End(); // メモリエディタウィンドウを終了

    //ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoCollapse);
    //// ツールのUI要素をここに追加
    //ImGui::End(); // ツールウィンドウを終了

    //ImGui::Begin("Dear ImGui Test Engine", nullptr, ImGuiWindowFlags_NoCollapse);
    //// メインシーンビューのUI要素を追加
    //ImGui::Text("Orthographic"); // シーンビューの例としてテキスト要素を追加
    //// ツールバーやビューポートオプションなどの要素を追加
    //ImGui::End(); // テストエンジンウィンドウを終了

    //ImGui::Begin("Stock Prices", nullptr, ImGuiWindowFlags_NoCollapse);
    //// 株価グラフの要素をここに追加
    //ImGui::End(); // 株価ウィンドウを終了

    //// 画像に基づくその他のドッカブルな要素
    //ImGui::Begin("Tools Item Picker", nullptr, ImGuiWindowFlags_NoCollapse);
    //// 「Show windows」や「Show mesh wh」などのオプションを追加
    //ImGui::End(); // ツールアイテムピッカーウィンドウを終了
}

void EditorManager::UpdateMainWindow()
{
	// ウィンドウの位置を指定する
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	// ウィンドウのサイズを指定する
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

	// フラグでウィンドウがフォーカスされても前面に持ってこないように設定
	ImGui::Begin(
		"Cho Editor",
		nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
	);
	// ウィンドウの中に表示するUI要素
	ImGui::End();
}
