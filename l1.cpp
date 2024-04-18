#include "l1.h"

// TODO: IMPLEMENT DATA / INSTR DIFFERENCE, NO MORE MAGIC NUMBERS

array<uint32_t, 16> L1::readLine(uint32_t addr, bool isData) // assume addr is 16B aligned
{
    uint32_t index = addr >> 6 & 0b111111111; // get the middle 9 bits
    uint32_t tag = addr >> 15;                // get the first 17 bits
    if (data[index].valid && data[index].tag == tag)
    {
        // cache hit

        return data[index].values;
    }
    else
    {
        // cache miss, fetch from L2 and update our cache

        // if(data[index].valid){
        //     L2::writeLine((data[index].tag << 15 | index << 6), line);
        // }

        // array<uint32_t, 16> line = L2::readLine(addr);
        data[index].valid = true;
        // data[index].values = line;
        data[index].tag = tag;
    }
}
void L1::writeLine(uint32_t addr, array<uint32_t, 16> line, bool isData) // assume addr is 16B aligned and entry is in cache
{
    uint32_t index = addr >> 6 & 0b111111111; // get the middle 9 bits
    uint32_t tag = addr >> 15;                // get the first 17 bits
    if (data[index].tag == tag)
    {
        // tag already in cache

        data[index].valid = true;
        data[index].values = line;
        data[index].tag = tag;
    }
    else
    {
        cerr << "bad thing in l1 writeline" << endl;
        // // kick what we currently have in cache to L2, write to L1

        // // L2::writeLine(addr, line);
        // data[index].valid = true;
        // data[index].values = line;
        // data[index].tag = tag;
    }
}
uint32_t L1::readItem(uint32_t addr, bool isData)
{
    uint32_t newaddr = (addr >> 6) << 6;
    return readLine(newaddr, isData)[addr & 0b111111];
}
void L1::writeItem(uint32_t addr, uint32_t val, bool isData)
{
    uint32_t newaddr = (addr >> 6) << 6;
    array<uint32_t, 16> line = readLine(newaddr, isData);
    line[addr & 0b111111] = val;
    writeLine(newaddr, line, isData);
}
