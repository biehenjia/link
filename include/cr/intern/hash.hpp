#pragma once

#include <cstdint>

namespace cr {

    inline uint64_t hmix64(uint64_t x) {
        x ^= x >> 33; x *= 0xff51afd7ed558ccdULL;
        x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL;
        x ^= x >> 33;
        return x;
    }

    inline uint64_t hfold(uint64_t state, uint64_t word) {
        return hmix64(state ^ word);
    }

}
