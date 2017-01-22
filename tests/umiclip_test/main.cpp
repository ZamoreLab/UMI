#include <string>
#include "gtest/gtest.h"
#include "UmiClipper.hpp"
#include "data.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {
class UmiClipTest
    : public ::testing::Test {
protected:
    UmiClipTest()
        :
        c(tests::TestTinyFastqFile.c_str(), "/dev/stdout", 5, 3) {
    }

    UmiClipper c;
};


TEST_F(UmiClipTest, tiny) {
    char *buf = (char *) malloc(1024);
    EXPECT_EQ(c.Clip(buf, 1024), 71);
    EXPECT_TRUE(strcmp(buf, "@test1_AGCTC\nCGTAGCTAGCTAGCTAGCTAGCATCGA\n+\nIIIIIIIIIIIIIIIIIIIIIIIIIII\n") == 0);
}

TEST_F(UmiClipTest, tiny2) {
    EXPECT_EQ(c.Clip(), 71);
}


} // namespace

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

