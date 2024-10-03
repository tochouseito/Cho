#include "FrameRate.h"
#include"UI/GameContext/GameContext.h"

void FrameRate::Initialize(GameContext* gameContext)
{
	// 現在時間を記録
	reference_ = std::chrono::steady_clock::now();

	gameContext_ = gameContext;
}

void FrameRate::Update()
{
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒（よりわずかに短い時間）立っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまでスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			// スリープの前に残り時間を計算
			auto remainingTime = kMinTime - (std::chrono::steady_clock::now() - reference_);
			// 残り時間が1マイクロ秒以上ならスリープ
			if (remainingTime.count() > 1) {
				std::this_thread::sleep_for(remainingTime);
			}
			//std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();

	// 新たに経過時間を取得（スリープ後の正確な時間）
	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(reference_ - now);

	// デルタタイムを秒単位に変換
	float deltaTime = elapsed.count() / 1000000.0f;
	// フレームレート計算
	float frameRate = 1.0f / deltaTime;

	gameContext_->SetFrameValue(frameRate, deltaTime);

}