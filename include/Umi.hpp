#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "SeqReader.hpp"
#include <string.h>

class Umi {
protected:
    int umi_len_ = 0;
    int pad_len_ = 0;
public:
    Umi(int, int);

    Umi(const Umi&) = delete;

    Umi(Umi&&);

    Umi& operator=(const Umi&) = delete;

    Umi& operator=(Umi&&);

    ~Umi();

};
