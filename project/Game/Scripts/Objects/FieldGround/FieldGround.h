#pragma once
#include "Game/Scripts/Objects/BaseObject/BaseObject.h"

// C++
#include <memory>

class PlayerCharacter;
class FieldBlock;

class Grounds : public BaseObject
{
public:
	// BaseObject を介して継承されました
	void Initialize() override;
	void Update() override;

	void LastFieldIndexAllocate();

	void SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);
	void SetPos(const Vector3& pos,const uint32_t& index);
	void SetRotate(const Vector3& rotate, const uint32_t& index);
	void SetLighting(int flag) { material->enableLighting = flag; }
	void SetNumGrounds(uint32_t num) { numGrounds = num; lastFieldIndex = num - 1; }

	Vector3 GetLastFieldPos() { return transform->transforms[lastFieldIndex].translation; }
	uint32_t GetLastFieldIndex() { return lastFieldIndex; }
private:
    float fieldModelOffsetX = 37.0;
	uint32_t numGrounds = 0;
	uint32_t lastFieldIndex = 0;
};

class FieldGround :
    public BaseObject
{
public:
	~FieldGround();
    // BaseObject を介して継承されました
    void Initialize() override;
    void Update() override;

	void MoveToNewPos();

	void SetPtr(SceneManager* sceneManager, ComponentManager* compManager, InputManager* inputManager);
	void SetPlayer(PlayerCharacter* player) { player_ = player; }
	void SetFieldBlock(FieldBlock* fieldBlock) { fieldBlock_ = fieldBlock; }
private:
    float fieldModelOffsetX = 37.0;
	//float modelOffsetX = 0.0;
	uint32_t numGrounds = 10;

	std::unique_ptr<Grounds> grounds=nullptr;

	PlayerCharacter* player_ = nullptr;
	FieldBlock* fieldBlock_ = nullptr;
};

