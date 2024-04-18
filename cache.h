#ifndef CACHE_H
#define CACHE_H

#include <array>
#include <cstdint>

using namespace std;

class Cache
{
    virtual ~Cache();
    virtual uint32_t readItem(uint32_t addr) = 0;
    virtual void writeItem(uint32_t addr, uint32_t val) = 0;
};

#endif // CACHE_H