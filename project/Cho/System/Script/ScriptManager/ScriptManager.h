#pragma once

#include <unordered_map>
#include <string>
#include"ObjectType/ObjectType.h"
#include"Script/ScriptStatus.h"

class ScriptManager
{
public:
	void Initialize();

	void AddScript(ObjectType type,const std::string& NewName);
private:
	std::unordered_map < ObjectType, std::unordered_map < std::string, ScriptStatus>> scripts;
};

