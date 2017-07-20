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

int UmiClipper::Clip() {
    std::string umi;
    int start_pos, end_pos;
    if (reader_base::Next()) {
        std::tie(start_pos, end_pos) = umi_base::IdentifyUmi(seq_->seq.s);
        if (start_pos != end_pos) {
            return fprintf(outfh_, "@%s_"
                    "%.*s\n"
                    "%s\n"
                    "+\n"
                    "%s\n"
                           , seq_->name.s
                           , end_pos - start_pos, seq_->seq.s
                           , seq_->seq.s + end_pos
                           , seq_->qual.s + end_pos
            );
        } else return 0;
    } else return 0;
}
