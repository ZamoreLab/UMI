#include "SeqReader.hpp"


Reader::Reader(const char *filename) {
    Open(filename);
}

Reader::Reader(const std::string& filename) {
    Open(filename.c_str());
}

Reader::~Reader() {
    if (seq_ != NULL) kseq_destroy(seq_);
    if (gzfh_ != Z_NULL) gzclose(gzfh_);
}

Reader::Reader(Reader&& other)
    :
    seq_(other.seq_)
    , gzfh_(other.gzfh_)
    , good_(other.good_) {
    other.gzfh_ = nullptr;
    other.seq_ = nullptr;
}

Reader& Reader::operator=(Reader&& other) {
    if (this != &other) {
        std::swap(gzfh_, other.gzfh_);
        std::swap(seq_, other.seq_);
        std::swap(good_, other.good_);
    }
    return *this;
}

void Reader::Open(const char *filename) {
    gzfh_ = gzopen(filename, "r");
    if (gzfh_ != Z_NULL) {
        seq_ = kseq_init(gzfh_);
        good_ = true;
    } else {
        good_ = true;
    }
}

bool Reader::Next() {
    good_ = kseq_read(seq_) >= 0;
    return good_;
}

bool Reader::IsGood() const noexcept {
    return good_;
}

char *Reader::GetName() {
    if (!good_) return nullptr;
    return seq_->name.s;
}

std::string Reader::GetName() const {
    if (!good_) return std::string();
    return seq_->name.s;
}

char *Reader::GetSeq() {
    if (!good_) return nullptr;
    return seq_->seq.s;
}

std::string Reader::GetSeq() const {
    if (!good_) return std::string();
    return seq_->seq.s;
}

char *Reader::GetQual() {
    if (!good_) return nullptr;
    return seq_->qual.s;
}

std::string Reader::GetQual() const {
    if (!good_) return std::string();
    return seq_->qual.s;
}
