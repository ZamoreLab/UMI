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


std::tuple<int, int, int> UmiByAlignment::IdentifyUmi(const char *read, const char *adpt) {
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
                                , AlignConfig<true
                                              , true
                                              , true
                                              , true>{} /* free-end alignment, no penalties for gap at the ends */
                                , AffineGaps()
    );
    #endif
    std::tuple<int, int, int> ret{0, 0, 0};

    #ifdef DEBUG
    std::cerr << alignment << '\n'
              << score << '\n'
              << MatchScore * strlen(adpt) + MaxMisMatchAllowed * MisMatchScore
              << "\n------\n";
    #endif
    if (score<(MatchScore * strlen(adpt)) + (MaxMisMatchAllowed * MisMatchScore)) return ret;

    int i = 0;
    while (row(alignment, 1)[i] != 'N') ++i;
    std::get<0>(ret) = i;
    while (row(alignment, 1)[++i] == 'N');
    std::get<1>(ret) = i--;
    while (row(alignment, 1)[++i] != '-');
    std::get<2>(ret) = i;
    return ret;
}

std::tuple<int, int, int> UmiByPosition::IdentifyUmi(const char *read, const char *adpt) {
    std::tuple<int, int, int> ret{0, 0, 0};
    std::get<2>(ret) = static_cast<int>(strlen(adpt));
    while (*adpt != 'N') ++adpt, ++std::get<0>(ret);
    std::get<1>(ret) = std::get<0>(ret);
    while (*adpt == 'N') ++adpt, ++std::get<1>(ret);
    return ret;
}
