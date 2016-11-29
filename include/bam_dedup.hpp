#pragma once
#include "bam_reader.hpp"
#include <string>
#include <boost/functional/hash.hpp>
#include <unordered_map>

class BamPeDedupper
    : public BamFilePeReader {
private:
    using base = BamFilePeReader;
    using self = BamPeDedupper;

protected:
    struct Coordinate {
        int32_t start;
        std::string chr;
        std::string umi1;
        std::string umi2;

        bool operator==(const Coordinate& other) const {
            return (chr == other.chr
                && start == other.start
                && umi1 == other.umi1
                && umi2 == other.umi2
            );
        }
    };

    struct CoordinateHasher {
        std::size_t operator()(const Coordinate& c) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, boost::hash_value(c.start));
            boost::hash_combine(seed, boost::hash_value(c.chr));
            boost::hash_combine(seed, boost::hash_value(c.umi1));
            boost::hash_combine(seed, boost::hash_value(c.umi2));
            return seed;
        }
    };

    using hash_t = std::unordered_map<Coordinate, std::pair<bam1_t *, bam1_t *>, CoordinateHasher>;

    static bool _BamQualCmp(bam1_t *a1, bam1_t *a2, bam1_t *b1, bam1_t *b2);

protected:
    hash_t rec_;
    samFile *out_ = nullptr;
    int umilen_ = 0;
public:
    explicit BamPeDedupper(const char *filename, const char *outname = "-", int l = 5);

    BamPeDedupper(BamPeDedupper&&);

    BamPeDedupper& operator=(BamPeDedupper&&);

    ~BamPeDedupper();

    bool IsSameRead() const override;

    virtual bool NextAligned() override;

    int Write();

};
