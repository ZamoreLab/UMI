#include <string>
#include <vector>
#include "data.hpp"
#include "bam_reader.hpp"
#include "gtest/gtest.h"

using namespace std;
namespace {
class BamPeReaderTest
    : public ::testing::Test {
protected:
    BamPeReaderTest() {
    }

public:
    BamFilePeReader reader;
};

}

TEST_F(BamPeReaderTest, ReaderInitialization) {
    EXPECT_FALSE(reader.IsInitiated());
    reader.Open(tests::TestPeBamFile.c_str());
    EXPECT_TRUE(reader.IsInitiated());
    reader.NextAligned();
    EXPECT_TRUE(reader.GetName() == "M03805:93:000000000-AP3DT:1:1101:1940:16746");
    EXPECT_TRUE(reader.IsInitiated());
    EXPECT_TRUE(reader.IsAppropriatelyAligned());
    reader.NextAligned();
    EXPECT_TRUE(reader.GetName() == "M03805:93:000000000-AP3DT:1:1101:2442:18423");
}

TEST_F(BamPeReaderTest, AlignedCount) {
    reader.Open(tests::TestPeBamFile.c_str());
    int i = 0;
    while(reader.NextAligned()) ++i;
    EXPECT_EQ(i, 7390);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}