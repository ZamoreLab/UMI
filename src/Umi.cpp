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

std::pair<int, int> UmiByAlignment::IdentifyUmi(const char *read, const char* adpt) {
    using namespace seqan;
    Align<Dna5String, ArrayGaps> alignment;
    resize(rows(alignment), 2);
    assignSource(row(alignment, 0), read);
    assignSource(row(alignment, 1), adpt);
    Score<int, ScoreMatrix<Dna5, NAsMatchMatrix> > scoringScheme(-6, -2);

    #ifdef GLOBAL_ALIGNMENT
    AlignConfig<true, true, true, true> alignConfig;
    int score = globalAlignment(alignment, scoringScheme, alignConfig);
    #else
    int score = localAlignment(alignment, scoringScheme, DynamicGaps());
    #endif

}
