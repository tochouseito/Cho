#pragma once
class Collider
{
public:

	virtual ~Collider() = default;
	// 半径を取得
	float GetRadius() { return radius_; }
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

	/*種別IDを取得*/
	uint32_t GetTypeID() const { return typeID_; }
	/*種別IDを設定*/
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; }

public:
	// 衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {}
	// 中心座標を取得
	virtual Vector3 GetCenterPosition() const = 0;
private:
	// 衝突半径
	float radius_ = 1.5f;
	
	/*種別ID*/
	uint32_t typeID_ = 0u;
};

