#include "PrecompiledHeader.h"
#include "GameOverPlayer.h"

void GameOverPlayer::Initialize()
{
	SetModel("player_animation");
	transform->translation = { 2.3f,1.0f,-7.0f };
	transform->degrees.y = 90.0f;
	material->enableLighting = true;
	animation->isRestart = false;
}

void GameOverPlayer::Update()
{
	AnimationUpdate();
	/*forcedTerminationAnimation = true;
	nextAnimationRequest = GameOverAnimation::GameOver;*/
}

void GameOverPlayer::AnimationUpdate()
{
	// アニメーション遷移
	if (nextAnimationRequest) {
		// 次のアニメーションを指定
		nextAnimation = nextAnimationRequest.value();
		nextAnimationRequest = std::nullopt;// リセット
	}
	// 現在のアニメーションを保存
	nowAnimation = static_cast<GameOverAnimation>(animation->animationIndex);
	// 現在のアニメーションと次のアニメーションが異なる場合、アニメーションが一周していたら切り替える
	if (nowAnimation != nextAnimation) {
		if (animation->isEnd || forcedTerminationAnimation) {// 一周していたら切り替える
			animation->animationIndex = static_cast<uint32_t>(nextAnimation);
			animation->isRun = true;
		}
	}
	// 強制終了のフラグをリセット
	forcedTerminationAnimation = false;
}
