#pragma once

#include <string>
#include <zlib.h>
#include <htslib/kseq.h>

KSEQ_INIT(gzFile, gzread)

class Reader {
protected:
    gzFile gzfh_ = Z_NULL;
    kseq_t *seq_ = NULL;
    bool good_ = false;
public:
    explicit Reader(const char *);

    explicit Reader(const std::string&);

    ~Reader();

    Reader(const Reader&) = delete;

    Reader(Reader&&);

    Reader& operator=(const Reader&) = delete;

    Reader& operator=(Reader&&);

    void Open(const char*);

    bool IsGood() const noexcept;

    bool Next();

    char *GetName();

    std::string GetName() const;

    char *GetSeq();

    std::string GetSeq() const;

    char *GetQual();

    std::string GetQual() const;

};