#include "PrecompiledHeader.h"
#include "FieldGround.h"

#include "Game/Scripts/Objects/PlayerCharacter/PlayerCharacter.h"

FieldGround::~FieldGround()
{
	BaseObject::~BaseObject();
}

void FieldGround::Initialize()
{
	grounds = std::make_unique<Grounds>();

	grounds->Create("Field", sceneManager_, compManager_, input, true, numGrounds);

	grounds->Initialize();
	grounds->SetNumGrounds(numGrounds);

	for (uint32_t i = 0; i < numGrounds; i++)
	{
		grounds->SetPos(Vector3((fieldModelOffsetX * i)+(fieldModelOffsetX*0.5f), 0.0f, 0.0f), i);
		grounds->SetRotate(Vector3(0.0f, -90.0f, 0.0f), i);
		grounds->SetLighting(1);
	}
}

void FieldGround::Update()
{
	MoveToNewPos();
	grounds->Update();
}

void FieldGround::MoveToNewPos()
{
	Vector3 playerPos = player_->GetCenterPosition();
	Vector3 lastFieldPos = grounds->GetLastFieldPos();
	if (playerPos.x > lastFieldPos.x) {
		grounds->LastFieldIndexAllocate();
		Vector3 newPos = lastFieldPos + Vector3(fieldModelOffsetX, 0.0f, 0.0f);
		grounds->SetPos(newPos, grounds->GetLastFieldIndex());
	}
}

void FieldGround::SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;
	input = inputManager;
}

void Grounds::Initialize()
{
	SetModel("field");
}

void Grounds::Update()
{
}

void Grounds::LastFieldIndexAllocate()
{
	lastFieldIndex++;
	if (lastFieldIndex >= numGrounds) {
		lastFieldIndex = 0;
	}
}

void Grounds::SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager)
{
	sceneManager_ = sceneManager;
	compManager_ = compManager;
	input = inputManager;
}

void Grounds::SetPos(const Vector3& pos, const uint32_t& index)
{
	transform->transforms[index].translation = pos;
}

void Grounds::SetRotate(const Vector3& rotate, const uint32_t& index)
{
	transform->transforms[index].degrees = rotate;
}
