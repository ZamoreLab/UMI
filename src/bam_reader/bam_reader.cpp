#include "bam_reader.hpp"
#include "utils.hpp"
#include <assert.h>

BamFileReader::BamFileReader(const char *bamfile) {
    good_ = Open(bamfile);
}

bool BamFileReader::Open(const char *bamfile) {
    bamfh_ = sam_open(bamfile, "rb");
    if (!bamfh_) {
        return false;
    }
    header_ = sam_hdr_read(bamfh_);
    return true;
}

BamFileReader::BamFileReader(BamFileReader&& other)
    : bamfh_(other.bamfh_)
      , header_(other.header_) {
    if (this != &other) {
        other.bamfh_ = nullptr;
        other.header_ = nullptr;
    }
}

BamFileReader& BamFileReader::operator=(BamFileReader&& other) {
    if (this != &other) {
        std::swap(this->bamfh_, other.bamfh_);
        std::swap(this->header_, other.header_);
    }
    return *this;
}


bool BamFileReader::IsInitiated() const {
    return bamfh_ != nullptr && header_ != nullptr;
}

bool BamFileReader::_IsPassPhred(bam1_t *b, uint8_t minphred) {
    if (b == nullptr) return false;
    uint8_t *c = bam_get_qual(b);
    for (int i = 0; i < b->core.l_qseq; ++i, ++c) {
        if (*c < minphred) {
            return false;
        }
    }
    return true;
}

bool BamFileReader::_IsPassMapq(bam1_t *b, int minmapq) {
    if (b == nullptr) return false;
    return b->core.qual >= minmapq;
}

bool BamFileReader::_IsPlusStrand(bam1_t *b) {
    return !(b->core.flag & BAM_FREVERSE);
}

int32_t BamFileReader::_GetStart(bam1_t *b) {
    return b->core.pos;
}

int32_t BamFileReader::_GetEnd(bam1_t *b) {
    return bam_endpos(b);
}

bam_hdr_t *BamFileReader::GetHeader() {
    return header_;
}

const bam_hdr_t *BamFileReader::GetHeader() const {
    return header_;
}

int32_t BamFileReader::_GetFlag(bam1_t *b) {
    return b->core.flag;;
}

uint32_t *BamFileReader::_GetCigar(bam1_t *b) {
    return bam_get_cigar(b);
}

uint32_t BamFileReader::_GetCigarLen(bam1_t *b) {
    return b->core.n_cigar;;
}

bool BamFileReader::IsGood() const noexcept {
    return good_;
}

/* BamFileSeReader */
BamFileSeReader::BamFileSeReader(const char *bamfile)
    : BamFileReader(bamfile) {
    b_ = bam_init1();
}

BamFileSeReader::BamFileSeReader(BamFileSeReader&& other)
    : BamFileReader(std::move(other))
      , b_(other.b_) {
    other.b_ = nullptr;
}

BamFileSeReader& BamFileSeReader::operator=(BamFileSeReader&& other) {
    if (this != &other) {
        BamFileReader::operator=(std::move(other));
        std::swap(this->b_, other.b_);
    }
    return *this;
}

bool BamFileSeReader::IsInitiated() const {
    return BamFileReader::IsInitiated() && b_ != nullptr;
}

bool BamFileSeReader::IsAligned() const {
    return !(b_->core.flag & BAM_FUNMAP);
}

bool BamFileSeReader::Next() {
    good_ = sam_read1(bamfh_, header_, b_) >= 0;
    return good_;
}

bool BamFileSeReader::NextAligned() {
    int ret = sam_read1(bamfh_, header_, b_);
    while (b_->core.flag & BAM_FUNMAP && ret > 0) {
        ret = sam_read1(bamfh_, header_, b_);
    }
    good_ = ret >= 0;
    return good_;
}

std::string BamFileSeReader::GetName() const {
    return bam_get_qname(b_);
}

std::string BamFileSeReader::GetChr() const {
    return header_->target_name[b_->core.tid];;
}

bool BamFileSeReader::IsPassPhred(uint8_t minphred) const {
    return BamFileReader::_IsPassPhred(b_, minphred);
}

bool BamFileSeReader::IsPassMapq(uint8_t minmapq) const {
    return BamFileReader::_IsPassMapq(b_, minmapq);
}

int32_t BamFileSeReader::GetStart() const {
    return BamFileReader::_GetStart(b_);
}

int32_t BamFileSeReader::GetEnd() const {
    return BamFileReader::_GetEnd(b_);
}

int32_t BamFileSeReader::GetFlag() const {
    return BamFileReader::_GetFlag(b_);
}

uint32_t *BamFileSeReader::GetCigar() const {
    return BamFileReader::_GetCigar(b_);
}

uint32_t BamFileSeReader::GetCigarLen() const {
    return BamFileReader::_GetCigarLen(b_);
}

std::string BamFileSeReader::GetSeq() const {
    const uint8_t *seq = bam_get_seq(b_);
    std::string qseq(static_cast<size_t>(b_->core.l_qseq), ' ');
    for (int i = 0; i < b_->core.l_qseq; ++i) {
        qseq[i] = seq_nt16_str[bam_seqi(seq, i)]; // always upper case
    }
    return qseq;
}

bam1_t *BamFileSeReader::GetBam() {
    return b_;
}

const bam1_t *BamFileSeReader::GetBam() const {
    return b_;
}

bool BamFileSeReader::IsPlusStrand() const {
    return BamFileReader::_IsPlusStrand(b_);
}

/* BamFilePeReader */
BamFilePeReader::BamFilePeReader(const char *bamfile)
    : BamFileReader(bamfile) {
    b1_ = bam_init1();
    b2_ = bam_init1();
}

BamFilePeReader::BamFilePeReader(BamFilePeReader&& other)
    : BamFileReader(std::move(other))
      , b1_(other.b1_)
      , b2_(other.b2_) {
    if (this != &other) {
        other.b1_ = nullptr;
        other.b2_ = nullptr;
    }
}

BamFilePeReader& BamFilePeReader::operator=(BamFilePeReader&& other) {
    if (this != &other) {
        BamFileReader::operator=(std::move(other));
        std::swap(this->b1_, other.b1_);
        std::swap(this->b2_, other.b2_);
    }
    return *this;
}


bool BamFilePeReader::Next() {
    if (sam_read1(bamfh_, header_, b1_) <= 0
        || sam_read1(bamfh_, header_, b2_) <= 0) {
        good_ = false;
        return good_;
    }
    if (IsAligned()) EnsureDirection();
    if (!IsSameRead()) good_ = false;
    return good_;
}

bool BamFilePeReader::NextAligned() {
    int ret1 = sam_read1(bamfh_, header_, b1_);
    while (b1_->core.flag & BAM_FUNMAP && ret1 > 0) {
        ret1 = sam_read1(bamfh_, header_, b1_);
    }
    if (ret1 <= 0) return false;

    int ret2 = sam_read1(bamfh_, header_, b2_);
    while (b2_->core.flag & BAM_FUNMAP && ret2 > 0) {
        ret2 = sam_read1(bamfh_, header_, b2_);
    }
    if (ret2 <= 0) return false;
    EnsureDirection();
    return IsAppropriatelyAligned()
        && IsSameRead();
}

bool BamFilePeReader::IsAligned() const {
    return !(b1_->core.flag & BAM_FUNMAP || b2_->core.flag & BAM_FUNMAP);
}

std::string BamFilePeReader::GetSeq1() const {
    const uint8_t *seq = bam_get_seq(b1_);
    std::string qseq(static_cast<size_t>(b1_->core.l_qseq), ' ');
    for (int i = 0; i < b1_->core.l_qseq; ++i) {
        qseq[i] = seq_nt16_str[bam_seqi(seq, i)];
    }
    assert(!(b1_->core.flag
        & BAM_FREVERSE)); // we always ensure direction so b1 is always on the left and on + strand
    return qseq;
}

std::string BamFilePeReader::GetSeq2() const {
    const uint8_t *seq = bam_get_seq(b2_);
    std::string qseq(static_cast<size_t>(b2_->core.l_qseq), ' ');
    for (int i = 0; i < b2_->core.l_qseq; ++i) {
        qseq[i] = seq_nt16_str[bam_seqi(seq, i)];
    }
    assert (b2_->core.flag & BAM_FREVERSE); // b2 is always on the - strand
    Utils::ReverseComplement(qseq);
    return qseq;
}

std::string BamFilePeReader::GetName() const {
    return bam_get_qname(b1_);
}

std::string BamFilePeReader::GetChr() const {
    return header_->target_name[b1_->core.tid];
}

bool BamFilePeReader::IsPassPhred(uint8_t minphred) const {
    return BamFileReader::_IsPassPhred(b1_, minphred)
        && BamFileReader::_IsPassPhred(b2_, minphred);
}

bool BamFilePeReader::IsPassMapq(uint8_t minphred) const {
    return BamFileReader::_IsPassMapq(b1_, minphred)
        && BamFileReader::_IsPassMapq(b2_, minphred);
}

int32_t BamFilePeReader::GetStart1() const {
    return BamFileReader::_GetStart(b1_);
}

int32_t BamFilePeReader::GetEnd1() const {
    return BamFileReader::_GetEnd(b1_);
}

int32_t BamFilePeReader::GetStart2() const {
    return BamFileReader::_GetStart(b2_);
}

int32_t BamFilePeReader::GetEnd2() const {
    return BamFileReader::_GetEnd(b2_);
}

int32_t BamFilePeReader::GetFlag1() const {
    return BamFileReader::_GetFlag(b1_);
}

int32_t BamFilePeReader::GetFlag2() const {
    return BamFileReader::_GetFlag(b2_);
}

uint32_t *BamFilePeReader::GetCigar1() const {
    return BamFileReader::_GetCigar(b1_);
}

uint32_t *BamFilePeReader::GetCigar2() const {
    return BamFileReader::_GetCigar(b2_);
}

uint32_t BamFilePeReader::GetCigarLen1() const {
    return BamFileReader::_GetCigarLen(b1_);
}

uint32_t BamFilePeReader::GetCigarLen2() const {
    return BamFileReader::_GetCigarLen(b2_);

}

bool BamFilePeReader::IsSameRead() const {
    return strcmp(bam_get_qname(b1_), bam_get_qname(b2_)) == 0;
}

bool BamFilePeReader::IsAlignedOppoStrand() const {
    return ((b1_->core.flag & BAM_FREVERSE) ^ (b2_->core.flag & BAM_FREVERSE)) > 0;
}

bool BamFilePeReader::IsAppropriatelyAligned() const {
    return IsAligned()
        && IsSameRead()
        && (b1_->core.flag & BAM_FREAD1) ^ (b2_->core.flag & BAM_FREAD1)
        && IsAlignedOppoStrand();
}

void BamFilePeReader::EnsureDirection() {
    if (b1_->core.pos > b2_->core.pos)
        SwapPair();
}

void BamFilePeReader::SwapPair() {
    bam1_t *swap = b1_;
    b1_ = b2_;
    b2_ = swap;
}

