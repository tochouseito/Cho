#pragma once
#include "Game/Scripts/Sprite/BaseSprite/BaseSprite.h"
class GameOverSprite :
    public BaseSprite
{
public:
	// BaseSprite を介して継承されました
	void Initialize() override;
	void Update() override;
private:

};

