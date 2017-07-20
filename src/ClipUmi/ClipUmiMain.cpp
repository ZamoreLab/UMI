#include "SeqReader.hpp"
#include "UmiClipper.hpp"
#include <getopt.h>
#include <string>

void usage(char *name) {
    fprintf(stderr, "This script clip UMI and attach it to the header"
            "\nusage:\n"
            "%s \n"
            "\t-h\tShow help message\n"
            "\t-i\tInput fastq file\n"
            "\t-o\tOutput fastq file\n"
            "\t-p\tUMI adaptor pattern, use N for UMI, such as AAGGTTCANNNNNNNNNNNNGGG\n"
            , name
    );
}

int main(int argc, char **argv) {
    int c;
    char fstdin[] = "/dev/stdin";
    char fstdout[] = "/dev/stdout";
    char *infile = fstdin, *outfile = fstdout;
    std::string umi_pattern;
    while ((c = getopt(argc, argv, "i:o:p:h")) != -1) {
        switch (c) {
            case 'i':
                infile = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'p':
                umi_pattern = optarg;
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                fprintf(stderr, "unknown option\n");
                exit(1);
        }
    }

    UmiClipper clipper(infile, outfile, umi_pattern.c_str());
    if (!clipper.IsGood()) {
        fprintf(stderr, "Cannot create UmiClipper, please check %s and %s\n", infile, outfile);
        goto err;
    }
    while (clipper.Clip());
    return 0;

    err:
    return 1;
}