#include "PrecompiledHeader.h"
#include "ScriptManager.h"

void ScriptManager::AddScript(const std::string& NewName)
{
	if (scripts.contains(NewName)) {
		return;
	}
	scripts[NewName] = NewName;
}
