#ifndef CACHE_H
#define CACHE_H

#include <array>
#include <cstdint>

using namespace std;

constexpr uint32_t log2i(uint32_t v) // helper function to find log2
{
    unsigned int r = 0;

    while (v >>= 1)
    {
        r++;
    }
    return r;
}

constexpr uint32_t SEED = 7777; // used for random cache eviction

#endif // CACHE_H