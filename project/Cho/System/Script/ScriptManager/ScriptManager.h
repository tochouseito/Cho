#pragma once

#include <unordered_map>
#include <string>


class ScriptManager
{
public:
	void AddScript(const std::string& NewName);
private:
	std::unordered_map<std::string, std::string> scripts;
};

