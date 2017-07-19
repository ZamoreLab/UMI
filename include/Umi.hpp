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

    Umi(int l)
        : len_(l) {}

protected:
    int len_;
    std::string seq_;
};

template <class UmiIdentifyPolicy>
std::pair<int, int> Umi<UmiIdentifyPolicy>::IdentifyUmi(const char *read) {
    return UmiIdentifyPolicy::IdentifyUmi(read, seq_.c_str());
};


struct UmiByAlignment {
    static std::pair<int,int> IdentifyUmi(const char* read, const char* adpt);
};

