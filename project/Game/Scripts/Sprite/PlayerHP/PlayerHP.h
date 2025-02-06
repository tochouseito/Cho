#pragma once

#include"Game/Scripts/Sprite/BaseSprite/BaseSprite.h"

class PlayerHP :
	public BaseSprite
{
public:
	// BaseSprite を介して継承されました
	void Initialize() override;
	void Update() override;

	void offset(int index);
	bool GetIsRender() { return sprite->render.visible; }
	void SetIsRender(bool setVisible) { sprite->render.visible = setVisible; }
private:
	
};

