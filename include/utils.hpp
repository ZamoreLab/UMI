#pragma once
#include <string>

namespace Utils {

void Warning(const std::string& s);

void Error(const std::string& s);

bool IsFileExists(const char *const file);

std::string& Reverse(std::string& s);

std::string& Complement(std::string& s);

std::string& ReverseComplement(std::string& s);

}