#include "BamDedup.hpp"
#include <utility>

BamPeDedupper::BamPeDedupper(const char *filename, const char *outname, int l, Dedupper::Methods m)
    : BamFilePeReader(filename)
      , umilen_(l) {
    if (outname) {
        out_ = sam_open(outname, "wb");
        good_ = sam_hdr_write(out_, header_) == 0;
    }
    dedupper_ = Dedupper::Generate(m);
}

BamPeDedupper::BamPeDedupper(BamPeDedupper&& other)
    :
    base(std::move(other))
    , umilen_(other.umilen_) {
    std::swap(dedupper_, other.dedupper_);
    std::swap(out_, other.out_);
}

BamPeDedupper& BamPeDedupper::operator=(BamPeDedupper&& other) {
    if (this != &other) {
        base::operator=(std::move(other));
        std::swap(dedupper_, other.dedupper_);
        std::swap(out_, other.out_);
        umilen_ = other.umilen_;
    }
    return *this;
}

BamPeDedupper::~BamPeDedupper() {
    sam_close(out_);
}

void BamPeDedupper::ResetDedupMethod(Dedupper::Methods m) {
    delete dedupper_, dedupper_ = nullptr;
    dedupper_ = Dedupper::Generate(m);
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

template <class Iterator>
int BamPeDedupper::Write(Iterator b, Iterator e) {
    int k = 0;
    for (Iterator i = b; i != e; ++i) {
        k += sam_write1(out_, header_, i->first) >= 0 && sam_write1(out_, header_, i->second) >= 0;
    }
    return k;
}

std::pair<int, int> BamPeDedupper::Run() {
    std::pair<int, int> tw{0, 0};
    int32_t previous_start = -1;
    std::string previous_chr;
    std::vector<std::pair<bam1_t *, bam1_t *>> inputs;
    while (BamFilePeReader::NextAligned()) {
        ++tw.first;

        // make sure
        if (previous_chr == BamFilePeReader::GetChr())
            assert(previous_start <= BamFilePeReader::GetStart1()
                       && "The input bam has to be sorted by the \1 while keeping the pair together");

        //        if (previous_chr == BamFilePeReader::GetChr()
        //            && previous_start > BamFilePeReader::GetStart1())
        //            fprintf(stderr, "preivous position %d, current name %s\n", previous_start, GetName().c_str());
        if (previous_chr != BamFilePeReader::GetChr()
            || previous_start != BamFilePeReader::GetStart1()) {
            // new position, need to pick the best one and output
            // 1. analyze the last batch and report output
            auto output = dedupper_->Pick(inputs, umilen_);
            tw.second += Write(output.begin(), output.end());
            // 2. clean up and get ready for next one
            for (auto& p : inputs) bam_destroy1(p.first), bam_destroy1(p.second);
            inputs.clear();
            previous_chr = GetChr();
            previous_start = GetStart1();
            // 3. put in
        }
        inputs.push_back(BamFilePeReader::MovePair());
    }
    // last write
    auto output = dedupper_->Pick(inputs, umilen_);
    tw.second += Write(output.begin(), output.end());
    // last clean
    for (auto& p : inputs) bam_destroy1(p.first), bam_destroy1(p.second);
    inputs.clear();
    return tw;
}
