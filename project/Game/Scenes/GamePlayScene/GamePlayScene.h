#pragma once
#include"Scene/BaseScene/BaseScene.h"
class GamePlayScene :
    public BaseScene
{
    // BaseScene を介して継承されました
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void ChangeScene() override;
};

