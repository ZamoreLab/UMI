
#include <string>
#include <vector>
#include <data.hpp>
#include "bam_reader.hpp"
#include "gtest/gtest.h"

using namespace std;
namespace {
class BamSeReaderTest
    : public ::testing::Test {
protected:
    BamSeReaderTest() {
    }

public:
    BamFileSeReader reader;
};

}


TEST_F(BamSeReaderTest, ReaderInitialization) {
    EXPECT_FALSE(reader.IsInitiated());
    reader.Open(::tests::TestTinyBamFile.c_str());
    EXPECT_TRUE(reader.IsInitiated());
}

TEST_F(BamSeReaderTest, ReadNext) {
    reader.Open(::tests::TestTinyBamFile.c_str());
    int c_aligned = 0, c_unaligned = 0;
    while (reader.Next()) {
        if (reader.IsAligned()) {
            ++c_aligned;
        } else {
            ++c_unaligned;
        }
    }
    EXPECT_EQ(c_aligned, 32150);
    EXPECT_EQ(c_unaligned, 296);
}

TEST_F(BamSeReaderTest, ReadNextAligned) {
    reader.Open(::tests::TestTinyBamFile.c_str());
    int c_aligned = 0;
    while (reader.NextAligned()) {
        EXPECT_TRUE(reader.IsAligned());
        ++c_aligned;
    }
    EXPECT_EQ(c_aligned, 32150);
}

TEST_F(BamSeReaderTest, GetNameSeqQual) {
    reader.Open(::tests::TestTinyBamFile.c_str());
    reader.NextAligned();
    EXPECT_EQ(reader.GetName(), "M04279:107:000000000-AUJN0:1:1101:12738:28027");
    EXPECT_EQ(reader.GetSeq()
              , "GTTTTGTTCCAGAGTCTATCACCCACTCTTTCCCATTTGCTCTTATTCGTGTAGATCGGAAGAGCACACGTCTGAACTCCAGTCACATAGCTAGATCTCGTATGCCGTCTTCTGCTTGAAAAAAAAAAAAAAAATATTAAAAATAACAACC");
    EXPECT_EQ(reader.GetChr(), "chr1");
    EXPECT_EQ(reader.GetStart(), 20674153);
    EXPECT_TRUE(reader.IsPlusStrand());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
