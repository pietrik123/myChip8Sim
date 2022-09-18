#pragma once

#include "MyGfx.h"

#include <stdint.h>
#include <stack>
#include <map>

namespace MyChip8
{

enum InstructionType : uint8_t
{
    CLEAR_SCREEN = 0x0,
    JUMP = 0x1,
    SET_VX = 0x6,
    ADD_TO_VX = 0x7,
    SET_IDX = 0xa,
    DISPLAY = 0xd
};

struct InstructionData
{
    InstructionType opcode;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
};

const uint8_t fontsData[] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip
{
    
public:

    static const uint16_t programStart = 0x200u;
    static const uint8_t screenWidth = 64u;
    static const uint8_t screenHeight = 32u;
    static const int memSize = 4096;

    // memory and registers
    uint8_t memory[memSize];
    uint16_t programCounter;
    uint16_t registerIdx;
    std::map<std::string, uint8_t> registers;
    
    // TODO delay timer
    // TODO sound timer
    
    std::stack<uint16_t> stack; 
    
    
    Chip();
    
    // operation funtion : fetch instruction, decode it and execute it
    uint16_t fetch();   
    InstructionData decode(uint16_t rawInstruction);
    void execute(MyGfx* gfx, const InstructionData& data);
    
    // instructions
    void clearScreen(MyGfx* gfx);
    void jump(uint16_t nnn);
    void setRegisterVx(uint8_t x, uint8_t nn);
    void addValToVx(uint8_t x, uint8_t nn);
    void setIdxRegister(uint16_t nnn);
    void display(MyGfx* gfx, uint8_t x, uint8_t y, uint8_t n);
    
    // logging
    void logRegisters();
    void logStack();
    void logMemory(int offset, int numOfBytes);
    
    // load program
    bool loadProgram(const std::string& fileName);
};

std::string getRegisterName(uint8_t idx);

}