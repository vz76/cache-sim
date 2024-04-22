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
void runSimulation(string filePath, ofstream &outputFile, Metrics metrics)
{
    ifstream inputFile;
    DRAM dram{metrics};
    L2<SET_ASSOCIATIVITY> l2{dram, metrics};
    L1 l1{l2, metrics};
    ostream &output = outputFile;
    output << "----------------------------------------------------------------" << endl;
    output << "Simulation Results for Set Associativity " << SET_ASSOCIATIVITY << ":" << endl;
    output << "----------------------------------------------------------------" << endl;
    vector<double> joules = {};
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
        // cout << "Simulation for " << path << " terminated." << endl;
        // output << "----------------------------------------------------------------" << endl;

        // output << "Total Instructions Executed: " << (read + write + instr) << endl;
        // output << "Reads (0): " << read << endl;
        // output << "Writes (1): " << write << endl;
        // output << "Fetches (2): " << instr << endl;
        // output << "----------------------------------------------------------------" << endl;

        // output << "L1 Instruction Cache Fetches: " << instr << endl;
        // output << "L1 Instruction Cache Misses: " << l1.instrMisses << endl;
        // output << "----------------------------------------------------------------" << endl;

        // output << "L1 Data Cache Fetches: " << (read + write) << endl;
        // output << "L1 Data Cache Misses (Read): " << l1.dataReadMisses << endl;
        // output << "L1 Data Cache Misses (Write): " << l1.dataWriteMisses << endl;
        // output << "----------------------------------------------------------------" << endl;

        // output << "L2 Cache Fetches (Instruction): " << (l2.instrFetches) << endl;
        // output << "L2 Cache Fetches (Read): " << (l2.readFetches) << endl;
        // output << "L2 Cache Fetches (Write): " << (l2.writeFetches) << endl;

        // output << "L2 Cache Misses (Instruction): " << (l2.instrMisses) << endl;
        // output << "L2 Cache Misses (Read): " << (l2.readMisses) << endl;
        // output << "L2 Cache Misses (Write): " << (l2.writeMisses) << endl;
        // output << "----------------------------------------------------------------" << endl;
        output << "Run " << i << ":" << endl;
        output << "Total Energy (J): " << metrics.totalJoules << endl;
        output << "Total Time (s): " << metrics.totalSeconds << endl;
        output << "----------------------------------------------------------------" << endl;
        // read = 0;
        // write = 0;
        // instr = 0;
        // l1.instrMisses = 0;
        // l1.dataReadMisses = 0;
        // l1.dataWriteMisses = 0;
        // l2.instrFetches = 0;
        // l2.readFetches = 0;
        // l2.writeFetches = 0;
        // l2.instrMisses = 0;
        // l2.readMisses = 0;
        // l2.writeMisses = 0;
        l1.flushCache();
        joules.push_back(metrics.totalJoules);
        seconds.push_back(metrics.totalSeconds);
        metrics.resetMetrics();
    }
    output << "Mean Energy (J): " << calculateMean(joules) << endl;
    output << "StDev Energy (J): " << calculateStandardDeviation(joules) << endl;
    output << "Mean Time (s): " << calculateMean(seconds) << endl;
    output << "StDev Time (s): " << calculateStandardDeviation(seconds) << endl;
    output << endl;
}

int main()
{
    // vector<string> paths = {
    //     "008.espresso.din", "013.spice2g6.din", "015.doduc.din", "022.li.din", "023.eqntott.din",
    //     "026.compress.din", "034.mdljdp2.din", "039.wave5.din", "047.tomcatv.din", "048.ora.din",
    //     "085.gcc.din", "089.su2cor.din", "090.hydro2d.din", "093.nasa7.din", "094.fpppp.din"};

    vector<string> paths = {"026.compress.din"};

    Metrics metrics;

    for (const auto &path : paths)
    {
        string filePath = "./traces/" + path;
        ofstream outputFile("./outputs/" + path.substr(0, path.size() - 4) + ".out");

        cout << "Simulating for " << path << endl;

        runSimulation<2>(filePath, outputFile, metrics);
        runSimulation<4>(filePath, outputFile, metrics);
        runSimulation<8>(filePath, outputFile, metrics);
    }
    cout << "Execution Complete." << endl;

    return 0;
}