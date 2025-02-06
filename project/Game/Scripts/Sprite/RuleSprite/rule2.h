#pragma once
#include "Game/Scripts/Sprite/BaseSprite/BaseSprite.h"
class ruleJump :
    public BaseSprite
{
public:
	// BaseSprite を介して継承されました
	void Initialize() override;
	void Update() override;
	bool GetIsRender() { return sprite->render.visible; }
	void SetIsRender(bool setVisible) { sprite->render.visible = setVisible; }
private:

};

