#pragma once

#include "BamAux.hpp"
#include <htslib/sam.h>
#include <unordered_map>
#include <boost/functional/hash.hpp>

class Dedupper {
public:

    enum class Methods {
        UNIQUE
    };

    using value_type = std::pair<bam1_t *, bam1_t *>;

    Dedupper(BamPairCompareMethods = BamPairCompareMethods::SumQual);

    Dedupper(const Dedupper&) = delete;

    Dedupper(Dedupper&&);

    Dedupper& operator=(const Dedupper&) = delete;

    Dedupper& operator=(Dedupper&&);

    virtual ~Dedupper();

    static Dedupper *Generate(Dedupper::Methods);

    // prototype
    virtual std::vector<Dedupper::value_type> Pick(const std::vector<value_type>& inputs, int umilen) = 0;

    //    template <class TIterator>
    //    virtual std::pair<TIterator, TIterator> Pick(TIterator, TIterator) = 0;

protected:
    BamPairCompare *comparer = nullptr;
};

class UniqueDedupper
    : public Dedupper {

public:

    //    template <class TIterator>
    //    std::pair<TIterator, TIterator> Pick(TIterator iterator, TIterator tIterator) override;

    std::vector<Dedupper::value_type> Pick(const std::vector<value_type>& inputs, int umilen) override;
};
