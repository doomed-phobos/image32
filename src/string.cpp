#include "string.h"

#include <vector>
#include <windows.h>

std::string to_utf8(const std::wstring& wstr)
{
   if(wstr.size() == 0) return std::string("");

   int required_size = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(),
                       nullptr, 0, NULL, NULL);

   std::vector<char> buf(++required_size);

   ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
   wstr.size(), &buf[0], buf.size(), NULL, NULL);

   return std::string(&buf[0]);
}

std::wstring from_utf8(const std::string& str)
{
   if(str.size() == 0) return std::wstring(L"");

   int required_size = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
                       str.size(), nullptr, 0);

   std::vector<wchar_t> buf(++required_size);

   ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
   str.size(), &buf[0], buf.size());

   return std::wstring(&buf[0]);
}