#pragma once

#include "TwoBit.hpp"
#include <cstdlib>
#include <cstring>

/// \brief
/// \tparam K length of kmer
template <int K>
class KmerCounter {
public:
    using SizeType = typename TwoBitSize<K>::Type;
    using CounterType = uint32_t;
    const static size_t PossibleKmer = (0x1u << (2 * K));
    const static size_t MemSize = sizeof(CounterType) * PossibleKmer;

    KmerCounter()
        :
        counter_{NULL} {
        counter_ = (CounterType *) std::malloc(MemSize);
        if(NULL != counter_) {
            memset(counter_, 0, MemSize);
        }
    }

    ~KmerCounter() {
        free(counter_);
    }

    bool IsGood() const noexcept { return counter_ != NULL; }

    void Clear() {
        memset(counter_, 0, PossibleKmer * sizeof(SizeType));
    }

    #ifndef _COUNTER_ASSERT_ALLOCATION
    #define _COUNTER_ASSERT_ALLOCATION assert(NULL != counter_ && "memory allocation failed");
    #endif

    CounterType& operator[](SizeType i) {
        _COUNTER_ASSERT_ALLOCATION
        return counter_[i];
    }

    const CounterType& operator[](SizeType i) const {
        _COUNTER_ASSERT_ALLOCATION
        return counter_[i];
    }

    CounterType& operator[](const char *s) {
        _COUNTER_ASSERT_ALLOCATION
        return counter_[CharString2TwoBit<K>(s)];
    }

    const CounterType& operator[](const char *s) const {
        _COUNTER_ASSERT_ALLOCATION
        return counter_[CharString2TwoBit<K>(s)];
    }

    CounterType *begin() {
        _COUNTER_ASSERT_ALLOCATION
        return counter_;
    }

    const CounterType *begin() const {
        _COUNTER_ASSERT_ALLOCATION
        return counter_;
    }

    CounterType *end() {
        _COUNTER_ASSERT_ALLOCATION
        return counter_ + PossibleKmer;
    }

    const CounterType *end() const {
        _COUNTER_ASSERT_ALLOCATION
        return counter_ + PossibleKmer;
    }

    #undef _COUNTER_ASSERT_ALLOCATION

protected:
    CounterType *counter_;
};