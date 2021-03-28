#ifndef _FILE_H
#define _FILE_H
#include <string>
#include <cstdint>
#include <cstdio>

bool is_directory(const std::string& filename);
size_t get_file_size(const std::string& filename);
std::string get_extension_from_file(const std::string& filename);

void offset(FILE* file, long offset);
uint8_t read8(FILE* file);
uint16_t read16(FILE* file);
uint32_t read32(FILE* file);
bool write16(FILE* file, uint16_t word);
bool write32(FILE* file, uint32_t dword);

#endif