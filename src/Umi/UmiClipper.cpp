#include "UmiClipper.hpp"
#include "SeqReader.hpp"

UmiClipper::UmiClipper(const char *filename, const char *outname, int len, int pad)
    : reader_base(filename)
      , umi_base(len, pad) {
    outfh_ = fopen(outname, "w");
    if (outfh_ == NULL) reader_base::good_ = false;
}

UmiClipper::UmiClipper(const std::string& filename, const std::string& outname, int len, int pad)
    :
    reader_base(filename)
    , umi_base(len, pad) {
    outfh_ = fopen(outname.c_str(), "w");
    if (outfh_ == NULL) reader_base::good_ = false;
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
    if (reader_base::Next()) {
        while (seq_->seq.l <= umi_len_ + pad_len_) {
            if (!reader_base::Next()) return 0;
        }
        return fprintf(outfh_, "@%s_"
                "%.*s\n"
                "%s\n"
                "+\n"
                "%s\n"
                       , seq_->name.s
                       , umi_len_, seq_->seq.s
                       , seq_->seq.s + umi_len_ + pad_len_
                       , seq_->qual.s + umi_len_ + pad_len_
        );
    } else return 0;
}

int UmiClipper::Clip(char *outbuf, size_t l) {
    if (reader_base::Next()) {
        while (seq_->seq.l <= umi_len_ + pad_len_) {
            if (!reader_base::Next()) return 0;
        }
        int i = 0;
        outbuf[i++] = '@';
        memcpy(outbuf + i, seq_->name.s, seq_->name.l), i += seq_->name.l;
        outbuf[i++] = '_';
        memcpy(outbuf + i, seq_->seq.s, umi_len_), i += umi_len_;
        return i + snprintf(outbuf + i, l - i, "\n%s\n"
                "+\n"
                "%s\n"
                            , seq_->seq.s + umi_len_ + pad_len_
                            , seq_->qual.s + umi_len_ + pad_len_
        );
    } else return 0;
}
