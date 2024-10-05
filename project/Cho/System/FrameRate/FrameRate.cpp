#include "FrameRate.h"
#include"UI/GameContext/GameContext.h"

void FrameRate::Initialize(GameContext* gameContext)
{
	// 現在時間を記録
	reference_ = std::chrono::steady_clock::now();

	gameContext_ = gameContext;
	//targetFPS = 60;
	//// 目標フレームレートの設定
	//this->targetFrameTime = 1000000.0 / targetFPS; // マイクロ秒単位
	//// パフォーマンスカウンターの周波数を取得
	//QueryPerformanceFrequency(&liPerfFreq);
	//// 開始時刻の取得
	//QueryPerformanceCounter(&liPerfCount);
	//previousTime = static_cast<double>(liPerfCount.QuadPart) * 1000000.0 / liPerfFreq.QuadPart;
	//accumulatedTimeError = 0.0; // 誤差の蓄積
}

void FrameRate::Update()
{
	//// 1/60秒ぴったりの時間
	//const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//// 1/60秒よりわずかに短い時間
	//const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//// 現在時間を取得する
	//std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//// 前回記録からの経過時間
	//std::chrono::microseconds elapsed =
	//	std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//// 1/60秒（よりわずかに短い時間）立っていない場合
	//if (elapsed < kMinCheckTime) {
	//	// 1/60秒経過するまでスリープを繰り返す
	//	while (std::chrono::steady_clock::now() - reference_ < kMinTime)
	//	{
	//		//// スリープの前に残り時間を計算
	//		//auto remainingTime = kMinTime - (std::chrono::steady_clock::now() - reference_);
	//		//// 残り時間が1マイクロ秒以上ならスリープ
	//		//if (remainingTime.count() > 1) {
	//		//	std::this_thread::sleep_for(remainingTime);
	//		//}
	//		std::this_thread::sleep_for(std::chrono::microseconds(1));
	//	}
	//}
	//// 現在の時間を記録する
	//reference_ = std::chrono::steady_clock::now();

	//// 新たに経過時間を取得（スリープ後の正確な時間）
	//elapsed = std::chrono::duration_cast<std::chrono::microseconds>(reference_ - now);

	//// デルタタイムを秒単位に変換
	//float deltaTime = elapsed.count() / 1000000.0f;
	//// フレームレート計算
	//float frameRate = 1.0f / deltaTime;

	//gameContext_->SetFrameValue(frameRate, deltaTime);
	//Wait();
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
		// 1/60秒経過するまでループで時間をチェックし続ける
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			// 必要に応じて短いスリープを行う（CPUの無駄な負荷を避ける）
			std::this_thread::sleep_for(std::chrono::microseconds(100));
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

void FrameRate::Wait()
{
	// 現在の時間を取得
	QueryPerformanceCounter(&liPerfCount);
	double currentTime = static_cast<double>(liPerfCount.QuadPart) * 1000000.0 / liPerfFreq.QuadPart;

	// 前のフレームから経過した時間を計算
	double elapsedTime = currentTime - previousTime;

	// 誤差を加味した実際に待機する時間
	double waitTime = targetFrameTime - elapsedTime + accumulatedTimeError;

	// 待機する時間が1マイクロ秒以上の場合のみスリープ
	if (waitTime > 0) {
		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(waitTime)));
	}

	// 次のフレームに向けて時間をリセット
	QueryPerformanceCounter(&liPerfCount);
	previousTime = static_cast<double>(liPerfCount.QuadPart) * 1000000.0 / liPerfFreq.QuadPart;

	// 次フレームにズレを補正（残りの誤差を次回のフレームで補正）
	accumulatedTimeError = waitTime - targetFrameTime;
}
