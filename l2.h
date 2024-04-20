#ifndef L2_H
#define L2_H

#include "cache.h"
#include <unordered_map>
#include <iostream>
#include <random>

using namespace std;

struct L2Entry // 4096 direct-mapped 64B entries in 256KB cache
{
    bool valid;
    bool dirty;
    uint32_t tag;
    array<uint32_t, 16> values;
};

struct L2Set
{
    array<L2Entry, SET_ASSOCIATIVITY> entries;
};

class DRAM;

class L2
{
private:
    array<L2Set, 4096 / SET_ASSOCIATIVITY> data = {};
    static constexpr uint32_t OFFSET_BITS = 6;
    static constexpr uint32_t INDEX_BITS = 12 - log2i(SET_ASSOCIATIVITY); // S_A = 2 -> 11 index bits, S_A = 4 -> 10 index bits, S_A = 8 -> 9 index bits
    static constexpr uint32_t TAG_BITS = 32 - OFFSET_BITS - INDEX_BITS;
    std::mt19937 rd{SEED};
    std::uniform_int_distribution<uint32_t> dist{0, SET_ASSOCIATIVITY - 1}; // [start, end]
    DRAM &dram;

public:
    L2(DRAM &dramref) : dram(dramref){};
    array<uint32_t, 16> readLine(uint32_t addr);             // assume addr is 16B aligned
    void writeLine(uint32_t addr, array<uint32_t, 16> line); // assume addr is 16B aligned
};

#endif // L2_H