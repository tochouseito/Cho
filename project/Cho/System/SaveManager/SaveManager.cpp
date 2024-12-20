#include "PrecompiledHeader.h"
#include "SaveManager.h"

#include"Script/ScriptManager/ScriptManager.h"
#include"ECS/EntityManager/EntityManager.h"
#include"ECS/ComponentManager/ComponentManager.h"
#include"ECS/PrefabManager/PrefabManager.h"
#include"ECS/GameObject/GameObject.h"
#include"Scene/SceneManager/SceneManager.h"

#include"Load/JsonFileLoader/JsonFileLoader.h"

#include"SystemState/SystemState.h"

void SaveManager::Initialize(
	ScriptManager* scriptManager,
	EntityManager* entityManager,
	ComponentManager* componentManager, 
	PrefabManager* prefabManager,
	SceneManager* sceneManager
)
{
	scriptManager_ = scriptManager;
	entityManager_ = entityManager;
	componentManager_ = componentManager;
	prefabManager_ = prefabManager;
	sceneManager_ = sceneManager;

}

void SaveManager::Save(JsonFileLoader* jsonLoad)
{
    jsonLoad;
    std::string projectName = ProjectName();
    std::string projectRoot = ProjectRoot();

    std::filesystem::path projectDataDir = projectRoot + "\\" + projectName + "\\" + "ProjectData";

    if (!std::filesystem::exists(projectDataDir)) {
        std::filesystem::create_directories(projectDataDir);
    }
    std::filesystem::path dataFile = projectDataDir / "projectData.json";
    std::string dfn = dataFile.string();
    const char* dataFileName = dfn.c_str();

    // シリアライズ
    nlohmann::ordered_json j;

    // シーンごとのゲームオブジェクトを格納
    //for (auto& scene : sceneManager_->GetScenes()) {
    nlohmann::ordered_json sceneJson;

    // シーン名をキーとして設定
    //std::string sceneName = scene.GetName();
    std::string sceneName = "MainScene";

    // ゲームオブジェクトを追加
    for (auto& gameObject : sceneManager_->GetGameObjects()) {
        nlohmann::ordered_json json;
        nlohmann::ordered_json gameObjJson;

        // 名前とIDを設定
        json["name"] = gameObject.first;
        json["entityId"] = gameObject.second->GetEntityID();

        // コンポーネント
        nlohmann::ordered_json compJ;

        // TransformComponent
        if (componentManager_->GetTransform(gameObject.second->GetEntityID())) {
            TransformComponent* tf = componentManager_->GetTransform(gameObject.second->GetEntityID());
            nlohmann::ordered_json tfJ;

            tfJ["position"] = { tf->translation.x, tf->translation.y, tf->translation.z };
            tfJ["rotation"] = { tf->rot.x, tf->rot.y, tf->rot.z };
            tfJ["scale"] = { tf->scale.x, tf->scale.y, tf->scale.z };

            compJ["transform"] = tfJ;
        }

        // コンポーネントを追加
        json["Components"] = compJ;

        gameObjJson["GameObject"] = json;

        // ゲームオブジェクトをシーンに追加
        sceneJson.push_back(gameObjJson);
    }

    // シーンを全体のJSONに追加
    j[sceneName] = sceneJson;
    // }

     // ファイルに書き込む
    std::ofstream file(dataFileName);
    if (!file.is_open()) {
        // ファイルが開けない
        assert(0);
    }
    file << j.dump(4);
    file.close();
}