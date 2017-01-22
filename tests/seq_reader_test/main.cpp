#include <string>
#include "gtest/gtest.h"
#include "SeqReader.hpp"
#include "data.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {
class ReaderTest
    : public ::testing::Test {
protected:
    ReaderTest():
        r(tests::TestTinyFastqFile.c_str()){
    }
    Reader r;
};


TEST_F(ReaderTest, tiny) {
    EXPECT_TRUE(r.Next());
    EXPECT_TRUE(strcmp(r.GetName(), "test1") == 0);
    EXPECT_TRUE(strcmp(r.GetSeq(), "AGCTCGTACGTAGCTAGCTAGCTAGCTAGCATCGA") == 0);
    EXPECT_TRUE(strcmp(r.GetQual(), "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII") == 0);
    EXPECT_FALSE(r.Next());
}

TEST_F(ReaderTest, gzip_tiny) {
    Reader gr(tests::TestTinyFastqFileGz.c_str());
    EXPECT_TRUE(gr.Next());
    EXPECT_TRUE(strcmp(gr.GetSeq(), "AGCTCGTACGTAGCTAGCTAGCTAGCTAGCATCGA") == 0);
    EXPECT_TRUE(strcmp(gr.GetName(), "test1") == 0);
    EXPECT_TRUE(strcmp(gr.GetQual(), "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII") == 0);
    EXPECT_FALSE(gr.Next());
}

} // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

