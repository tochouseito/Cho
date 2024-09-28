#pragma once

#include<chrono>
#include<thread>

class Framelate
{
public:// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数

	/*記録時間FPS固定用*/
	std::chrono::steady_clock::time_point reference_;

};

