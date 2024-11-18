#include "PrecompiledHeader.h"
#include "StartSetting.h"

#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"D3D12/RTVManager/RTVManager.h"
#include"D3D12/DrawExecution/DrawExecution.h"

// SceneManager
#include"Scene/SceneManager/SceneManager.h"

#include"ECS/GameObject/GameObject.h"

void StartSetting::Initialize(
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
	/*std::string name = "DebugCamera";
	sceneManager->AddCameraObject(name);*/
	sceneManager->CreateDebugCamera();
	std::string name = "SceneCamera";
	name = sceneManager->AddCameraObject(name);
	Entity ent = sceneManager->GetCameraObject(name)->GetEntityID();
	entityManager->SetCameraID(ent);
	// デフォルトメッシュ生成
	rvManager->CreateMeshPattern();
	rtvManager;
	drawExe;
	componentManager;
	systemManager;
	prefabManager;
}
