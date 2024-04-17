#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "dram.h"

using namespace std;

int main()
{
    // vector<string> paths = {
    //     "008.espresso.din", "013.spice2g6.din", "015.doduc.din", "022.li.din", "023.eqntott.din",
    //     "026.compress.din", "034.mdljdp2.din", "039.wave5.din", "047.tomcatv.din", "048.ora.din",
    //     "085.gcc.din", "089.su2cor.din", "090.hydro2d.din", "093.nasa7.din", "094.fpppp.din"};

    vector<string> paths = {"test.din"};

    for (const auto &path : paths)
    {
        string filePath = path;
        ifstream inputFile;
        inputFile.open(filePath);

        string line;
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
            { // memory read
            }
            else if (operation == 1)
            { // memory write
            }
            else if (operation == 2)
            { // instruction fetch
            }
            else if (operation == 3)
            { // ignore
                continue;
            }
            else if (operation == 4)
            { // flush cache
            }
        }
        inputFile.close();
    }

    return 0;
}
