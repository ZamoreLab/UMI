#include "SeqReader.hpp"
#include "UmiClipper.hpp"
#include <getopt.h>

void usage(char *name) {
    fprintf(stderr, "This script clip UMI and attach it to the header"
            "\nusage:\n"
            "%s \n"
            "\t-h\tShow help message\n"
            "\t-i\tInput fastq file\n"
            "\t-o\tOutput fastq file\n"
            "\t-l\tLength of UMI\n"
            "\t-p\tLength of UMI padding\n"
            , name
    );
}

int main(int argc, char **argv) {
    int c, umi_len = 0, umi_pad = 0;
    char fstdin[] = "/dev/stdin";
    char fstdout[] = "/dev/stdout";
    char *infile = fstdin, *outfile = fstdout;
    while ((c = getopt(argc, argv, "i:o:l:p:h")) != -1) {
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
            case 'p':
                umi_pad = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                fprintf(stderr, "unknown option\n");
                exit(1);
        }
    }

    UmiClipper clipper(infile, outfile, umi_len, umi_pad);
    if (!clipper.IsGood()) {
        fprintf(stderr, "Cannot create UmiClipper, please check %s and %s\n", infile, outfile);
        goto err;
    }
    while (clipper.Clip());
    return 0;

    err:
    return 1;
}