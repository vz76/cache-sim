#ifndef DRAM_H
#define DRAM_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>

using namespace std;

class DRAM
{
private:
    unordered_map<uint32_t, uint32_t> memory;

public:
    void write(uint32_t addr, uint32_t val);
    uint32_t read(uint32_t addr);
};

#endif // DRAM_H
