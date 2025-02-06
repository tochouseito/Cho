#pragma once
#include "Game/Scripts/Light/BaseLight/BaseLight.h"
class StreetLight :
	public BaseLight
{
public:
	// BaseLight を介して継承されました
	void Initialize() override;
	void Update() override;

	void SetPosition(const Vector3& position,const uint32_t& index);
private:
	// 道路の街灯の位置に合わせるためのオフセット
	// 上から見た時の位置
	Vector3 leftTopOffset = Vector3(-8.0f, 1.5f, 4.5f);// 左上
	Vector3 rightTopOffset = Vector3(13.0f, 1.5f, 4.5f);// 右上
	Vector3 leftBottomOffset = Vector3(-8.0f, 1.5f, -6.5f);// 左下
	Vector3 rightBottomOffset = Vector3(13.0f, 1.5f, -6.5f);// 右下
};

