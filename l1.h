#ifndef L1_H
#define L1_H

#include "cache.h"
#include <unordered_map>
#include <iostream>

using namespace std;

struct L1Entry // 512 entries in 32KB cache
{
    bool valid;
    uint32_t tag; // split into [17 bits of tag] [9 bits of index] [6 bits of offset (all 0s)]
    array<uint32_t, 16> values;
};

class L1 : public Cache
{
private:
    array<L1Entry, 512> data = {};

    array<uint32_t, 16> readLine(uint32_t addr, bool isData);             // assume addr is 16B aligned
    void writeLine(uint32_t addr, array<uint32_t, 16> line, bool isData); // assume addr is 16B aligned

public:
    virtual ~L1();
    virtual uint32_t readItem(uint32_t addr, bool isData);
    virtual void writeItem(uint32_t addr, uint32_t val, bool isData);
};

#endif // L1_H