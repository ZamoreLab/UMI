#pragma once

#include <htslib/sam.h>
#include <utility>
#include <string>


enum class BamPairCompareMethods {
    SumQual
};

/* compare two bam pairs with the same UMI to get the best pair */
struct BamPairCompare {
    virtual bool operator()(const std::pair<bam1_t *, bam1_t *>& l
                            , const std::pair<bam1_t *, bam1_t *>& r
                           ) = 0;

    virtual ~BamPairCompare() {};
};

/* compare two bam pairs using their accumulated qualities */
struct BamPairCompareBySumQuality
    : BamPairCompare {
    virtual bool operator()(const std::pair<bam1_t *, bam1_t *>& l
                            , const std::pair<bam1_t *, bam1_t *>& r
                           ) override;

    virtual ~BamPairCompareBySumQuality() {}
};

BamPairCompare *GenerateBamPairCompare(BamPairCompareMethods);

std::string GetBamUmi(bam1_t *, int);