#pragma once
#include <string>

std::string to_utf8(const std::wstring& wstr);
std::wstring from_utf8(const std::string& str);