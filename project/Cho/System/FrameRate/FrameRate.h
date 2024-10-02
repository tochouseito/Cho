#pragma once

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

	/*記録時間FPS固定用*/
	std::chrono::steady_clock::time_point reference_;

	GameContext* gameContext_ = nullptr;

};