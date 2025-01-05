#pragma once
#include"Scene/BaseScene/BaseScene.h"

// C++
#include<memory>

// Objects
#include"Game/Objects/GameObject/Player/Player.h"

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

private:
    SceneManager* sceneManager_ = nullptr;
    ComponentManager* compManager_ = nullptr;

    std::unique_ptr<Player> player = nullptr;
};
