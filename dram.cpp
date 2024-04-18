#include "dram.h"

void DRAM::write(uint32_t addr, array<uint32_t, 16> val)
{
    memory[addr] = val;
}

array<uint32_t, 16> DRAM::read(uint32_t addr)
{
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