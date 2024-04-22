#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "l1.h"
#include "l2.h"
#include "dram.h"
#include "metrics.h"

using namespace std;

double calculateMean(const std::vector<double> &values)
{
    double sum = 0.0;
    for (double value : values)
    {
        sum += value;
    }
    return sum / values.size();
}

double calculateStandardDeviation(const std::vector<double> &values)
{
    double mean = calculateMean(values);
    double sumSquaredDifferences = 0.0;
    for (double value : values)
    {
        double difference = value - mean;
        sumSquaredDifferences += difference * difference;
    }
    double variance = sumSquaredDifferences / values.size();
    return std::sqrt(variance);
}

template <uint32_t SET_ASSOCIATIVITY>
void runSimulation(string filePath, ofstream &outputFile, ofstream &outputLog, Metrics metrics)
{
    ifstream inputFile;
    DRAM dram{metrics};
    L2<SET_ASSOCIATIVITY> l2{dram, metrics};
    L1 l1{l2, metrics};
    ostream &output = outputFile;
    ostream &logput = outputLog;
    uint32_t totalInstructions = 0;
    output << "----------------------------------------------------------------" << endl;
    output << "Simulation Results for Set Associativity " << SET_ASSOCIATIVITY << ":" << endl;
    output << "----------------------------------------------------------------" << endl;
    vector<double> totalJoules = {};
    vector<double> totalL1Joules = {};
    vector<double> totalL2Joules = {};
    vector<double> totalDRAMJoules = {};
    vector<double> seconds = {};
    for (int i = 1; i <= 10; i++)
    {
        inputFile.open(filePath);
        string line;
        int read, write, instr = 0;
        while (getline(inputFile, line))
        {
            istringstream iss(line);
            int operation;
            uint32_t address;
            uint32_t value;

            if (!(iss >> operation >> hex >> address >> value))
            {
                cerr << "invalid input" << line << endl;
                continue;
            }

            if (operation == 0)
            {                               // memory read
                l1.readItem(address, true); // value should be 0?
                read++;
            }
            else if (operation == 1)
            { // memory write
                l1.writeItem(address, value, true);
                write++;
            }
            else if (operation == 2)
            {                                // instruction fetch
                l1.readItem(address, false); // ignore value?
                instr++;
            }
            else if (operation == 3)
            { // ignore
                continue;
            }
            else if (operation == 4)
            { // flush cache, ignore (per #139 on Ed)
                continue;
            }
        }
        inputFile.close();
        totalInstructions = (read + write + instr);

        logput << "----------------------------------------------------------------" << endl;
        logput << "Set Associativity " << SET_ASSOCIATIVITY << ", Run " << i << ":" << endl;
        logput << "Total Energy (J): " << metrics.totalJoules << endl;
        logput << "Total Time (s): " << metrics.totalSeconds << endl;
        logput << "----------------------------------------------------------------" << endl;

        logput << "Total Instructions Executed: " << totalInstructions << endl;
        logput << "Reads (0): " << read << endl;
        logput << "Writes (1): " << write << endl;
        logput << "Fetches (2): " << instr << endl;
        logput << "----------------------------------------------------------------" << endl;

        logput << "L1 Instruction Cache Fetches: " << instr << endl;
        logput << "L1 Instruction Cache Misses: " << l1.instrMisses << endl;
        logput << "----------------------------------------------------------------" << endl;

        logput << "L1 Data Cache Fetches: " << (read + write) << endl;
        logput << "L1 Data Cache Misses (Read): " << l1.dataReadMisses << endl;
        logput << "L1 Data Cache Misses (Write): " << l1.dataWriteMisses << endl;
        logput << "----------------------------------------------------------------" << endl;

        logput << "L2 Cache Fetches (Instruction): " << (l2.instrFetches) << endl;
        logput << "L2 Cache Fetches (Read): " << (l2.readFetches) << endl;
        logput << "L2 Cache Fetches (Write): " << (l2.writeFetches) << endl;

        logput << "L2 Cache Misses (Instruction): " << (l2.instrMisses) << endl;
        logput << "L2 Cache Misses (Read): " << (l2.readMisses) << endl;
        logput << "L2 Cache Misses (Write): " << (l2.writeMisses) << endl;
        logput << "----------------------------------------------------------------" << endl;
        logput << endl;

        read = 0;
        write = 0;
        instr = 0;
        l1.instrMisses = 0;
        l1.dataReadMisses = 0;
        l1.dataWriteMisses = 0;
        l2.instrFetches = 0;
        l2.readFetches = 0;
        l2.writeFetches = 0;
        l2.instrMisses = 0;
        l2.readMisses = 0;
        l2.writeMisses = 0;

        l1.flushCache();
        totalJoules.push_back(metrics.totalJoules);
        totalDRAMJoules.push_back(metrics.DRAMJoules);
        totalL1Joules.push_back(metrics.L1Joules);
        totalL2Joules.push_back(metrics.L2Joules);
        seconds.push_back(metrics.totalSeconds);
        metrics.resetMetrics();
    }
    output << "Mean Energy (J): " << calculateMean(totalJoules) << endl;
    output << "StDev Energy (J): " << calculateStandardDeviation(totalJoules) << endl;
    output << "Mean L1 Energy (J): " << calculateMean(totalL1Joules) << endl;
    output << "Mean L2 Energy (J): " << calculateMean(totalL2Joules) << endl;
    output << "Mean DRAM Energy (J): " << calculateMean(totalDRAMJoules) << endl;
    output << endl;

    output << "Mean Time (s): " << calculateMean(seconds) << endl;
    output << "StDev Time (s): " << calculateStandardDeviation(seconds) << endl;
    output << "Mean Time per Instruction (ns): " << calculateMean(seconds) / totalInstructions * 1e9 << endl;
    output << endl;
}

int main()
{
    vector<string> paths = {
        "008.espresso.din", "013.spice2g6.din", "015.doduc.din", "022.li.din", "023.eqntott.din",
        "026.compress.din", "034.mdljdp2.din", "039.wave5.din", "047.tomcatv.din", "048.ora.din",
        "085.gcc.din", "089.su2cor.din", "090.hydro2d.din", "093.nasa7.din", "094.fpppp.din"};

    // vector<string> paths = {"026.compress.din"}; // USED FOR TESTING

    Metrics metrics;

    for (const auto &path : paths)
    {
        string filePath = "./traces/" + path;
        ofstream outputLog("./logs/" + path.substr(0, path.size() - 4) + ".log");
        ofstream outputFile("./outputs/" + path.substr(0, path.size() - 4) + ".out");

        cout << "Executing for " << path << endl;

        runSimulation<2>(filePath, outputFile, outputLog, metrics);
        runSimulation<4>(filePath, outputFile, outputLog, metrics);
        runSimulation<8>(filePath, outputFile, outputLog, metrics);
    }
    cout << "Execution Complete." << endl;

    return 0;
}