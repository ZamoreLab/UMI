#include "BamAux.hpp"

bool BamPairCompareBySumQuality::operator()(const std::pair<bam1_t *, bam1_t *>& l
                                            , const std::pair<bam1_t *, bam1_t *>& r
                                           ) {
    int i{0}, q1{0}, q2{0};
    uint8_t *s;
    s = bam_get_qual(l.first);
    for (i = 0; i < l.first->core.l_qseq; ++i, ++s) q1 += *s - 33;

    s = bam_get_qual(l.second);
    for (i = 0; i < l.second->core.l_qseq; ++i, ++s) q1 += *s - 33;

    s = bam_get_qual(r.first);
    for (i = 0; i < r.first->core.l_qseq; ++i, ++s) q2 += *s - 33;

    s = bam_get_qual(r.second);
    for (i = 0; i < r.second->core.l_qseq; ++i, ++s) q2 += *s - 33;

    return q1 > q2;
}

std::string GetBamUmi(bam1_t *b, int umilen) {
    char *s = bam_get_qname(b);
    while (*s++ != '_');
    return std::string{s, s + umilen};
}

BamPairCompare *GenerateBamPairCompare(BamPairCompareMethods m) {
    if (m == BamPairCompareMethods::SumQual) return new BamPairCompareBySumQuality;
    return nullptr;
}
