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

    std::string read2 = "AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACAT";
    std::string adpt2 = "NNNNNNNNNNTACGA";
};
//
//
//TEST_F(UmiClipTest, UmiByAlignment) {
//    int umi_start, umi_end, ist_start;
//    std::tie(umi_start, umi_end, ist_start) = UmiByAlignment::IdentifyUmi(read.c_str(), adpt.c_str());
//    EXPECT_EQ(umi_start, 18);
//    EXPECT_EQ(umi_end, 22);
//    EXPECT_EQ(ist_start, 27);
//    EXPECT_EQ(read.substr(umi_start, umi_end - umi_start), "TAGC");
//}
//
//TEST_F(UmiClipTest, UmiByAlignment2) {
//    int umi_start, umi_end, ist_start;
//    std::tie(umi_start, umi_end, ist_start) = UmiByAlignment::IdentifyUmi(badread.c_str(), adpt.c_str());
//    EXPECT_EQ(umi_start, 0);
//    EXPECT_EQ(umi_end, 0);
//    EXPECT_EQ(ist_start, 0);
//}


TEST_F(UmiClipTest, UmiByAlignment3) {
    int umi_start, umi_end, ist_start;
    std::tie(umi_start, umi_end, ist_start) = UmiByAlignment::IdentifyUmi(read2.c_str(), adpt2.c_str());
    EXPECT_EQ(umi_start, 0);
    EXPECT_EQ(umi_end, 10);
    EXPECT_EQ(ist_start, 15);
    EXPECT_EQ(read2.substr(umi_start, umi_end - umi_start), "AGTAGCTAGT");
}

TEST_F(UmiClipTest, UmiByPosition) {
    int umi_start, umi_end, ist_start;
    std::tie(umi_start, umi_end, ist_start) = UmiByPosition::IdentifyUmi(read.c_str(), adpt.c_str());
    EXPECT_EQ(umi_start, 4);
    EXPECT_EQ(umi_end, 8);
    EXPECT_EQ(ist_start, 13);
}

} // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

