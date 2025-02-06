#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

enum GameOverAnimation {
	None,
	GameOver,
};

class GameOverPlayer : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

	void AnimationUpdate();
private:
	GameOverAnimation nowAnimation = GameOverAnimation::None;// 現在のアニメーション保存用、ゲームスクリプトでは更新しない
	GameOverAnimation nextAnimation = GameOverAnimation::GameOver;// 次のアニメーション指定用、ゲームスクリプトではこちらにアニメーションを指定する
	std::optional<GameOverAnimation> nextAnimationRequest = std::nullopt; // 次のアニメーションリクエスト。空なら現在のアニメーションがループする
	bool forcedTerminationAnimation =true;// アニメーションを強制終了するかどうか
};

