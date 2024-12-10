#include "PrecompiledHeader.h"
#include "StartSetting.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

#include"Editor/EditorManager/EditorManager.h"

#include"Generator/GameProject/GameProject.h"
#include"Generator/ScriptProject/ScriptProject.h"

void StartSetting::Initialize(
	ResourceViewManager* rvManager,
	RTVManager* rtvManager,
	DrawExecution* drawExe,
	EntityManager* entityManager,
	ComponentManager* componentManager,
	SystemManager* systemManager,
	PrefabManager* prefabManager,
	SceneManager* sceneManager,
	EditorManager* editorManager
)
{
	// ひとまず開発用プロジェクトの作成
	GameProject::CreateGameFolder();

	sceneManager->CreateDebugCamera();
	std::string name = "SceneCamera";
	name = sceneManager->AddCameraObject(name);
	Entity ent = sceneManager->GetCameraObject(name)->GetEntityID();
	entityManager->SetCameraID(ent);
	// デフォルトメッシュ生成
	rvManager->CreateMeshPattern();
	// デフォルトオブジェクト生成
	name = "Cube";
	name = sceneManager->AddGameObject(name);
	TransformComponent TFComp;
	MeshComponent MeshComp;
	MeshComp.meshID = static_cast<uint32_t>(MeshPattern::Cube);
	MaterialComponent MaterialComp;
	RenderComponent renderComp;
	sceneManager->GetGameObject(name)->AddComponent(TFComp);
	sceneManager->GetGameObject(name)->AddComponent(MeshComp);
	sceneManager->GetGameObject(name)->AddComponent(MaterialComp);
	sceneManager->GetGameObject(name)->AddComponent(renderComp);
	editorManager->SetSelectedGOName(name);
	editorManager->SetSelectedGO(sceneManager->GetGameObject(name));
	rtvManager;
	drawExe;
	componentManager;
	systemManager;
	prefabManager;
}
