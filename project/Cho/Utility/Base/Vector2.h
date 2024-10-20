#pragma once
/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

public:
	// 初期化
	void Initialize() {
		x = 0.0f;
		y = 0.0f;
	}
};