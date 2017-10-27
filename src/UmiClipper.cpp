#include "UmiClipper.hpp"
#include "SeqReader.hpp"

UmiClipper::UmiClipper(const char *filename, const char *outname, const char *pattern)
    : reader_base(filename)
      , umi_base(pattern) {
    outfh_ = fopen(outname, "w");
    if (outfh_ == NULL) reader_base::good_ = false;
}

UmiClipper::UmiClipper(const std::string& filename, const std::string& outname, const std::string& pattern)
    :
    reader_base(filename)
    , umi_base(pattern.c_str()) {
    outfh_ = fopen(outname.c_str(), "w");
    if (NULL == outfh_) reader_base::good_ = false;
}

UmiClipper::UmiClipper(UmiClipper&& other)
    :
    reader_base(std::move(other))
    , umi_base(std::move(other)) {
    std::swap(outfh_, other.outfh_);
}

UmiClipper& UmiClipper::operator=(UmiClipper&& other) {
    if (this != &other) {
        reader_base::operator=(std::move(other));
        umi_base::operator=(std::move(other));
        std::swap(outfh_, other.outfh_);
    }
    return *this;
}

UmiClipper::~UmiClipper() {
    fclose(outfh_);
}

bool UmiClipper::Clip() {
    std::string umi;
    int numN = std::count_if(pattern_.cbegin(), pattern_.cend(), [](char c) { return c == 'N'; });
    int umi_start, umi_end, ist_start, ist_end;
    if (reader_base::Next()) {
        std::tie(umi_start, umi_end, ist_start, ist_end) = umi_base::IdentifyUmi(seq_->seq.s);
        if (umi_start != umi_end && (umi_end - umi_start == numN)) {
            fprintf(outfh_, "@%s_"
                    "%.*s\n"
                    "%s\n"
                    "+\n"
                    "%s\n"
                    , seq_->name.s
                    , umi_end - umi_start, seq_->seq.s
                    , seq_->seq.s + ist_start
                    , seq_->qual.s + ist_start
            );
            return true;
        } else return false;
    } else {
        good_ = false;
        return false;
    }
}

std::tuple<int, int, int, int> UmiClipper::Clip2() {
    std::string umi;
    if (reader_base::Next()) {
        return umi_base::IdentifyUmi(seq_->seq.s);
    }
    return std::make_tuple(-1, -1, -1, -1);
}

void UmiClipper::Output(std::tuple<int, int, int, int> pos, FILE *out) {
    if (out == nullptr) out = outfh_;
    int umi_start, umi_end, ist_start, ist_end;
    std::tie(umi_start, umi_end, ist_start, ist_end) = pos;
    fprintf(out, "@%s_"
            "%.*s\n"
            "%s\n"
            "+\n"
            "%s\n"
            , seq_->name.s
            , umi_end - umi_start, seq_->seq.s
            , seq_->seq.s + ist_start
            , seq_->qual.s + ist_start
    );
}

void UmiClipper::OutputUmi(std::tuple<int, int, int, int> pos, const char *umi, FILE *out) {
    if (out == nullptr) out = outfh_;
    int umi_start, umi_end, ist_start, ist_end;
    std::tie(umi_start, umi_end, ist_start, ist_end) = pos;
    fprintf(out, "@%s_"
            "%s\n"
            "%s\n"
            "+\n"
            "%s\n"
            , seq_->name.s
            , umi
            , seq_->seq.s + ist_start
            , seq_->qual.s + ist_start
    );
}

PairEndUmiClipper::PairEndUmiClipper(UmiClipper *l, UmiClipper *r)
    :
    lclipper_(l), rclipper_(r) {
}

bool PairEndUmiClipper::IsGood() {
    return lclipper_->IsGood() && rclipper_->IsGood();
}

bool PairEndUmiClipper::Clip() {
    int l_umi_start, l_umi_end, l_ist_start, l_ist_end;
    int r_umi_start, r_umi_end, r_ist_start, r_ist_end;
    auto lpos = lclipper_->Clip2();
    auto rpos = rclipper_->Clip2();
    std::tie(l_umi_start, l_umi_end, l_ist_start, l_ist_end) = lpos;
    std::tie(r_umi_start, r_umi_end, r_ist_start, r_ist_end) = rpos;
    if (l_umi_start != l_umi_end && r_umi_start != r_umi_end) {
        /* only output if both are found */
        std::string umi(lclipper_->GetSeq(), std::get<1>(lpos) - std::get<0>(lpos));
        umi.append(lclipper_->GetSeq(), std::get<1>(lpos) - std::get<0>(lpos));
        lclipper_->OutputUmi(lpos, umi.c_str());
        rclipper_->OutputUmi(rpos, umi.c_str());
        return true;
    } else {
        return false;
    }
}

