#pragma once

#include "BamAux.hpp"
#include "BamReader.hpp"
#include "Dedup.hpp"
#include <htslib/sam.h>
#include <string>
#include <unordered_map>

/* dedup class */
class BamPeDedupper
    : public BamFilePeReader {
private:
    using base = BamFilePeReader;
    using self = BamPeDedupper;

protected:
    samFile *out_ = nullptr;
    Dedupper *dedupper_ = nullptr;
    int umilen_ = 0;

public:
    explicit BamPeDedupper(const char *filename
                           , const char *outname = "-"
                           , int l = 5
                           , Dedupper::Methods = Dedupper::Methods::UNIQUE
                          );

    BamPeDedupper(BamPeDedupper&&);

    BamPeDedupper& operator=(BamPeDedupper&&);

    ~BamPeDedupper();

    void ResetDedupMethod(Dedupper::Methods);

    bool IsSameRead() const override;

    template <class Iterator>
    int Write(Iterator b, Iterator e);

    std::pair<int, int> Run();

};
