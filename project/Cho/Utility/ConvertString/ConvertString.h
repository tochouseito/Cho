#pragma once
#include <string>
#include<format>
void Log(const std::string& message);
std::wstring ConvertString(const std::string& str);

std::string ConvertString(const std::wstring& str);

