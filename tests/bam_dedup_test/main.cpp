#include <string>
#include <vector>
#include "data.hpp"
#include "BamDedup.hpp"
#include "gtest/gtest.h"

using namespace std;
namespace {
class BamDedupTest
    : public ::testing::Test {
protected:
    BamDedupTest()
        :
        deduper(tests::TestPeUmiBamFile.c_str(), "/tmp/out.bam") {

    }

public:
    BamPeDedupper deduper;
};

}

TEST_F(BamDedupTest, ReaderInitialization) {
    EXPECT_TRUE(deduper.IsInitiated());
}


TEST_F(BamDedupTest, AlignedCount) {
    int i = 0;
    while (deduper.NextAligned()) {
        ++i;
    }
    EXPECT_EQ(i, 49064);
    EXPECT_EQ(deduper.Write(), 48871);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}