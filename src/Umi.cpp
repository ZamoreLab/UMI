#include "Umi.hpp"

namespace seqan {
struct NAsMatchMatrix {};

#define M 2
#define X -2

template < >
struct ScoringMatrixData_<int, Dna5, NAsMatchMatrix> {
    enum {
        VALUE_SIZE = ValueSize<Dna5>::VALUE, TAB_SIZE = VALUE_SIZE * VALUE_SIZE
    };

    static inline int const *getData() {
        static int const _data[TAB_SIZE] =
            { /*A  C  G  T  N*/
                M, X, X, X, M, /* A */
                X, M, X, X, M, /* C */
                X, X, M, X, M, /* G */
                X, X, X, M, M, /* T */
                X, X, X, X, M  /* N */
            };
        return _data;
    }
};

#undef M
#undef X
}

std::tuple<int, int, int, int> UmiByAlignment::IdentifyUmi(const char *read, const char *adpt) {
    return IdentifyFivePrimeUmi(read, adpt);
};

std::tuple<int, int, int, int> UmiByAlignment::IdentifyFivePrimeUmi(const char *read, const char *adpt) {
    using namespace seqan;
    static const int MatchScore = ScoringMatrixData_<int, Dna5, NAsMatchMatrix>::getData()[0];
    static const int MisMatchScore = ScoringMatrixData_<int, Dna5, NAsMatchMatrix>::getData()[1];
    static const int MaxMisMatchAllowed = 3;
    Align<Dna5String, ArrayGaps> alignment;
    resize(rows(alignment), 2);
    assignSource(row(alignment, 0), read);
    assignSource(row(alignment, 1), adpt);
    Score<int, ScoreMatrix<Dna5, NAsMatchMatrix> > scoringScheme(-1, -3); /* gap ext and gap open */
    #ifdef USE_LOCAL_ALIGNMENT
    int score = localAlignment(alignment, scoringScheme, DynamicGaps());
    #else
    int score = globalAlignment(alignment
                                , scoringScheme
                                , FivePrimeFavorAlignConfig{}
                                , AffineGaps()
    );
    #endif
    std::tuple<int, int, int, int> ret{0, 0, 0, 0};

//    #define DEBUG 1
    #ifdef DEBUG
    std::cerr << alignment << '\n'
              << score << '\n'
              << MatchScore * strlen(adpt) + MaxMisMatchAllowed * MisMatchScore
              << "\n------\n";
    #endif
    if (score<(MatchScore * strlen(adpt)) + (MaxMisMatchAllowed * MisMatchScore)) return ret;

    int i = 0, j = 0; // j is the iterator on the alignment; i is the iterator on the read
    while (row(alignment, 0)[j] == '-') ++j; // skip the follow case
    //       i=0
    //    ---GTCCTGCTGGG---
    //       j       |||
    //    NNNNNNNNNNNGGG---

    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                            i
    while (row(alignment, 1)[j] != 'N') ++i, ++j;
    std::get<0>(ret) = i;
    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                                      i
    while (row(alignment, 1)[j] == 'N') ++i, ++j;
    std::get<1>(ret) = i;
    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                                           i
    while (row(alignment, 1)[j] != '-') ++i, ++j;
    std::get<2>(ret) = i;

    std::get<3>(ret) = strlen(read);

    return ret;
}

std::tuple<int, int, int, int> UmiByAlignment::IdentifyThreePrimeUmi(const char *read, const char *adpt) {
    using namespace seqan;
    static const int MatchScore = ScoringMatrixData_<int, Dna5, NAsMatchMatrix>::getData()[0];
    static const int MisMatchScore = ScoringMatrixData_<int, Dna5, NAsMatchMatrix>::getData()[1];
    static const int MaxMisMatchAllowed = 3;
    Align<Dna5String, ArrayGaps> alignment;
    resize(rows(alignment), 2);
    assignSource(row(alignment, 0), read);
    assignSource(row(alignment, 1), adpt);
    Score<int, ScoreMatrix<Dna5, NAsMatchMatrix> > scoringScheme(-1, -3); /* gap ext and gap open */
    #ifdef USE_LOCAL_ALIGNMENT
    int score = localAlignment(alignment, scoringScheme, DynamicGaps());
    #else
    int score = globalAlignment(alignment
                                , scoringScheme
                                , ThreePrimeFavorAlignConfig{}
                                , AffineGaps()
    );
    #endif
    std::tuple<int, int, int, int> ret{0, 0, 0, 0};

//    #define DEBUG 1
    #ifdef DEBUG
    std::cerr << alignment << '\n'
              << score << '\n'
              << MatchScore * strlen(adpt) + MaxMisMatchAllowed * MisMatchScore
              << "\n------\n";
    #endif
    if (score<(MatchScore * strlen(adpt)) + (MaxMisMatchAllowed * MisMatchScore)) return ret;

    int i = 0;
    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                            i
    while (row(alignment, 1)[i] != 'N') ++i;
    std::get<0>(ret) = i;

    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                                      i
    while (row(alignment, 1)[++i] == 'N');
    std::get<1>(ret) = i--;
    //        AGTAGCTAGTTCGACATAGCTAGTACGAACTACGACATGACATAGCTAGTACGGACAT
    //                                      |||||
    //        --------------------NNNNNNNNNNTACGA-----------------------
    //                                           i
    while (row(alignment, 1)[++i] != '-');
    std::get<2>(ret) = i;

    std::get<3>(ret) = strlen(read);

    return ret;
}

std::tuple<int, int, int, int> UmiByPosition::IdentifyUmi(const char *read, const char *adpt) {
    std::tuple<int, int, int, int> ret{0, 0, 0, 0};
    std::get<2>(ret) = static_cast<int>(strlen(adpt));
    std::get<3>(ret) = static_cast<int>(strlen(read));
    while (*adpt != 'N') ++adpt, ++std::get<0>(ret);
    std::get<1>(ret) = std::get<0>(ret);
    while (*adpt == 'N') ++adpt, ++std::get<1>(ret);
    return ret;
}
