#pragma once
#include"Game/Scripts/Camera/BaseCamera/BaseCamera.h"
class MainCamera :
	public BaseCamera
{
public:
	// BaseCamera を介して継承されました
	void Initialize() override;
	void Update() override;

	void FollowCamera(Vector3* pos) { followPos_ = pos; }

	void SetOffsetPos(const Vector3& pos) { offsetPos_ = pos; }

	// 度数で角度を操作
	void SetOffsetDegrees(const Vector3& degrees) { offsetDegrees_ = degrees; }

private:
	Vector3* followPos_ = nullptr;
	Vector3 offsetPos_ = { 0,0,0 };
	Vector3 offsetDegrees_ = { 0,0,0 };
};

