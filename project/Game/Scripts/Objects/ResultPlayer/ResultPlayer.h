#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

enum ResultAnimation {
	Result,
};

class ResultPlayer :
	public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

private:
	ResultAnimation nowAnimation = ResultAnimation::Result;// 現在のアニメーション保存用、ゲームスクリプトでは更新しない
	ResultAnimation nextAnimation = ResultAnimation::Result;// 次のアニメーション指定用、ゲームスクリプトではこちらにアニメーションを指定する
	std::optional<ResultAnimation> nextAnimationRequest = std::nullopt; // 次のアニメーションリクエスト。空なら現在のアニメーションがループする
	bool forcedTerminationAnimation = false;// アニメーションを強制終了するかどうか
};

