#include "dram.h"

void DRAM::write(uint32_t addr, uint32_t val)
{
    memory[addr] = val;
}

uint32_t DRAM::read(uint32_t addr)
{
    auto it = memory.find(addr);
    if (it != memory.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}