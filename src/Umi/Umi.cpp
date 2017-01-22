#include "Umi.hpp"

Umi::Umi(int l, int p)
    :
    umi_len_{l}, pad_len_{p} {

}

Umi::Umi(Umi&& other)
    :
    umi_len_(other.umi_len_)
    , pad_len_(other.pad_len_) {

}

Umi& Umi::operator=(Umi&& other) {
    if (this != &other) {
        umi_len_ = other.umi_len_;
        pad_len_ = other.pad_len_;
    }
    return *this;
}

Umi::~Umi() {
}
