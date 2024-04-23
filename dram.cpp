#include "dram.h"

array<uint32_t, 16> DRAM::read(uint32_t addr)
{
    metrics.step(0, false);

    readAccesses++;

    auto it = memory.find(addr);
    if (it != memory.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}

void DRAM::write(uint32_t addr, array<uint32_t, 16> val)
{
    metrics.step(0, true);

    writeAccesses++;

    memory[addr] = val;
}

void DRAM::clearMemory()
{
    memory.clear();
}