#include "SeqReader.hpp"
#include "UmiClipper.hpp"
#include <getopt.h>
#include <string>

void usage(char *name) {
    fprintf(stderr, "This script clip UMI and attach it to the header"
            "\nusage:\n"
            "%s \n"
            "\t-h\tShow help message\n"
            "\t-i\t(Single-End) Input fastq file\n"
            "\t-o\t(Single-End) Output fastq file\n"
            "\t-p\t(Single-End) UMI adaptor pattern, use N for UMI, such as AAGGTTCANNNNNNNNNNNNGGG\n"

            "\t-l\t(Pair-End) Input \\1 fastq file\n"
            "\t-r\t(Pair-End) Input \\2 fastq file\n"
            "\t-L\t(Pair-End) Output fastq file for \\1\n"
            "\t-R\t(Pair-End) Output fastq file for \\2\n"
            "\t-1\t(Pair-End) UMI adaptor pattern \\1\n"
            "\t-2\t(Pair-End) UMI adaptor pattern \\2\n"
            , name
    );
}

int main(int argc, char **argv) {
    int c;
    char fstdin[] = "/dev/stdin";
    char fstdout[] = "/dev/stdout";
    char *infile = fstdin, *outfile = fstdout;
    char *lfqfile{nullptr}, *rfqfile{nullptr};
    char *loutfile{nullptr}, *routfile{nullptr};
    std::string umi_pattern, lumi_pattern, rumi_pattern;
    while ((c = getopt(argc, argv, "i:l:r:o:L:R:p:1:2:h")) != -1) {
        switch (c) {
            case 'i':
                infile = optarg;
                break;
            case 'l':
                lfqfile = optarg;
                break;
            case 'r':
                rfqfile = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'L':
                loutfile = optarg;
                break;
            case 'R':
                routfile = optarg;
                break;
            case 'p':
                umi_pattern = optarg;
                break;
            case '1':
                lumi_pattern = optarg;
                break;
            case '2':
                rumi_pattern = optarg;
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                fprintf(stderr, "unknown option\n");
                exit(1);
        }
    }
    int total{0}, found{0};
    if (lfqfile == nullptr && rfqfile == nullptr) {
        /* single-end */
        UmiClipper clipper(infile, outfile, umi_pattern.c_str());
        if (!clipper.IsGood()) {
            fprintf(stderr, "Cannot create UmiClipper, please check %s and %s\n", infile, outfile);
            goto err;
        }
        while (clipper.IsGood()) ++total, found += clipper.Clip();
    } else {
        /* pair-end */
        if (loutfile == nullptr || routfile == nullptr) {
            fprintf(stderr, "Please specify output files in Pair-End mode\n");
            goto err;
        }
        if (lumi_pattern.empty() || rumi_pattern.empty()) {
            if (umi_pattern.empty()) {
                fprintf(stderr, "Please specify umi pattern for \\1 and \\2 in Pair-End mode\n");
                goto err;
            } else {
                lumi_pattern = rumi_pattern = umi_pattern;
            }
        }
        UmiClipper lclilpper(lfqfile, loutfile, lumi_pattern.c_str());
        UmiClipper rclilpper(rfqfile, routfile, rumi_pattern.c_str());
        PairEndUmiClipper clipper(&lclilpper, &rclilpper);
        while (clipper.IsGood()) ++total, found += clipper.Clip();
    }
    fprintf(stderr, "Total\t%d\nFound\t%d\n", total, found);
    return 0;

    err:
    return 1;
}