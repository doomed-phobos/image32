#ifndef STRING_H
#define STRING_H
#include <string>

std::string string_to_lower(const std::string& str);
std::string format_to_string(const char* format, ...);
std::string to_utf8(const wchar_t* wsrc, const int n);
inline std::string to_utf8(const std::wstring& wstr) {return to_utf8(wstr.c_str(), (int)wstr.size());}
std::wstring from_utf8(const std::string& str);

#endif