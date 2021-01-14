#ifndef STRING_H
#define STRING_H
#include <string>

std::string format_to_string(const char* format, ...);
std::string to_utf8(const std::wstring& wstr);
std::wstring from_utf8(const std::string& str);

#endif