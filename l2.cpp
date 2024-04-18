#include "l2.h"

array<uint32_t, 16> L2::readLine(uint32_t addr) // assume addr is 16B aligned
{
    uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1);
    uint32_t tag = addr >> (32 - TAG_BITS);
    L2Set &set = data[index];

    for (auto &entry : set.entries)
    {
        if (entry.valid && entry.tag == tag)
        {
            // cache hit

            return entry.values;
        }
    }

    // cache miss, evict to memory, fetch from memory, and update our cache

    // if(data[index].valid){
    //     L2::writeLine((data[index].tag << 15 | index), line);
    // }

    // random evict to memory
    uint32_t evictIndex = dist(rd);
    L2Entry &evictEntry = set.entries[evictIndex];
    if (evictEntry.valid)
    {
        // DRAM::write((evictEntry.tag << 15 | index << OFFSET_BITS), evictEntry.values);
    }

    // array<uint32_t, 16> line = DRAM::read(addr);
    evictEntry.valid = true;
    // evictEntry.values = line;
    evictEntry.tag = tag;
}

void L2::writeLine(uint32_t addr, array<uint32_t, 16> line) // assume addr is 16B aligned and entry is in cache
{
    uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1);
    uint32_t tag = addr >> (32 - TAG_BITS);

    L2Set &set = data[index];

    for (auto &entry : set.entries)
    {
        if (entry.tag == tag)
        {
            // tag already in cache

            entry.valid = true;
            entry.values = line;
            entry.tag = tag;
        }
    }
}