#include "utils.hpp"
#include <unistd.h>
#include "kernel_color.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>


void ::Utils::Warning(const std::string& s) {
    std::cerr << KERNAL_BOLDMAGENTA << "[Warning] " << s << KERNAL_RESET << std::endl;
}

void ::Utils::Error(const std::string& s) {
    std::cerr << KERNAL_BOLDRED << "[Error] " << s << KERNAL_RESET << std::endl;
    exit(EXIT_FAILURE);
}

bool ::Utils::IsFileExists(const char *file) {
    return access(file, F_OK) != -1;
}

std::string& ::Utils::Reverse(std::string& s) {
    auto begin = s.begin();
    auto end = s.end();
    auto distance = std::distance(begin, end);
    std::advance(end, -1);
    while (distance > 0) {
        std::swap(*begin, *end);
        std::advance(begin, 1);
        std::advance(end, -1);
        distance -= 2;
    }
    return s;
}

std::string& ::Utils::Complement(std::string& s) {
    for (auto& x : s) {
        switch (x) {
            case 'A':
            case 'a':
                x = 'T';
                break;
            case 'C':
            case 'c':
                x = 'G';
                break;
            case 'G':
            case 'g':
                x = 'C';
                break;
            case 'T':
            case 't':
            case 'U':
            case 'u':
                x = 'A';
                break;
            case 'N':
            case 'n':
                x = 'N';
                break;
            default:
                break;
        }
    }
    return s;
}

std::string& ::Utils::ReverseComplement(std::string& s) {
    return Complement(Reverse(s));
}