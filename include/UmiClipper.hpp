#pragma once

#include "Umi.hpp"
#include "UmiClipper.hpp"
#include <string>

class UmiClipper
    : public Reader
      , public Umi<UmiByAlignment> {
private:
    using reader_base = Reader;
    using umi_base = Umi;
    using self = UmiClipper;

protected:
    FILE *outfh_ = NULL;
public:
    UmiClipper(const char *, const char *, const char*);

    UmiClipper(const std::string&, const std::string&, const std::string&);

    UmiClipper(UmiClipper&&);

    UmiClipper& operator=(UmiClipper&&);

    ~UmiClipper();

    bool Clip();

};
