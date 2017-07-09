#include "gtest/gtest.h"
#include "TwoBit.hpp"
#include "Kmer.hpp"
#include <type_traits>

using namespace std;

namespace {

class KmerTest
    : public ::testing::Test {
protected:
    KmerTest() {

    }

public:

};

TEST_F(KmerTest, SizeSelection) {
    const bool b1 = std::is_same<TwoBitSize<1>::Type, uint8_t>::value;
    const bool b2 = std::is_same<TwoBitSize<2>::Type, uint8_t>::value;
    const bool b3 = std::is_same<TwoBitSize<3>::Type, uint8_t>::value;
    const bool b4 = std::is_same<TwoBitSize<4>::Type, uint8_t>::value;
    const bool b5 = std::is_same<TwoBitSize<5>::Type, uint16_t>::value;
    const bool b6 = std::is_same<TwoBitSize<6>::Type, uint16_t>::value;
    const bool b7 = std::is_same<TwoBitSize<7>::Type, uint16_t>::value;
    const bool b8 = std::is_same<TwoBitSize<8>::Type, uint16_t>::value;
    const bool b9 = std::is_same<TwoBitSize<9>::Type, uint32_t>::value;
    const bool b10 = std::is_same<TwoBitSize<10>::Type, uint32_t>::value;
    const bool b11 = std::is_same<TwoBitSize<11>::Type, uint32_t>::value;
    const bool b16 = std::is_same<TwoBitSize<16>::Type, uint32_t>::value;
    const bool b31 = std::is_same<TwoBitSize<31>::Type, uint64_t>::value;
    EXPECT_TRUE(b1);
    EXPECT_TRUE(b2);
    EXPECT_TRUE(b3);
    EXPECT_TRUE(b4);
    EXPECT_TRUE(b5);
    EXPECT_TRUE(b6);
    EXPECT_TRUE(b7);
    EXPECT_TRUE(b8);
    EXPECT_TRUE(b9);
    EXPECT_TRUE(b10);
    EXPECT_TRUE(b11);
    EXPECT_TRUE(b16);
    EXPECT_TRUE(b31);

    EXPECT_TRUE(b11);
    const size_t k1 = KmerCounter<1>::PossibleKmer;
    const size_t k2 = KmerCounter<2>::PossibleKmer;
    const size_t k3 = KmerCounter<3>::PossibleKmer;
    const size_t k4 = KmerCounter<4>::PossibleKmer;
    const size_t k5 = KmerCounter<5>::PossibleKmer;
    const size_t k6 = KmerCounter<6>::PossibleKmer;
    const size_t k7 = KmerCounter<7>::PossibleKmer;
    const size_t k8 = KmerCounter<8>::PossibleKmer;
    const size_t k9 = KmerCounter<9>::PossibleKmer;
    const size_t k10 = KmerCounter<10>::PossibleKmer;
    const size_t k11 = KmerCounter<11>::PossibleKmer;
    EXPECT_EQ(k1, 4);
    EXPECT_EQ(k2, 4 * 4);
    EXPECT_EQ(k3, 4 * 4 * 4);
    EXPECT_EQ(k4, 4 * 4 * 4 * 4);
    EXPECT_EQ(k5, 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k6, 4 * 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k7, 4 * 4 * 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k8, 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k9, 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k10, 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4);
    EXPECT_EQ(k11, 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4 * 4);
}

TEST_F(KmerTest, String2TwoBit) {
    uint32_t t1 = CharString2TwoBit<11>("ACCATTCAATT");
    EXPECT_EQ(t1, (TwoBitSize<11>::Type) 0b0001010011110100001111);
    EXPECT_EQ(t1, 343311);
    uint32_t t2 = CharString2TwoBit<16>("AACCAGTTGCAGGAAC");
    EXPECT_EQ(t2, (TwoBitSize<16>::Type) 0b00000101001011111001001010000001);
}

TEST_F(KmerTest, TwoBit2String1) {
    uint32_t t1 = CharString2TwoBit<11>("ACCATTCAATT");
    EXPECT_EQ(TwoBit2String<11>(t1), string("ACCATTCAATT"));
    uint32_t t2 = CharString2TwoBit<16>("AACCAGTTGCAGGAAC");
    EXPECT_EQ(TwoBit2String<16>(t2), string("AACCAGTTGCAGGAAC"));
    uint64_t t3 = CharString2TwoBit<32>("ACTAACCAGTTGCAGGAACAGCTACGTACGAT");
    EXPECT_EQ(TwoBit2String<32>(t3), string("ACTAACCAGTTGCAGGAACAGCTACGTACGAT"));
}

TEST_F(KmerTest, CounterTest1) {
    KmerCounter<4> c;
    ASSERT_TRUE(c.IsGood());
    c[CharString2TwoBit<4>("ACGT")]++;
    c["AAAA"] = 3;
    c["AAGG"] = 99999;
    EXPECT_EQ(c[CharString2TwoBit<4>("ACGT")], 1);
    EXPECT_EQ(c["CCAA"], 0);
    EXPECT_EQ(c["AAAA"], 3);
    EXPECT_EQ(c["AAGG"], 99999);
}

TEST_F(KmerTest, CounterTest2) {
    KmerCounter<12> c;
    ASSERT_TRUE(c.IsGood());
    c[CharString2TwoBit<12>("ACGTTAGCTAGC")]++;
    c["AGCTAGACTAAT"] = 713817;
    c["GGGGGGGGGTAA"] = 938392;
    EXPECT_EQ(c[CharString2TwoBit<12>("ACGTTAGCTAGC")], 1);
    EXPECT_EQ(c["AGCTAGACTAAT"], 713817);
    EXPECT_EQ(c["GGGGGGGGGTAA"], 938392);
    EXPECT_EQ(c["AGCTAGACACAT"], 0);
    EXPECT_EQ(c["AGCTAGACACAC"], 0);
    EXPECT_EQ(c["TGCTAGACACAA"], 0);
    EXPECT_EQ(c["GGCTAGACACAG"], 0);
    EXPECT_EQ(c["AGCTCGACACAT"], 0);
}

TEST_F(KmerTest, Iterator1) {
    KmerCounter<4> c;
    int i = 0;
    for (auto& n: c) {
        EXPECT_EQ(n, size_t(0));
        n = ++i;
    }
    EXPECT_EQ(i, 256);
    i = 1;
    for (auto& n : c) {
        EXPECT_EQ(n, size_t(i++));
    }
}

} // namespace


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

