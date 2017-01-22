#include <iostream>
#include <vector>
#include <algorithm>
#include "BamReader.hpp"

using namespace std;

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Sort Pair-End bam file by their coordinates while keeping the pairs together\n"
            "\nusage:\n"
            "\t\t%s  input.pe.bam output.bam\n", argv[0]);
        return 1;
    }

    BamFilePeReader br{argv[1]};

    samFile *out = sam_open(argv[2], "wb");
    if (sam_hdr_write(out, br.GetHeader()) != 0) {
        fprintf(stderr, "Failed to open %s to write\n", argv[2]);
        return 1;
    }

    using PairedBam = pair<bam1_t *, bam1_t *>;
    vector<PairedBam> pool;
    while (br.NextAligned()) {
        pool.push_back(br.MovePair());
    }
    sort(pool.begin(), pool.end(), [](const PairedBam& a, const PairedBam& b) -> bool {
        if (a.first->core.tid == b.first->core.tid)
            return a.first->core.pos < b.first->core.pos;
        else
            return a.first->core.tid < b.first->core.tid;
    });

    for (const auto& p : pool) {
        sam_write1(out, br.GetHeader(), p.first);
        sam_write1(out, br.GetHeader(), p.second);
    }
    sam_close(out);
    return 0;
}