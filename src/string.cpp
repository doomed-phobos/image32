#include "string.h"

#include <cstdarg>
#include <vector>
#include <windows.h>

std::string string_to_lower(const std::string& str)
{
   std::string result(str);

   for(auto it = result.begin(); it != result.end(); it++)
      *it = std::tolower(*it);

   return result;
}

std::string format_to_string(const char* format, ...)
{
   va_list args;
   va_start(args, format);
   std::size_t required_size = vsnprintf(nullptr, 0, format, args);
   std::vector<char> buf(++required_size);

   vsnprintf(&buf[0], buf.size(), format, args);
   
   va_end(args);

   return std::string(&buf[0]);
}

std::string to_utf8(const std::wstring& wstr)
{
   if(wstr == L"") return std::string();
   
   int required_size = ::WideCharToMultiByte(CP_UTF8,
   0, wstr.c_str(), wstr.size(), NULL, 0, 0, 0);

   std::vector<char> str(++required_size);
   ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(),
   &str[0], str.size(), 0, 0);

   return std::string(&str[0]);
}

std::wstring from_utf8(const std::string& str)
{
   if(str == "") return std::wstring(L"");

   int required_size = ::MultiByteToWideChar(CP_UTF8, 0,
   str.c_str(), str.size(), NULL, 0);

   std::vector<wchar_t> wstr(++required_size);
   ::MultiByteToWideChar(CP_UTF8, 0,
   str.c_str(), str.size(), &wstr[0], wstr.size());

   return std::wstring(&wstr[0]);
}