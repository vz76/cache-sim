#ifndef DRAM_H
#define DRAM_H

#include "metrics.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <array>

using namespace std;

class DRAM
{
private:
    unordered_map<uint32_t, array<uint32_t, 16>> memory = {};
    Metrics &metrics;

public:
    DRAM(Metrics &metricsref) : metrics(metricsref){};
    void write(uint32_t addr, array<uint32_t, 16> val);
    array<uint32_t, 16> read(uint32_t addr);
    void clearMemory();
};

#endif // DRAM_H
