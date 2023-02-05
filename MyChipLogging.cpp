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