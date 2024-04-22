#ifndef L2_H
#define L2_H

#include "cache.h"
#include "metrics.h"
#include "l2.h"
#include "dram.h"

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

template <uint32_t N>
struct L2Set
{
    array<L2Entry, N> entries;
};

class L2Interface
{
public:
    uint32_t instrFetches = 0, readFetches = 0, writeFetches = 0;
    uint32_t instrMisses = 0, readMisses = 0, writeMisses = 0;
    virtual array<uint32_t, 16> readLine(uint32_t addr, bool isData, bool isWrite) = 0; // assume addr is 16B aligned
    virtual void writeLine(uint32_t addr, array<uint32_t, 16> line) = 0;                // assume addr is 16B aligned
    virtual void flushCache() = 0;
};

class DRAM;

template <uint32_t N>
class L2 : public L2Interface
{
private:
    array<L2Set<N>, 4096 / N> data = {};
    static constexpr uint32_t OFFSET_BITS = 6;
    static constexpr uint32_t INDEX_BITS = 12 - log2i(N); // S_A = 2 -> 11 index bits, S_A = 4 -> 10 index bits, S_A = 8 -> 9 index bits
    static constexpr uint32_t TAG_BITS = 32 - OFFSET_BITS - INDEX_BITS;
    std::mt19937 rd{std::random_device()()};                // for deterministic output, change to use rd{SEED}
    std::uniform_int_distribution<uint32_t> dist{0, N - 1}; // [start, end]
    DRAM &dram;
    Metrics &metrics;

public:
    L2(DRAM &dramref, Metrics &metricsref) : dram(dramref), metrics(metricsref){};
    array<uint32_t, 16> readLine(uint32_t addr, bool isData, bool isWrite) // assume addr is 16B aligned
    {
        metrics.step(2, false);
        if (isData)
        {
            if (isWrite)
            {
                writeFetches++;
            }
            else
            {
                readFetches++;
            }
        }
        else
        {
            instrFetches++;
        }

        uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1);
        uint32_t tag = addr >> (32 - TAG_BITS);
        L2Set<N> &set = data[index];

        for (auto &entry : set.entries)
        {
            if (entry.valid && entry.tag == tag)
            {
                // cache hit

                return entry.values;
            }
        }

        // cache miss, evict to memory, fetch from memory, and update our cache
        if (isData)
        {
            if (isWrite)
            {
                writeMisses++;
            }
            else
            {
                readMisses++;
            }
        }
        else
        {
            instrMisses++;
        }

        // random evict to memory
        uint32_t evictIndex = dist(rd);
        L2Entry &evictEntry = set.entries[evictIndex];
        if (evictEntry.valid && evictEntry.dirty)
        {
            dram.write((evictEntry.tag << 15 | index << OFFSET_BITS), evictEntry.values);
        }

        array<uint32_t, 16> line = dram.read(addr);
        evictEntry.valid = true;
        evictEntry.dirty = false;
        evictEntry.values = line;
        evictEntry.tag = tag;

        return line;
    }

    void writeLine(uint32_t addr, array<uint32_t, 16> line) // assume addr is 16B aligned and entry is in cache
    {
        metrics.step(2, true);
        uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1);
        uint32_t tag = addr >> (32 - TAG_BITS);

        L2Set<N> &set = data[index];

        for (auto &entry : set.entries)
        {
            if (entry.tag == tag)
            {
                // tag already in cache

                entry.valid = true;
                entry.dirty = true;
                entry.values = line;
                entry.tag = tag;
            }
        }
    }

    void flushCache()
    {
        data = {};
        dram.clearMemory();
    }
};

#endif // L2_H