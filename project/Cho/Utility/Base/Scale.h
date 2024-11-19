#pragma once
struct Scale final {
	float x = 1.0f;
	float y = 1.0f;
	float z = 1.0f;
public:
	// 初期化
	inline void Initialize() {
		x = 1.0f;
		y = 1.0f;
		z = 1.0f;
	}
	// スケール加算
	inline Scale operator+(const Scale& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	// スケール減算
	inline Scale operator-(const Scale& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}

	// スケール乗算
	inline Scale operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	// スケール除算
	inline Scale operator/(float scalar) const {
		if (scalar == 0.0f) {
			return { 1.0f, 1.0f, 1.0f };
		}
		return { x / scalar, y / scalar, z / scalar };
	}

	// 均一スケール判定
	inline bool IsUniform() const {
		return x == y && y == z;
	}
};