#pragma once
#include "bam_reader.hpp"
#include <string>
#include <boost/functional/hash.hpp>
#include <unordered_map>

class BamPeDedupper
    : public BamFilePeReader {

protected:
    struct Coordinate {
        int32_t start;
        std::string umi;

        bool operator==(const Coordinate& other) const {
            return (start == other.start
                && umi == other.umi);
        }
    };

    struct CoordinateHasher {
        std::size_t operator()(const Coordinate& c) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, boost::hash_value(c.start));
            boost::hash_combine(seed, boost::hash_value(c.umi));
            return seed;
        }
    };

    using hash_t = std::unordered_map<Coordinate, bam1_t *, CoordinateHasher>;

protected:
    hash_t rec_;

public:
    BamPeDedupper(const char *filename);

    BamPeDedupper(BamPeDedupper&&);

    BamPeDedupper& operator=(BamPeDedupper&&);

    ~BamPeDedupper();

    bool IsSameRead() const override;
};