#pragma once

#include "Umi.hpp"
#include "UmiClipper.hpp"
#include <string>
#include <tuple>

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
    UmiClipper(const char *, const char *, const char *);

    UmiClipper(const std::string&, const std::string&, const std::string&);

    UmiClipper(UmiClipper&&);

    UmiClipper& operator=(UmiClipper&&);

    ~UmiClipper();

    bool Clip();

    void Output(std::tuple<int, int, int, int>, FILE* f = nullptr);

    std::tuple<int, int, int, int> Clip2();
};

class PairEndUmiClipper {
private:
    UmiClipper *lclipper_, *rclipper_;
public:
    PairEndUmiClipper(UmiClipper *l, UmiClipper *r);

    bool IsGood();

    bool Clip();
};