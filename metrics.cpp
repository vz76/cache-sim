#include "metrics.h"
#include <cstdint>
#include <iostream>

using namespace std;

void Metrics::step(int category, bool isWrite)
{
    if (isWrite) // since we do writeback eviction for both L1 and L2, all writes are async, only calculate active energy in that level
    {
        if (category == 0) // DRAM
        {
            double count = 0.0;
            // totalJoules += DRAM_PENALTY_JOULES; // PENALTY OMITTED
            count += DRAM_ACTIVE_WATTS * WRITE_TIME;
            totalJoules += count;
            DRAMJoules += count;
            // totalJoules += TOTAL_IDLE_WATTS * WRITE_TIME; // ASYNC
            // totalSeconds += WRITE_TIME; // ASYNC
        }
        // else if (category == 1) // L1 - writebacks to L1 can never happen, write hits in L1 are assumed to not stall past the necessary read as per #150
        // {
        // }
        else if (category == 2) // L2
        {
            double count = 0.0;
            // totalJoules += L2_PENALTY_JOULES; // PENALTY OMITTED
            count += L2_ACTIVE_WATTS * WRITE_TIME;
            totalJoules += count;
            L2Joules += count;
            // totalJoules += TOTAL_IDLE_WATTS * WRITE_TIME; // ASYNC
            // totalSeconds += WRITE_TIME; // ASYNC
        }
    }
    else
    {
        if (category == 0) // DRAM
        {
            double count = 0.0;
            count += DRAM_PENALTY_JOULES;
            count += DRAM_ACTIVE_WATTS * DRAM_TIME;
            count += TOTAL_IDLE_WATTS * DRAM_TIME;
            DRAMJoules += count;
            totalJoules += count;
            totalSeconds += DRAM_TIME;
        }
        else if (category == 1) // L1
        {
            double count = 0.0;
            count += L1_ACTIVE_WATTS * L1_TIME;
            count += TOTAL_IDLE_WATTS * L1_TIME;
            L1Joules += count;
            totalJoules += count;
            totalSeconds += L1_TIME;
        }
        else if (category == 2) // L2
        {
            double count = 0.0;
            count += L2_PENALTY_JOULES;
            count += L2_ACTIVE_WATTS * L2_TIME;
            count += TOTAL_IDLE_WATTS * L2_TIME;
            L2Joules += count;
            totalJoules += count;
            totalSeconds += L1_TIME;
        }
    }
}

void Metrics::resetMetrics()
{
    totalJoules = 0.0;
    totalSeconds = 0.0;
}