#pragma once

#include <cstdint>

namespace cr {

    
    inline uint32_t hmix(uint32_t a, uint32_t b) {
        uint32_t h = a ^ (b + 0x9e3779b9u + (a << 6) + (a >> 2));
        
        h ^= h >> 16; h *= 0x85ebca6bu;
        h ^= h >> 13; h *= 0xc2b2ae35u;
        h ^= h >> 16;

        
        return h;
    }

}
