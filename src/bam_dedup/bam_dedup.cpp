#include "bam_dedup.hpp"


BamPeDedupper::BamPeDedupper(const char *filename, const char *outname)
    : BamFilePeReader(filename) {
    if (outname) {
        out_ = sam_open(outname, "wb");
        good_ = sam_hdr_write(out_, header_) == 0;
    }
}

BamPeDedupper::BamPeDedupper(BamPeDedupper&& other)
    :
    base(std::move(other)) {
    rec_.swap(other.rec_);
    std::swap(out_, other.out_);
}

BamPeDedupper& BamPeDedupper::operator=(BamPeDedupper&& other) {
    if (this != &other) {
        base::operator=(std::move(other));
        rec_.swap(other.rec_);
        std::swap(out_, other.out_);
    }
    return *this;
}

BamPeDedupper::~BamPeDedupper() {
    sam_close(out_);
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
    return sam_write1(out_, header_, b1_) + sam_write1(out_, header_, b2_);
}