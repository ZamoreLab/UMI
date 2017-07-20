#include <string>
#include "gtest/gtest.h"
//#include "UmiClipper.hpp"
#include "Umi.hpp"
#include "data.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {
class UmiClipTest
    : public ::testing::Test {
protected:
    UmiClipTest() {
    }

    std::string read = "ACATGCATCGTACGATCGTAGCTACGATCA";
    std::string badread = "ACATGCATCGTACCATCCATCAGTCGATCGAC";
    std::string adpt = "ATCGNNNNTACGA";
};


TEST_F(UmiClipTest, UmiByAlignment) {
    int start, end;
    std::tie(start, end) = UmiByAlignment::IdentifyUmi(read.c_str(), adpt.c_str());
    EXPECT_EQ(start, 18);
    EXPECT_EQ(end, 22);
    EXPECT_EQ(read.substr(start, end - start), "TAGC");
}

TEST_F(UmiClipTest, UmiByAlignment2) {
    int start, end;
    std::tie(start, end) = UmiByAlignment::IdentifyUmi(badread.c_str(), adpt.c_str());
    EXPECT_EQ(start, 0);
    EXPECT_EQ(end, 0);
}


TEST_F(UmiClipTest, UmiByPosition) {
    int start, end;
    std::tie(start, end) = UmiByPosition::IdentifyUmi(read.c_str(), adpt.c_str());
    EXPECT_EQ(start, 4);
    EXPECT_EQ(end, 8);
}

} // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

