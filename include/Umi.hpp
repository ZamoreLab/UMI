#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "SeqReader.hpp"
#include <string.h>

class UmiClipper
    : public Reader {
private:
    using base = Reader;
    using self = UmiClipper;

protected:
    int umi_len_ = 0;
    int umi_pad_ = 0;
    FILE* outfh_ = NULL;

public:
    UmiClipper(const char *, const char*, int, int);

    UmiClipper(const std::string&, const std::string&, int, int);

    UmiClipper(UmiClipper&&);

    UmiClipper& operator=(UmiClipper&&);

    ~UmiClipper();

    int Clip();

    int Clip(char*, size_t l);

};
