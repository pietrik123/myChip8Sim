#include "Chip.h"

#include <iostream>
#include <iomanip>

// TODO

namespace MyChip8
{

void Chip::logStack()
{
    
}

void Chip::logRegisters()
{
    int cnt = 0;
    for (const auto r : registers)
    {
        std::cout << "v" << std::hex << cnt << " : " << std::hex << static_cast<int>(r) << "; ";
        cnt++;
    }
    std::cout << "PC : " << programCounter << "\n";
    std::cout << "I : " << registerIdx << "\n";
}

void Chip::logMemory(int offset, int numOfBytes)
{
    std::cout << "\n--------\nMemory\n--------\n";
    int i = 0;
    int  maxAddrToDisplay = offset + numOfBytes;
    const int maxAddr = memSize-1;
    const int bytesInRow = 8;
    
    if (maxAddrToDisplay > maxAddr)
    {
        maxAddrToDisplay = maxAddr;
    }
    
    int bytesCnt = 0;
    while (offset + i <= maxAddrToDisplay)
    {   
        std::cout << std::setw(4) << std::hex << static_cast<int>(memory[offset + i]);
        bytesCnt++;
        if (bytesCnt % 8 == 0)
        {
            std::cout << "\n";
        }
        i++;
    }
    
    std::cout << "\n";
}

}