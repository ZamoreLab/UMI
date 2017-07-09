#pragma once

#include <cstdint>
#include <string>

extern const uint8_t nt_to_bit[128];
extern const char bit_to_nt[4];

template <bool L4, bool L8, bool L16, bool L32>
struct _two_bit_selector;

template < >
struct _two_bit_selector<false, false, false, false> {
    using Type = uint8_t;
};

template < >
struct _two_bit_selector<true, false, false, false> {
    using Type = uint16_t;
};

template < >
struct _two_bit_selector<true, true, false, false> {
    using Type = uint32_t;
};

template < >
struct _two_bit_selector<true, true, true, false> {
    using Type = uint64_t;
};

// explicitly disable the specialization of this class
//template < >
//struct _two_bit_selector<true, true, true, true> {};

template <int T>
struct TwoBitSize {
    static const bool l4 = T > 4;
    static const bool l8 = T > 8;
    static const bool l16 = T > 16;
    static const bool l32 = T > 32;

    using Type = typename _two_bit_selector<l4, l8, l16, l32>::Type;
};

template <int K>
typename TwoBitSize<K>::Type CharString2TwoBit(const char *s) {
    typename TwoBitSize<K>::Type k{0};
    for (int i = 0; i < K; ++i) {
        k = ((k << 2) | nt_to_bit[*s++]);
    }
    return k;
}

template <int K>
std::string TwoBit2String(typename TwoBitSize<K>::Type b) {
    std::string ret(K, 'N');
    typename TwoBitSize<K>::Type mask{0b11};
    for (int i = K - 1; i >= 0; --i) {
        ret[i] = bit_to_nt[b & mask];
        b >>= 2;
    }
    return ret;
}
