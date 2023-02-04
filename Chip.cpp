#include "Chip.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>

namespace MyChip8
{

std::string getInstructionTypeDescription(InstructionType instructionType)
{
    if (instrTypeVsDescriptionMap.find(instructionType) != instrTypeVsDescriptionMap.end())
    {
        return instrTypeVsDescriptionMap.at(instructionType);
    }
    return std::string {"UNKNOW_DESCRIPTION"};
}

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
    
    std::memcpy(memory+fontOffset, fontsData, 80);
    programCounter = programStart;

    std::srand(std::time(nullptr));
}

uint16_t Chip::fetch()
{
    uint8_t firstByte = memory[programCounter];
    uint8_t secondByte = memory[programCounter+1];
    
    programCounter += 2;
    
    return ((static_cast<uint16_t>(firstByte) << 8) | static_cast<uint16_t>(secondByte));
}

static InstructionType getInstructionType(uint8_t* nibblesArray)
{
    
    if (nibblesArray[0] == 0x0 && nibblesArray[1] == 0x0 && nibblesArray[2] == 0xe && nibblesArray[3] == 0x0)
    {
        return InstructionType::CLEAR_SCREEN;
    }
    else if (nibblesArray[0] == 0x0 && nibblesArray[1] == 0x0 && nibblesArray[2] == 0xe && nibblesArray[3] == 0xe)
    {
        return InstructionType::RETURN_FROM_SUBROUTINE;
    }
    else if (nibblesArray[0] == 0x1)
    {
        return InstructionType::JUMP;
    }
    else if (nibblesArray[0] == 0x2)
    {
        return InstructionType::CALL_SUBROUTINE;
    }
    else if (nibblesArray[0] == 0x3)
    {
        return InstructionType::SKIP_VX_EQUAL_NN;
    }
    else if (nibblesArray[0] == 0x4)
    {
        return InstructionType::SKIP_VX_NOT_EQUAL_NN;
    }
    else if (nibblesArray[0] == 0x5)
    {
        return InstructionType::SKIP_VX_EQUAL_VY;
    }
    else if (nibblesArray[0] == 0x9)
    {
        return InstructionType::SKIP_VX_NOT_EQUAL_VY;
    }
    else if (nibblesArray[0] == 0x6)
    {
        return InstructionType::SET_VX;
    }
    else if (nibblesArray[0] == 0x7)
    {
        return InstructionType::ADD_TO_VX;
    }
    // LOGICAL AND ARITHMETIC
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x0)
    {
        return InstructionType::SET_VX_VY;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x1)
    {
        return InstructionType::BINARY_OR;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x2)
    {
        return InstructionType::BINARY_AND;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x3)
    {
        return InstructionType::LOGICAL_XOR;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x4)
    {
        return InstructionType::ADD_VX_VY;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x5)
    {
        return InstructionType::SUBTRACT_VX_VY;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x6)
    {
        return InstructionType::SHIFT_RIGHT;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x7)
    {
        return InstructionType::SUBTRACT_VY_VX;
    }
    else if (nibblesArray[0] == 0x8 && nibblesArray[3] == 0x8)
    {
        return InstructionType::SHIFT_LEFT;
    }
    else if (nibblesArray[0] == 0xa)
    {
        return InstructionType::SET_IDX;
    }
    else if (nibblesArray[0] == 0xc)
    {
        return InstructionType::RANDOM;
    }
    else if (nibblesArray[0] == 0xd)
    {
        return InstructionType::DISPLAY;
    }
    return InstructionType::UNKNOWN;
}

InstructionData Chip::decode(uint16_t rawInstruction)
{
    InstructionData data;
    
    // decoding instruction, byte order is BIG-ENDIAN

    uint8_t nibbles [] = 
    {
        static_cast<uint8_t>((rawInstruction & 0xf000) >> 12),
        static_cast<uint8_t>((rawInstruction & 0xf00) >> 8),
        static_cast<uint8_t>((rawInstruction & 0xf0) >> 4),
        static_cast<uint8_t>(rawInstruction & 0xf)
    };

    data.instructionType = getInstructionType(nibbles);
    
    data.x = nibbles[1];
    data.y = nibbles[2];
    data.n = nibbles[3];

    data.nn = static_cast<uint8_t>(rawInstruction & 0x00ff);
    data.nnn = rawInstruction & 0x0fff;
    
    std::cout << "Raw instr: " << std::hex << static_cast<int>(rawInstruction) << "\n";

    std::cout << "Instruction type: " << getInstructionTypeDescription(data.instructionType) << " \n"
              << "X: " << static_cast<int>(data.x) << " \n"
              << "Y: " << static_cast<int>(data.y) << " \n"
              << "N: " << static_cast<int>(data.n) << " \n"
              << "NN: " << static_cast<int>(data.nn) << " \n"
              << "NNN: " << static_cast<int>(data.nnn) << " \n\n";
    
    return data;
}

void Chip::execute(MyGfx* gfx, const InstructionData& data)
{
    
    switch (data.instructionType)
    {
        case InstructionType::CLEAR_SCREEN:
            f_00E0_clearScreen(gfx);
            break;
        case InstructionType::JUMP:
            f_1NNN_jump(data.nnn);
            break;
        case InstructionType::SET_VX:
            f_6XNN_setRegisterVx(data.x, data.nn);
            break;
        case InstructionType::ADD_TO_VX:
            f_7XNN_addValToVx(data.x, data.nn);
            break;
        case InstructionType::SET_IDX:
            f_ANNN_setIdxRegister(data.nnn);
            break;
        case InstructionType::DISPLAY:
            f_DXYN_display(gfx, data.x, data.y, data.n);
            break;
        case InstructionType::CALL_SUBROUTINE:
            f_2NNN_callSubroutine(data.nnn);
            break;
        case InstructionType::RETURN_FROM_SUBROUTINE:
            f_00EE_returnFromSubroutine();
            break;
        case InstructionType::SKIP_VX_EQUAL_NN:
            f_3XNN_skipVxEqualNN(data.x, data.nn);
            break;
        case InstructionType::SKIP_VX_NOT_EQUAL_NN:
            f_4XNN_skipVxNotEqualNN(data.x, data.nn);
            break;
        case InstructionType::SKIP_VX_EQUAL_VY:
            f_5XY0_skipVxEqualVy(data.x, data.y);
            break;
        case InstructionType::SKIP_VX_NOT_EQUAL_VY:
            f_9XY0_skipVxNotEqualVy(data.x, data.y);
            break;
        case InstructionType::SET_VX_VY:
            f_8XY0_setVxVy(data.x, data.y);
            break;
        case InstructionType::BINARY_OR:
            f_8XY1_binaryOr(data.x, data.y);
            break;
        case InstructionType::BINARY_AND:
            f_8XY2_binaryAnd(data.x, data.y);
            break;
        case InstructionType::LOGICAL_XOR:
            f_8XY3_logicalXor(data.x, data.y);
            break;
        case InstructionType::ADD_VX_VY:
            f_8XY4_addVxAndVy(data.x, data.y);
            break;
        case InstructionType::SUBTRACT_VX_VY:
            f_8XY5_substractVxAndVy(data.x, data.y);
            break;
        case InstructionType::SUBTRACT_VY_VX:
            f_8XY7_substractVyAndVx(data.x, data.y);
            break;
        case InstructionType::SHIFT_RIGHT:
            f_8XY6_shiftRight(data.x, data.y);
            break;
        case InstructionType::SHIFT_LEFT:
            f_8XYE_shiftLeft(data.x, data.y);
            break;
        case InstructionType::RANDOM:
            f_CXNN_random(data.x, data.nn);
            break;
        case InstructionType::SKIP_IF_KEY_PRESSED:
            f_EX9E_skipIfKeyIsPressed(data.x);
            break;
        case InstructionType::SKIP_IF_KEY_NOT_PRESSED:
            f_EXA1_skipIfKeyIsNotPressed(data.x);
            break;
        case InstructionType::SET_VX_TIMER:
            f_FX07_setVxDelayTimer(data.x);
            break;
        case InstructionType::SET_DELAY_TIMER_VX:
            f_FX15_setDelayTimerVx(data.x);
            break;
        case InstructionType::SET_SOUND_TIMER_VX:
            f_FX18_setSoundTimerVx(data.x);
            break;
        case InstructionType::ADD_TO_IDX:
            f_FX1E_addToIndex(data.x);
            break;
        case InstructionType::GET_KEY:
            f_FX0A_getKey();
            break;
        case InstructionType::FONT_CHAR:
            f_FX29_getCharacter(data.x);
            break;
        case InstructionType::BIN_CODED_DEC_CONV:
            f_FX33_binDecConvert(data.x);
            break;
        case InstructionType::STORE_MEM:
            f_FX55_storeInMem(data.x);
            break;
        case InstructionType::LOAD_MEM:
            f_FX65_loadFromMem(data.x);
        default:
            std::cout << "Unknown instruction: " << static_cast<int>(data.instructionType) << "\n-----\n";
    }
}

void Chip::f_00E0_clearScreen(MyGfx* gfx)
{
    gfx->clearDisplay();
}

void Chip::f_1NNN_jump(uint16_t nnn)
{
    programCounter = nnn;
}

void Chip::f_BNNN_jumpWithOffset(uint16_t nnn)
{
    programCounter = nnn + registers["v0"];
}

void Chip::f_FX1E_addToIndex(uint8_t x)
{
    registerIdx += registers[getRegisterName(x)];
}

void Chip::f_2NNN_callSubroutine(uint16_t nnn)
{
    stack.push(programCounter);
    programCounter = nnn;
}

void Chip::f_00EE_returnFromSubroutine()
{
    programCounter = stack.top();
    stack.pop();
}

void Chip::f_6XNN_setRegisterVx(uint8_t x, uint8_t nn)
{
    registers[getRegisterName(x)] = nn;
}

void Chip::f_7XNN_addValToVx(uint8_t x, uint8_t nn)
{
    registers[getRegisterName(x)] += nn;
}

void Chip::f_ANNN_setIdxRegister(uint16_t nnn)
{
    registerIdx = nnn;
}

void Chip::f_3XNN_skipVxEqualNN(uint8_t x, uint8_t nn)
{
    if (registers[getRegisterName(x)] == nn)
    {
        programCounter += 2;
    }
}

void Chip::f_4XNN_skipVxNotEqualNN(uint8_t x, uint8_t nn)
{
    if (registers[getRegisterName(x)] != nn)
    {
        programCounter += 2;
    }
}

void Chip::f_5XY0_skipVxEqualVy(uint8_t x, uint8_t y)
{
    if (registers[getRegisterName(x)] == registers[getRegisterName(y)])
    {
        programCounter += 2;
    }
}

void Chip::f_9XY0_skipVxNotEqualVy(uint8_t x, uint8_t y)
{
    if (registers[getRegisterName(x)] != registers[getRegisterName(y)])
    {
        programCounter += 2;
    }
}

void Chip::f_8XY0_setVxVy(uint8_t x, uint8_t y)
{
    registers[getRegisterName(x)] = registers[getRegisterName(y)];
}

void Chip::f_8XY1_binaryOr(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = vx | vy;
}

void Chip::f_8XY2_binaryAnd(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = vx | vy;
}

void Chip::f_8XY3_logicalXor(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = vx ^ vy;
}

void Chip::f_8XY4_addVxAndVy(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    uint16_t tmpSum = vx + vy;
    vx  = static_cast<uint8_t>(tmpSum && 0xff);
    registers["vf"] = (tmpSum > 255 ? 1 : 0);
}

void Chip::f_8XY5_substractVxAndVy(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = static_cast<uint8_t>(vx - vy);
    registers["vf"] = (vx >= vy ? 1 : 0);
}

void Chip::f_8XY7_substractVyAndVx(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = static_cast<uint8_t>(vy - vx);
    registers["vf"] = (vy >= vx ? 1 : 0);
}

void Chip::f_8XY6_shiftRight(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = vy;
    registers["vf"] = (vx & 0x1 > 0u ? 1 : 0);
    vx = (vx >> 1);
}

void Chip::f_8XYE_shiftLeft(uint8_t x, uint8_t y)
{
    auto& vx = registers[getRegisterName(x)];
    const auto& vy = registers[getRegisterName(y)];

    vx = vy;
    registers["vf"] = (vx & 0x80 > 0u ? 1 : 0);
    vx = (vx << 1);
}

void Chip::f_DXYN_display(MyGfx* gfx, uint8_t x, uint8_t y, uint8_t n)
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

void Chip::f_CXNN_random(uint8_t x, uint8_t nn)
{
    registers[getRegisterName(x)] = std::rand() % 255;
}

void Chip::skipDependingOnKeyState(uint8_t x, bool keyStatePressed)
{
    const uint8_t key = registers[getRegisterName(x)];
    if (!(isKeyPressed(key) ^ keyStatePressed))
    {
        programCounter += 2u;
    }
}

void Chip::f_EX9E_skipIfKeyIsPressed(uint8_t x)
{
    skipDependingOnKeyState(x, true);
}

void Chip::f_EXA1_skipIfKeyIsNotPressed(uint8_t x)
{
    skipDependingOnKeyState(x, false);
}

void Chip::f_FX0A_getKey()
{
    if (!isAnyKeyPressed())
    {
        programCounter -= 2u;  
    }
}

void Chip::f_FX29_getCharacter(uint8_t x)
{
    registerIdx = fontOffset + (registers[getRegisterName(x)] & 0x0f) * 0x05;
}

void Chip::f_FX33_binDecConvert(uint8_t x)
{
    uint8_t val = registers[getRegisterName(x)];
    memory[registerIdx] = val / 100u;
    memory[registerIdx+2] = val % 10u;
    memory[registerIdx+1] = (val - memory[registerIdx])/10u; 
}

void Chip::f_FX55_storeInMem(uint8_t x)
{
    for (auto i = 0u; i < x; i++)
    {
        memory[registerIdx+i] = registers[getRegisterName(i)];        
    }
}

void Chip::f_FX65_loadFromMem(uint8_t x)
{
    for (auto i = 0u; i < x; i++)
    {
        registers[getRegisterName(i)] = memory[registerIdx+i];
    }
}

void Chip::f_FX07_setVxDelayTimer(uint8_t x)
{
    registers[getRegisterName(x)] = delayTimer;
}

void Chip::f_FX15_setDelayTimerVx(uint8_t x)
{
    delayTimer = registers[getRegisterName(x)];
}

void Chip::f_FX18_setSoundTimerVx(uint8_t x)
{
    soundTimer = registers[getRegisterName(x)];
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

bool Chip::loadProgram(const std::string& fileName)
{
    std::ifstream ifs(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
    
    if (ifs.fail())
    {
        std::cout << "Cannot open file: " << fileName << "\n";
        return false;
    }
    
    const int maxSize = memSize - programStart;
   
    char data[maxSize];
    ifs.read(data, maxSize);
    
    for (auto i = programStart; i < programStart+ifs.gcount(); i++)
    {
        memory[i] = static_cast<uint8_t>(data[i-programStart]);
    }
    std::cout <<"\n";
    
    ifs.close();
    return true;
}

    
}