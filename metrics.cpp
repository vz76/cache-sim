#include "metrics.h"
#include <cstdint>
#include <iostream>

using namespace std;

void Metrics::step(int category, bool isWrite)
{
    if (isWrite)
    {
        if (category == 0) // DRAM
        {
            totalJoules += DRAM_PENALTY_JOULES;
            totalJoules += DRAM_ACTIVE_WATTS * WRITE_TIME;
            totalJoules += TOTAL_IDLE_WATTS * WRITE_TIME;
            totalSeconds += WRITE_TIME;
        }
        else if (category == 1) // L1
        {
            totalJoules += L1_ACTIVE_WATTS * WRITE_TIME;
            totalJoules += TOTAL_IDLE_WATTS * WRITE_TIME;
            totalSeconds += WRITE_TIME;
        }
        else if (category == 2) // L2
        {
            totalJoules += L2_PENALTY_JOULES;
            totalJoules += L2_ACTIVE_WATTS * WRITE_TIME;
            totalJoules += TOTAL_IDLE_WATTS * WRITE_TIME;
            totalSeconds += WRITE_TIME;
        }
    }
    else
    {
        if (category == 0) // DRAM
        {
            totalJoules += DRAM_PENALTY_JOULES;
            totalJoules += DRAM_ACTIVE_WATTS * DRAM_TIME;
            totalJoules += TOTAL_IDLE_WATTS * DRAM_TIME;
            totalSeconds += DRAM_TIME;
        }
        else if (category == 1) // L1
        {
            totalJoules += L1_ACTIVE_WATTS * L1_TIME;
            totalJoules += TOTAL_IDLE_WATTS * L1_TIME;
            totalSeconds += L1_TIME;
        }
        else if (category == 2) // L2
        {
            totalJoules += L2_PENALTY_JOULES;
            totalJoules += L2_ACTIVE_WATTS * L2_TIME;
            totalJoules += TOTAL_IDLE_WATTS * L2_TIME;
            totalSeconds += L1_TIME;
        }
    }
}

void Metrics::resetMetrics()
{
    totalJoules = 0.0;
    totalSeconds = 0.0;
}