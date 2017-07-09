#include "TwoBit.hpp"

#define _A 0b000
#define _C 0b001
#define _G 0b010
#define _T 0b011
#define _N 0b100

const uint8_t nt_to_bit[128] = {
    _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N
    , _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N
    , _N, _N, _N, _N, _N, _N, _N, _A, _N, _C, _N, _N, _N, _G, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _T, _N, _N
    , _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _A, _N, _C, _N, _N, _N, _G, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N
    , _T, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N, _N
};

#undef _A
#undef _C
#undef _G
#undef _T
#undef _N

const char bit_to_nt[4] = {'A', 'C', 'G', 'T'};

