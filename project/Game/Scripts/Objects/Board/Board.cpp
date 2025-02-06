#include "PrecompiledHeader.h"
#include "Board.h"

void Board::Initialize()
{
	SetModel("Obstacles");
}

void Board::Update()
{
}

void Board::SetPos(std::vector<Vector3> posContainer)
{
	uint32_t index = 0;
	for (const Vector3& pos : posContainer)
	{
		transform->transforms[index].translation = pos;
		transform->transforms[index].scale.Initialize();
		transform->transforms[index].scale = { 0.7f, 0.5f, 1.0f };
		index++;
	}
 	if (index < numInstance - 1) {
		for (uint32_t i = index;i < numInstance;++i) {
			transform->transforms[i].scale.Zero();
		}
	}
}
