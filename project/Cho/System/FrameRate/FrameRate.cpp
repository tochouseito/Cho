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
	// フレームレートピッタリの時間
	constexpr std::chrono::microseconds kMinTime(static_cast<uint64_t>(1000000.0f / 60.0f));

	//// 1/60秒よりわずかに短い時間
	constexpr std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 64.0f));

	// 現在時間を取得する
	auto now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒 (よりわずかに短い時間) 経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		auto wait_until = reference_ + kMinTime;
		while (std::chrono::steady_clock::now() < wait_until) {
			std::this_thread::yield(); // CPUに他のスレッドの実行を許可
		}
	}

	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();

	// スリープ後の正確な経過時間を計算
	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(reference_ - now);

	// デルタタイムを秒単位に変換
	float deltaTime = static_cast<float>(elapsed.count()) / 1000000.0f;

	// もし deltaTime が極端に小さすぎない場合にフレームレートを計算
	float frameRate = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;

	// フレームレートとデルタタイムをゲームコンテキストにセット
	gameContext_->SetFrameValue(frameRate, deltaTime);
}


