#pragma once

#include "Umi.hpp"
#include "UmiClipper.hpp"
#include <string>

class UmiClipper
    : public Reader
      , public Umi {
private:
    using reader_base = Reader;
    using umi_base = Umi;
    using self = UmiClipper;

protected:
    FILE *outfh_ = NULL;

public:
    UmiClipper(const char *, const char *, int, int);

    UmiClipper(const std::string&, const std::string&, int, int);

    UmiClipper(UmiClipper&&);

    UmiClipper& operator=(UmiClipper&&);

    ~UmiClipper();

    int Clip();

    int Clip(char *, size_t l);

};
