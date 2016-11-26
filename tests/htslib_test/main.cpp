#include <string>
#include "gtest/gtest.h"
#include "htslib/sam.h"
#include "data.hpp"

namespace {
class HtsLibTest
    : public ::testing::Test {
protected:
    HtsLibTest() {
    }
};


TEST_F(HtsLibTest, test_1st) {
    samFile *in = sam_open(tests::TestTinyBamFile.c_str(), "r");
    EXPECT_FALSE(in == NULL);
    bam_hdr_t *header = sam_hdr_read(in);
    bam1_t *aln = bam_init1();
    int ret = sam_read1(in, header, aln);
    EXPECT_TRUE(ret == 365);
}


} // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
