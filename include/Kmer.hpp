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

    CounterType& operator[](SizeType i) {
        return counter_[i];
    }

    const CounterType& operator[](SizeType i) const {
        return counter_[i];
    }

    CounterType& operator[](const char *s) {
        return counter_[CharString2TwoBit<K>(s)];
    }

    const CounterType& operator[](const char *s) const {
        return counter_[CharString2TwoBit<K>(s)];
    }

    CounterType *begin() {
        return counter_;
    }

    const CounterType *begin() const {
        return counter_;
    }

    CounterType *end() {
        return counter_ + PossibleKmer;
    }

    const CounterType *end() const {
        return counter_ + PossibleKmer;
    }

protected:
    CounterType *counter_;
};