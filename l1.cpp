#include "l1.h"
#include "l2.h"

array<uint32_t, 16> L1::readLine(uint32_t addr, bool isData, bool isWrite) // assume addr is 16B aligned
{
    metrics.step(1, false); // cost for checking cache before write is considered as per Ed post #134

    uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1); // get the middle 9 bits
    uint32_t tag = addr >> (32 - TAG_BITS);                                   // get the first 17 bits
    auto &cache = isData ? data : instr;

    if (cache[index].valid && cache[index].tag == tag)
    {
        // cache hit
        return cache[index].values;
    }
    else
    {
        // cache miss, fetch from L2 and update our cache
        if (isData)
        {
            if (isWrite)
            {
                dataWriteMisses++;
            }
            else
            {
                dataReadMisses++;
            }
            dataMisses++;
        }
        else
        {
            instrMisses++;
        }

        if (cache[index].valid && cache[index].dirty)
        {
            l2.writeLine((cache[index].tag << 15 | index << 6), cache[index].values);
        }

        array<uint32_t, 16> line = l2.readLine(addr, isData, isWrite);
        cache[index].valid = true;
        cache[index].dirty = false;
        cache[index].values = line;
        cache[index].tag = tag;

        return cache[index].values;
    }
}
void L1::writeLine(uint32_t addr, array<uint32_t, 16> line, bool isData) // assume addr is 16B aligned and entry is in cache
{
    metrics.step(1, true);

    uint32_t index = addr >> OFFSET_BITS & (((uint32_t)1 << INDEX_BITS) - 1); // get the middle 9 bits
    uint32_t tag = addr >> (32 - TAG_BITS);                                   // get the first 17 bits
    auto &cache = isData ? data : instr;

    if (cache[index].tag == tag)
    {
        // tag already in cache
        cache[index].dirty = true;
        cache[index].valid = true;
        cache[index].values = line;
        cache[index].tag = tag;
    }
}
uint32_t L1::readItem(uint32_t addr, bool isData)
{
    uint32_t newaddr = (addr >> OFFSET_BITS) << OFFSET_BITS;
    return readLine(newaddr, isData, false)[(addr >> ALIGN_BITS) & (((uint32_t)1 << (OFFSET_BITS - ALIGN_BITS)) - 1)];
}
void L1::writeItem(uint32_t addr, uint32_t val, bool isData)
{
    uint32_t newaddr = (addr >> OFFSET_BITS) << OFFSET_BITS;
    array<uint32_t, 16> line = readLine(newaddr, isData, true);
    line[(addr >> ALIGN_BITS) & (((uint32_t)1 << (OFFSET_BITS - ALIGN_BITS)) - 1)] = val;
    writeLine(newaddr, line, isData);
}
