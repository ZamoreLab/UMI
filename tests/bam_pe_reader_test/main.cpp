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
    EXPECT_TRUE(reader.IsInitiated());
    EXPECT_FALSE(reader.IsAppropriatelyAligned());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}