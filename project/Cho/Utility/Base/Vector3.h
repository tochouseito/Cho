#pragma once
/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
public:
	// 初期化
	void Initialize() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
};