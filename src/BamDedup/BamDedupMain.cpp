#include "BamDedup.hpp"
#include <string>

using namespace std;

void usage(char *name) {
    fprintf(stderr, "This script parse bam file and remove duplicated UMI"
            "\nusage:\n"
            "%s \n"
            "\t-h\tShow help message\n"
            "\t-i\tInput bam file, need to be sorted by coordinate while having left and right reads together\n"
            "\t-o\tOutput bam file\n"
            "\t-l\tLength of UMI\n"
            , name
    );
}

int main(int argc, char **argv) {
    int c, umi_len = 5;
    char fstdin[] = "/dev/stdin";
    char fstdout[] = "/dev/stdout";
    char *infile = fstdin, *outfile = fstdout;
    while ((c = getopt(argc, argv, "i:o:l:h")) != -1) {
        switch (c) {
            case 'i':
                infile = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'l':
                umi_len = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                fprintf(stderr, "unknown option\n");
                exit(1);
        }
    }
    BamPeDedupper d(infile, outfile, umi_len);
    auto tw = d.Run();
    fprintf(stderr, "total_pairs:\t%d\n"
            "output_pairs:\t%d\n"
            "duplication_rate:\t%.3f\n"
            , tw.first
            , tw.second
            , double(tw.first - tw.second) / tw.first
    );
    return 0;
}