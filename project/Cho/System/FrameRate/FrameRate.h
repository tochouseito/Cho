#pragma once

#include <windows.h>
#include <iostream>
#include<chrono>
#include<thread>

class GameContext;
class FrameRate
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameContext* gameContext);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数

	void Wait();

	/*記録時間FPS固定用*/
	std::chrono::steady_clock::time_point reference_;
	LARGE_INTEGER liPerfFreq;  // パフォーマンスカウンターの周波数
	LARGE_INTEGER liPerfCount; // パフォーマンスカウンターの現在値
	double previousTime;       // 前回のフレームの時間（マイクロ秒単位）
	double targetFrameTime;    // 1フレームの目標時間（マイクロ秒単位）
	double accumulatedTimeError; // フレームレート補正用の誤差の蓄積
	int targetFPS;// 目標フレームレート

	GameContext* gameContext_ = nullptr;
};