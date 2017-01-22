#include "Dedup.hpp"

Dedupper *Dedupper::Generate(Dedupper::Methods m) {
    if (m == Dedupper::Methods::UNIQUE) return new UniqueDedupper;
    else return nullptr;
}

Dedupper::Dedupper(BamPairCompareMethods m)
    : comparer(GenerateBamPairCompare(m)) {
}

Dedupper::Dedupper(Dedupper&& other)
    : comparer(other.comparer) {
    other.comparer = nullptr;
}

Dedupper& Dedupper::operator=(Dedupper&& other) {
    if (this != &other) {
        std::swap(comparer, other.comparer);
    }
    return *this;
}

Dedupper::~Dedupper() {
    delete comparer;
}


std::vector<Dedupper::value_type> UniqueDedupper::Pick(const std::vector<Dedupper::value_type>& inputs, int umilen) {
    std::vector<Dedupper::value_type> bests;
    std::string umi;
    std::unordered_map<std::string, std::vector<Dedupper::value_type> > pools;
    for (const auto& p: inputs) {
        umi = GetBamUmi(p.first, umilen) + GetBamUmi(p.second, umilen);
        pools[umi].emplace_back(p);
    }
    Dedupper::value_type best;
    for (auto& pool : pools) {
        if (pool.second.size() > 0) {
            auto iter = pool.second.begin();
            best = *iter++;
            while (iter != pool.second.end()) {
                if ((*comparer)(*iter, best)) best = *iter;
                ++iter;
            }
            bests.push_back(best);
        }
    }
    return bests;
}
