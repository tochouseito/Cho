#pragma once
#include"Scene/BaseScene/BaseScene.h"

// C++
#include<memory>
#include<string>

// Objects
#include"Game/Objects/GameObject/Player/Player.h"
#include"Game/Objects/GameObject/Ground/Ground.h"

// Camera
#include"Game/Objects/Camera/FollowCamera/FollowCamera.h"

class SceneManager;
class ComponentManager;
class GameScene :
    public BaseScene
{
public:
    // BaseScene を介して継承されました
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void ChangeScene() override;

    void CreateGameObject(const std::string& name);
    void CreateCamera(const std::string& name);

private:
    SceneManager* sceneManager_ = nullptr;
    ComponentManager* compManager_ = nullptr;

    // BaseObjects
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<Ground> ground = nullptr;

    // CameraObjects
    std::unique_ptr<FollowCamera> followCamera = nullptr;
};
