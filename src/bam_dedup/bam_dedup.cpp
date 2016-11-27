#include <htslib/sam.h>
#include "bam_dedup.hpp"
#include <iostream>

BamPeDedupper::BamPeDedupper(const char *filename, const char *outname, int l)
    : BamFilePeReader(filename)
      , umilen_(l) {
    if (outname) {
        out_ = sam_open(outname, "wb");
        good_ = sam_hdr_write(out_, header_) == 0;
    }
}

BamPeDedupper::BamPeDedupper(BamPeDedupper&& other)
    :
    base(std::move(other))
    , umilen_(other.umilen_) {
    rec_.swap(other.rec_);
    std::swap(out_, other.out_);
}

BamPeDedupper& BamPeDedupper::operator=(BamPeDedupper&& other) {
    if (this != &other) {
        base::operator=(std::move(other));
        rec_.swap(other.rec_);
        std::swap(out_, other.out_);
        umilen_ = other.umilen_;
    }
    return *this;
}

BamPeDedupper::~BamPeDedupper() {
    sam_close(out_);
    for (auto& i : rec_) {
        bam_destroy1(i.second.first), i.second.first = nullptr;
        bam_destroy1(i.second.second), i.second.second = nullptr;
    }
}

bool BamPeDedupper::IsSameRead() const {
    char *s1 = bam_get_qname(b1_);
    char *s2 = bam_get_qname(b2_);
    while (*s1 != '_') {
        if (*s1 != *s2) return false;
        ++s1, ++s2;
    }
    return true;
}

int BamPeDedupper::Write() {
    int k = 0;
    for (const auto& i : rec_) {
        bam1_t *b1 = i.second.first;
        bam1_t *b2 = i.second.second;
        if (sam_write1(out_, header_, b1) >= 0
            && sam_write1(out_, header_, b2) >= 0)
            ++k;
    }
    return k;
}

bool BamPeDedupper::NextAligned() {
    bool ret = BamFilePeReader::NextAligned();
    if (ret) {
        char *s1 = bam_get_qname(b1_);
        while (*s1++ != '_');
        char *s2 = bam_get_qname(b2_);
        while (*s2++ != '_');
        Coordinate c{_GetStart(b1_)
                     , std::string{s1, s1 + umilen_}
                     , std::string{s2, s2 + umilen_}
        };
        auto e = rec_.find(c);
        if (e != rec_.end()) {
            // need to decide which one is better
            if (_BamQualCmp(b1_, b2_, e->second.first, e->second.second)) {
                std::swap(b1_, e->second.first), std::swap(b2_, e->second.second);
            }
        } else {
            rec_[c] = std::make_pair(bam_dup1(b1_)
                                     , bam_dup1(b2_)
            );
        }
    }
    return ret;
}

bool BamPeDedupper::_BamQualCmp(bam1_t *a1, bam1_t *a2, bam1_t *b1, bam1_t *b2) {
    int i{0}, q1{0}, q2{0};
    uint8_t *s;
    s = bam_get_qual(a1);
    for (i = 0; i < a1->core.l_qseq; ++i, ++s) q1 += *s;

    s = bam_get_qual(a2);
    for (i = 0; i < a2->core.l_qseq; ++i, ++s) q1 += *s;

    s = bam_get_qual(b1);
    for (i = 0; i < b1->core.l_qseq; ++i, ++s) q2 += *s;

    s = bam_get_qual(b2);
    for (i = 0; i < b2->core.l_qseq; ++i, ++s) q2 += *s;

    return q1 > q2;
}
