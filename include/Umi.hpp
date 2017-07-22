#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "SeqReader.hpp"
#include <string.h>
#include <seqan/basic.h>
#include <seqan/align.h>
#include <tuple>

template <class UmiIdentifyPolicy>
class Umi {

public:
    std::tuple<int, int, int, int> IdentifyUmi(const char *read);

    Umi(const char *s)
        : pattern_(s) {}

protected:
    std::string pattern_;
};

template <class UmiIdentifyPolicy>
std::tuple<int, int, int, int> Umi<UmiIdentifyPolicy>::IdentifyUmi(const char *read) {
    return UmiIdentifyPolicy::IdentifyUmi(read, pattern_.c_str());
};


struct UmiByAlignment {
    static std::tuple<int, int, int, int> IdentifyUmi(const char *read, const char *adpt);

    static std::tuple<int, int, int, int> IdentifyFivePrimeUmi(const char *read, const char *adpt);

    static std::tuple<int, int, int, int> IdentifyThreePrimeUmi(const char *read, const char *adpt);

private:
    using FivePrimeFavorAlignConfig = seqan::AlignConfig<false // penalty for inserting gaps on the left of query
                                                         , true
                                                         , true
                                                         , true>;

    using ThreePrimeFavorAlignConfig = seqan::AlignConfig<true
                                                          , true
                                                          , true
                                                          , false>; // penalty for inserting gaps on the right of query
};

struct UmiByPosition {
    static std::tuple<int, int, int, int> IdentifyUmi(const char *read, const char *adpt);

    static std::tuple<int, int, int, int> IdentifyThreePrimeUmi(const char *read, const char *adpt);
};
