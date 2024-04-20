#ifndef L1_H
#define L1_H

#include "cache.h"
#include <unordered_map>
#include <iostream>

using namespace std;

struct L1Entry // 512 entries in 32KB cache
{
    bool valid;
    bool dirty;
    uint32_t tag; // split into [17 bits of tag] [9 bits of index] [6 bits of offset (all 0s)]
    array<uint32_t, 16> values;
};

class L2;

class L1
{
private:
    array<L1Entry, 512> data = {};
    array<L1Entry, 512> instr = {};
    L2 &l2;

    static constexpr uint32_t ALIGN_BITS = 2;
    static constexpr uint32_t OFFSET_BITS = 6;
    static constexpr uint32_t INDEX_BITS = 9; // 2 -> 11 index bits, 4 -> 10 index bits, 8 -> 9 index bits
    static constexpr uint32_t TAG_BITS = 17;

    array<uint32_t, 16> readLine(uint32_t addr, bool isData);             // assume addr is 16B aligned
    void writeLine(uint32_t addr, array<uint32_t, 16> line, bool isData); // assume addr is 16B aligned

public:
    L1(L2 &l2ref) : l2(l2ref){};
    uint32_t readItem(uint32_t addr, bool isData);
    void writeItem(uint32_t addr, uint32_t val, bool isData);
};

#endif // L1_H