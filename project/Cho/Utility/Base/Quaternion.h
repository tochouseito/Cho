#pragma once
struct Quaternion final {
	float x;
	float y;
	float z;
	float w;

public:
	// 初期化
	void Initialize() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
};