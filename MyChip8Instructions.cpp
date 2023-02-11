#include "Chip.h"

#include <iostream>

namespace MyChip8
{

void Chip::f_1NNN_jump(uint16_t nnn)
{
    programCounter = nnn;
}

void Chip::f_BNNN_jumpWithOffset(uint16_t nnn)
{
    programCounter = nnn + registers[0u];
}

void Chip::f_FX1E_addToIndex(uint8_t x)
{
    registerIdx += registers[x];
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
    registers[x] = nn;
}

void Chip::f_7XNN_addValToVx(uint8_t x, uint8_t nn)
{
    registers[x] += nn;
}

void Chip::f_ANNN_setIdxRegister(uint16_t nnn)
{
    registerIdx = nnn;
}

void Chip::f_3XNN_skipVxEqualNN(uint8_t x, uint8_t nn)
{
    if (registers[x] == nn)
    {
        programCounter += 2;
    }
}

void Chip::f_4XNN_skipVxNotEqualNN(uint8_t x, uint8_t nn)
{
    if (registers[x] != nn)
    {
        programCounter += 2;
    }
}

void Chip::f_5XY0_skipVxEqualVy(uint8_t x, uint8_t y)
{
    if (registers[x] == registers[y])
    {
        programCounter += 2;
    }
}

void Chip::f_9XY0_skipVxNotEqualVy(uint8_t x, uint8_t y)
{
    if (registers[x] != registers[y])
    {
        programCounter += 2;
    }
}

void Chip::f_8XY0_setVxVy(uint8_t x, uint8_t y)
{
    registers[x] = registers[y];
}

void Chip::f_8XY1_binaryOr(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    vx = vx | vy;
}

void Chip::f_8XY2_binaryAnd(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    vx = vx & vy;
}

void Chip::f_8XY3_logicalXor(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    vx = vx ^ vy;
}

void Chip::f_8XY4_addVxAndVy(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    uint16_t tmpSum = vx + vy;
    
    registers[0xf] = (tmpSum > 0xff ? 1 : 0);
    vx  = tmpSum > 0xff ? (tmpSum & 0xff) : tmpSum;
}

void Chip::f_8XY5_substractVxAndVy(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    registers[0xf] = (vx > vy ? 1 : 0);
    vx = static_cast<uint8_t>(vx - vy);
}

void Chip::f_8XY7_substractVyAndVx(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    registers[0xf] = (vy > vx ? 1 : 0);
    vx = static_cast<uint8_t>(vy - vx);
}

void Chip::f_8XY6_shiftRight(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    //vx = vy;
    registers[0xf] = ((vx & 0x1) > 0u ? 1 : 0);
    vx = (vx >> 1);
}

void Chip::f_8XYE_shiftLeft(uint8_t x, uint8_t y)
{
    auto& vx = registers[x];
    const auto& vy = registers[y];

    //vx = vy;
    registers[0xf] = (vx & 0x80 > 0u ? 1 : 0);
    vx = (vx << 1);
}

void Chip::f_CXNN_random(uint8_t x, uint8_t nn)
{
    registers[x] = std::rand() % 255;
}

void Chip::f_FX29_getCharacter(uint8_t x)
{
    registerIdx = fontOffset + (registers[x] & 0x0f) * 0x05;
}

void Chip::f_FX33_binDecConvert(uint8_t x)
{
    uint8_t val = registers[x];
    memory[registerIdx] = val / 100u;
    memory[registerIdx+2] = val % 10u;
    memory[registerIdx+1] = (val - memory[registerIdx]*100)/10u; 
}

void Chip::f_FX55_storeInMem(uint8_t x)
{
    for (auto i = 0u; i <= x; i++)
    {
        memory[registerIdx+i] = registers[i];        
    }
}

void Chip::f_FX65_loadFromMem(uint8_t x)
{
    for (auto i = 0u; i <= x; i++)
    {
        registers[i] = memory[registerIdx+i];
    }
}

void Chip::f_FX07_setVxDelayTimer(uint8_t x)
{
    registers[x] = delayTimer;
}

void Chip::f_FX15_setDelayTimerVx(uint8_t x)
{
    delayTimer = registers[x];
}

void Chip::f_FX18_setSoundTimerVx(uint8_t x)
{
    soundTimer = registers[x];
}

}