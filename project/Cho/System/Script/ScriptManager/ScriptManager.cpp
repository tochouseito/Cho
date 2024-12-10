#include "PrecompiledHeader.h"
#include "ScriptManager.h"

void ScriptManager::Initialize()
{
	// 全タイプのスクリプトコンテナを用意
	scripts[ObjectType::Object];
	scripts[ObjectType::Camera];
	scripts[ObjectType::Light];
}

// スクリプトを新規追加
void ScriptManager::AddScript(ObjectType type, const std::string& NewName)
{
	if (scripts.contains(type)) {
		return;
	}
	if (scripts[type].contains(NewName)) {
		return;
	}
	ScriptStatus& script = scripts[type][NewName];
	script.name = NewName; script.type = type;
}
