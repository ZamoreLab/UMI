#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "SeqReader.hpp"
#include <string.h>
#include <seqan/basic.h>
#include <seqan/align.h>

template <class UmiIdentifyPolicy>
class Umi {

public:
    std::pair<int, int> IdentifyUmi(const char *read);

    Umi(const char *s)
        : pattern_(s) {}

protected:
    std::string pattern_;
};

template <class UmiIdentifyPolicy>
std::pair<int, int> Umi<UmiIdentifyPolicy>::IdentifyUmi(const char *read) {
    return UmiIdentifyPolicy::IdentifyUmi(read, pattern_.c_str());
};


struct UmiByAlignment {
    static std::pair<int, int> IdentifyUmi(const char *read, const char *adpt);
};

struct UmiByPosition {
    static std::pair<int, int> IdentifyUmi(const char *read, const char *adpt);
};
