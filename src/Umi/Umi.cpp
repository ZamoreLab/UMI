#include <SeqReader.hpp>
#include "Umi.hpp"

UmiClipper::UmiClipper(const char *filename, const char *outname, int len, int pad)
    : base(filename)
      , umi_len_(len)
      , umi_pad_(pad) {
    outfh_ = fopen(outname, "w");
    if (outfh_ == NULL) base::good_ = false;
}

UmiClipper::UmiClipper(const std::string& filename, const std::string& outname, int len, int pad)
    :
    base(filename)
    , umi_len_(len)
    , umi_pad_(pad) {
    outfh_ = fopen(outname.c_str(), "w");
    if (outfh_ == NULL) base::good_ = false;
}

UmiClipper::UmiClipper(UmiClipper&& other)
    :
    base(std::move(other))
    , umi_len_(other.umi_len_)
    , umi_pad_(other.umi_pad_) {
    std::swap(outfh_, other.outfh_);
}

UmiClipper& UmiClipper::operator=(UmiClipper&& other) {
    if (this != &other) {
        base::operator=(std::move(other));
        umi_len_ = other.umi_len_;
        umi_pad_ = other.umi_pad_;
        std::swap(outfh_, other.outfh_);
    }
    return *this;
}

UmiClipper::~UmiClipper() {
    fclose(outfh_);
}

int UmiClipper::Clip() {
    if (base::Next()) {
        while (seq_->seq.l <= umi_len_ + umi_pad_) {
            if (!base::Next()) return 0;
        }
        return fprintf(outfh_, "@%s_"
                "%.*s\n"
                "%s\n"
                "+\n"
                "%s\n"
                       , seq_->name.s
                       , umi_len_, seq_->seq.s
                       , seq_->seq.s + umi_len_ + umi_pad_
                       , seq_->qual.s + umi_len_ + umi_pad_
        );
    } else return 0;
}

int UmiClipper::Clip(char *outbuf, size_t l) {
    if (base::Next()) {
        while (seq_->seq.l <= umi_len_ + umi_pad_) {
            if (!base::Next()) return 0;
        }
        int i = 0;
        outbuf[i++] = '@';
        memcpy(outbuf + i, seq_->name.s, seq_->name.l), i += seq_->name.l;
        outbuf[i++] = '_';
        memcpy(outbuf + i, seq_->seq.s, umi_len_), i += umi_len_;
        return i + snprintf(outbuf + i, l - i, "\n%s\n"
                "+\n"
                "%s\n"
                            , seq_->seq.s + umi_len_ + umi_pad_
                            , seq_->qual.s + umi_len_ + umi_pad_
        );
    } else return 0;
}
