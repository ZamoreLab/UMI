#pragma once

#include <htslib/sam.h>
#include <math.h>
#include <string>
#include <cstring>

class BamFileReader {
protected:
    htsFile *bamfh_ = nullptr;
    bam_hdr_t *header_ = nullptr;
    bool good_ = false;

    static bool _IsPassPhred(bam1_t *b, uint8_t minhpred);

    static bool _IsPassMapq(bam1_t *b, int minmapq);

    static int32_t _GetStart(bam1_t *b);

    static int32_t _GetEnd(bam1_t *b);

    static int32_t _GetFlag(bam1_t *b);

    static uint32_t *_GetCigar(bam1_t *b);

    static uint32_t _GetCigarLen(bam1_t *b);

    static std::string _GetChr(bam1_t *b, bam_hdr_t *h);

    static bool _IsPlusStrand(bam1_t *b);

public:
    BamFileReader() {};

    explicit BamFileReader(const char *bamfile);

    BamFileReader(const BamFileReader&) = delete;

    BamFileReader(BamFileReader&& other);

    BamFileReader& operator=(const BamFileReader&) = delete;

    BamFileReader& operator=(BamFileReader&& other);

    ~BamFileReader() {
        bam_hdr_destroy(header_);
        sam_close(bamfh_);
    }

    bool Open(const char *bamfile);

    virtual bool IsInitiated() const;

    virtual bool IsAligned() const = 0;

    virtual bool IsGood() const noexcept;

    virtual bool Next() = 0;

    virtual bool NextAligned() = 0;

    virtual std::string GetName() const = 0;

    virtual std::string GetChr() const = 0;

    bam_hdr_t *GetHeader();

    const bam_hdr_t *GetHeader() const;

    virtual bool IsPassPhred(uint8_t minphred) const = 0;

    virtual bool IsPassMapq(uint8_t minmapq) const = 0;

};


class BamFileSeReader
    : public BamFileReader {
protected:
    bam1_t *b_ = nullptr;
public:
    BamFileSeReader()
        : BamFileReader() {
        b_ = bam_init1();
    }

    explicit BamFileSeReader(const char *bamfile);

    BamFileSeReader(const BamFileSeReader&) = delete;

    BamFileSeReader(BamFileSeReader&& other);

    BamFileSeReader& operator=(const BamFileSeReader&) = delete;

    BamFileSeReader& operator=(BamFileSeReader&& other);

    ~BamFileSeReader() {
        bam_destroy1(b_);
    }

    virtual bool IsInitiated() const override;

    virtual bool IsAligned() const override;

    virtual bool Next() override;

    virtual bool NextAligned() override;

    virtual std::string GetName() const override;

    int32_t GetStart() const;

    int32_t GetEnd() const;

    int32_t GetFlag() const;

    uint32_t *GetCigar() const;

    uint32_t GetCigarLen() const;

    std::string GetSeq() const;

    std::string GetChr() const override;

    bam1_t *GetBam();

    const bam1_t *GetBam() const;

    virtual bool IsPassPhred(uint8_t minphred) const override;

    virtual bool IsPassMapq(uint8_t minmapq) const override;

    bool IsPlusStrand() const;

};


class BamFilePeReader
    : public BamFileReader {
protected:
    bam1_t *b1_ = nullptr;
    bam1_t *b2_ = nullptr;

public:
    BamFilePeReader()
        : BamFileReader() {
        b1_ = bam_init1();
        b2_ = bam_init1();
    }

    explicit BamFilePeReader(const char *bamfile);

    BamFilePeReader(const BamFilePeReader&) = delete;

    BamFilePeReader(BamFilePeReader&& other);

    BamFilePeReader& operator=(const BamFilePeReader&) = delete;

    BamFilePeReader& operator=(BamFilePeReader&& other);

    ~BamFilePeReader() {
        bam_destroy1(b1_);
        bam_destroy1(b2_);
    }

    virtual bool Next() override;

    virtual bool NextAligned() override;

    virtual std::string GetName() const override;

    virtual std::string GetChr() const override;

    virtual bool IsAligned() const override;

    virtual bool IsPassPhred(uint8_t minphred) const override;

    virtual bool IsPassMapq(uint8_t minphred) const override;

    std::string GetSeq1() const;

    std::string GetSeq2() const;

    int32_t GetStart1() const;

    int32_t GetEnd1() const;

    int32_t GetStart2() const;

    int32_t GetEnd2() const;

    int32_t GetFlag1() const;

    int32_t GetFlag2() const;

    uint32_t *GetCigar1() const;

    uint32_t *GetCigar2() const;

    uint32_t GetCigarLen1() const;

    uint32_t GetCigarLen2() const;

    virtual bool IsSameRead() const;

    bool IsAlignedOppoStrand() const;

    bool IsAppropriatelyAligned() const;

    void EnsureDirection();

    void SwapPair();

    std::pair<bam1_t *, bam1_t *> GetPair();

    std::pair<bam1_t *, bam1_t *> MovePair();

    bool IsOverlapped() const { return GetEnd1() > GetStart2(); }

    int OverlapBy() const { return GetEnd1() - GetStart2(); }

};