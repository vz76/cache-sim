#ifndef CACHE_H
#define CACHE_H

#include <array>
#include <cstdint>

using namespace std;

struct CacheResult
{
    bool valid;
    array<uint32_t, 16> line;
};

class Cache
{
    virtual ~Cache();
    virtual CacheResult readLine(uint32_t addr);                     // assume addr is 16B aligned
    virtual void writeLine(uint32_t addr, array<uint32_t, 16> line); // assume addr is 16B aligned
    virtual uint32_t readItem(uint32_t addr);
    virtual void writeItem(uint32_t addr, uint32_t val);
};

#endif // CACHE_H