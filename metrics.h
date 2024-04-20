#ifndef METRICS_H
#define METRICS_H

#include <array>
#include <cstdint>

using namespace std;

class Metrics
{
    constexpr static double TOTAL_IDLE_WATTS = 0.5 + 0.8 + 0.8;
    // active is on top of idle
    constexpr static double L1_ACTIVE_WATTS = 0.5;
    constexpr static double L2_ACTIVE_WATTS = 1.2;
    constexpr static double DRAM_ACTIVE_WATTS = 3.2;

    constexpr static double L2_PENALTY_JOULES = 5e-12;
    constexpr static double DRAM_PENALTY_JOULES = 640e-12;

    constexpr static double L1_TIME = 5e-10;
    constexpr static double L2_TIME = 5e-9 - L1_TIME;
    constexpr static double DRAM_TIME = 5e-8 - L2_TIME;

    double totalJoules = 0.0;

    void step(int category);
    double returnTotal();
};

#endif // METRICS_H