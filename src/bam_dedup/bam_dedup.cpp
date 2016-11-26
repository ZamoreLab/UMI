#include "bam_dedup.hpp"

bool BamPeDedupper::IsSameRead() const {
    char *s1 = bam_get_qname(b1_);
    char *s2 = bam_get_qname(b2_);
    while (*s1 != '_') {
        if (*s1 != *s2) return false;
        ++s1, ++s2;
    }
    return true;
}
