#include "Chip.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>

namespace MyChip8
{

std::string getRegisterName(uint8_t idx)
{
    return std::string {"v"} + std::to_string(static_cast<int>(idx));
}

Chip::Chip()
{
    registers["V0"] = 0;
    registers["V1"] = 0;
    registers["V2"] = 0;
    registers["V3"] = 0;
    registers["V4"] = 0;
    registers["V5"] = 0;
    registers["V6"] = 0;
    registers["V7"] = 0;
    registers["V8"] = 0;
    registers["V9"] = 0;
    registers["VA"] = 0;
    registers["VB"] = 0;
    registers["VC"] = 0;
    registers["VD"] = 0;
    registers["VE"] = 0;
    registers["VF"] = 0;
    
    std::memcpy(memory+0x050, fontsData, 80);
    programCounter = programStart;
}

uint16_t Chip::fetch()
{
    uint8_t firstByte = memory[programCounter];
    uint8_t secondByte = memory[programCounter+1];
    
    programCounter += 2;
    
    return ((static_cast<uint16_t>(firstByte) << 8) | static_cast<uint16_t>(secondByte));
}

InstructionData Chip::decode(uint16_t rawInstruction)
{
    InstructionData data;
    
    // decoding instruction, byte order is BIG-ENDIAN
    data.opcode = static_cast<InstructionType>((rawInstruction & 0xf000) >> 12);
    data.x = static_cast<uint8_t>((rawInstruction & 0xf00) >> 8);
    data.y = static_cast<uint8_t>((rawInstruction & 0xf0) >> 4);
    data.n = static_cast<uint8_t>(rawInstruction & 0xf);
    data.nn = static_cast<uint8_t>(rawInstruction & 0x00ff);
    data.nnn = rawInstruction & 0x0fff;
    
    std::cout << "Raw instr: " << std::hex << static_cast<int>(rawInstruction) << "\n";
    std::cout << "Opcode: " << static_cast<int>(data.opcode) << " \n"
              << "X: " << static_cast<int>(data.x) << " \n"
              << "Y: " << static_cast<int>(data.y) << " \n"
              << "N: " << static_cast<int>(data.n) << " \n"
              << "NN: " << static_cast<int>(data.nn) << " \n"
              << "NNN: " << static_cast<int>(data.nnn) << " \n\n";
    
    return data;
}

void Chip::execute(MyGfx* gfx, const InstructionData& data)
{
    
    switch (data.opcode)
    {
        case CLEAR_SCREEN:
            clearScreen(gfx);
            break;
        case JUMP:
            jump(data.nnn);
            break;
        case SET_VX:
            setRegisterVx(data.x, data.nn);
            break;
        case ADD_TO_VX:
            addValToVx(data.x, data.nn);
            break;
        case SET_IDX:
            setIdxRegister(data.nnn);
            break;
        case DISPLAY:
            display(gfx, data.x, data.y, data.n);
            break;
        default:
            std::cout << "Unknown instruction: " << static_cast<int>(data.opcode) << "\n-----\n";
    }
}

void Chip::clearScreen(MyGfx* gfx)
{
    gfx->clearDisplay();
}

void Chip::jump(uint16_t nnn)
{
    programCounter = nnn;
}

void Chip::setRegisterVx(uint8_t x, uint8_t nn)
{
    registers[getRegisterName(x)] = nn;
}

void Chip::addValToVx(uint8_t x, uint8_t nn)
{
    registers[getRegisterName(x)] += nn;
}

void Chip::setIdxRegister(uint16_t nnn)
{
    registerIdx = nnn;
}

void Chip::display(MyGfx* gfx, uint8_t x, uint8_t y, uint8_t n)
{
    uint8_t posX = registers[getRegisterName(x)] % screenWidth;
    uint8_t posY = registers[getRegisterName(y)] % screenHeight;
    
    std::cout << "[x,y] " << static_cast<int>(posX) << " " << static_cast<int>(posY) << "\n";
    
    registers["vf"] = 0;
    
    for (auto i = 0; i < n; i++)
    {
        if (posY + i >= screenHeight)
        {
            std::cout << "Exceed max height\n";
            break;
        }
        
        // get first row of 8 pixels - each bit is one pixel
        auto pixelsInRowData = memory[registerIdx + i];
        for (auto j = 0; j < 8; j++)
        {
            if (posX + j >= screenWidth)
            {
                std::cout << "Exceed max width\n";
                break;
            }
            
            auto currentPixelPosX = posX+j;
            auto currentPixelPosY = posY+i;
        
            // check if pixel on the sprite is filled
            if ((pixelsInRowData & ((1<<7)>>j)) > 0 )
            {
                if ( gfx->isPixelFilled(currentPixelPosX, currentPixelPosY) )
                {
                    gfx->clearPixel(currentPixelPosX, currentPixelPosY);
                    // set vf register
                    registers["vf"] = 1;
                }
                else
                {
                    gfx->fillPixel(currentPixelPosX, currentPixelPosY);
                }
            }
        }
    }
    gfx->updateDisplay();
}

void Chip::logRegisters()
{
    std::cout << "\n--------\nRegisters\n--------\n";
    for (const auto& r : registers)
    {
        std::cout << r.first << " : " << std::hex << static_cast<int>(r.second) << " , ";
    }
    std::cout << "\n\n";
    
    std::cout << "PC : " << std::hex << static_cast<int>(programCounter) << "\n";
    std::cout << "Index register : " << std::hex << static_cast<int>(registerIdx) << "\n";

}

// TODO
void Chip::logStack()
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

void Chip::loadProgram(const std::string& fileName)
{
    std::ifstream ifs(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
    
    const int maxSize = memSize - programStart;
   
    char data[maxSize];
    ifs.read(data, maxSize);
    
    for (auto i = programStart; i < programStart+ifs.gcount(); i++)
    {
        memory[i] = static_cast<uint8_t>(data[i-programStart]);
    }
    std::cout <<"\n";
    
    ifs.close();
}

    
}