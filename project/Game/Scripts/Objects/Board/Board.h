#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"
///////////////////////////////////////////////////////////////////////
// ボードを表示するためのモデル用のクラス
///////////////////////////////////////////////////////////////////////

// C++
#include <vector>

class FieldBlock;
class Board : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

	void SetPos(std::vector<Vector3> posContainer);
	void SetPos(const Vector3& pos, const uint32_t& index) { transform->transforms[index].translation = pos; }

	void SetScale(const Scale & scale, const uint32_t& index) { transform->transforms[index].scale = scale; }

	void SetFieldBlock(FieldBlock* fieldBlock) { fieldBlock_ = fieldBlock; }

private:
	FieldBlock* fieldBlock_ = nullptr;

	uint32_t numInstance = 60;
};

