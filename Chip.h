#pragma once

#include "MyGfx.h"
#include "MyKeyboardInput.h"

#include <stdint.h>
#include <stack>
#include <array>
#include <map>
#include <string>

namespace MyChip8
{

enum class InstructionType 
{
    UNKNOWN,
    CLEAR_SCREEN,
    JUMP,
    JUMP_WITH_OFFSET,
    SET_VX,
    ADD_TO_VX,
    SET_IDX,
    DISPLAY,
    CALL_SUBROUTINE,
    RETURN_FROM_SUBROUTINE,
    SKIP_VX_EQUAL_NN,
    SKIP_VX_NOT_EQUAL_NN,
    SKIP_VX_EQUAL_VY,
    SKIP_VX_NOT_EQUAL_VY,
    SET_VX_VY,
    BINARY_OR,
    BINARY_AND,
    LOGICAL_XOR,
    ADD_VX_VY,
    SUBTRACT_VX_VY,
    SUBTRACT_VY_VX,
    SHIFT_RIGHT,
    SHIFT_LEFT,
    RANDOM,
    SKIP_IF_KEY_PRESSED,
    SKIP_IF_KEY_NOT_PRESSED,
    SET_VX_TIMER,
    SET_DELAY_TIMER_VX,
    SET_SOUND_TIMER_VX,
    ADD_TO_IDX,
    GET_KEY,
    FONT_CHAR,
    BIN_CODED_DEC_CONV,
    STORE_MEM,
    LOAD_MEM
};

const std::map<InstructionType, std::string> instrTypeVsDescriptionMap
{
    {InstructionType::UNKNOWN, "UNKNOWN"},
    {InstructionType::CLEAR_SCREEN, "CLEAR_SCREEN"},
    {InstructionType::JUMP, "JUMP"},
    {InstructionType::JUMP_WITH_OFFSET, "JUMP_WITH_OFFSET"},
    {InstructionType::SET_VX, "SET_VX"},
    {InstructionType::ADD_TO_VX, "ADD_TO_VX"},
    {InstructionType::SET_IDX, "SET_IDX"},
    {InstructionType::DISPLAY, "DISPLAY"},
    {InstructionType::CALL_SUBROUTINE, "CALL_SUBROUTINE"},
    {InstructionType::RETURN_FROM_SUBROUTINE, "RETURN_FROM_SUBROUTINE"},
    {InstructionType::SKIP_VX_EQUAL_NN, "SKIP_VX_EQUAL_NN"},
    {InstructionType::SKIP_VX_NOT_EQUAL_NN, "SKIP_VX_NOT_EQUAL_NN"},
    {InstructionType::SKIP_VX_EQUAL_VY, "SKIP_VX_EQUAL_VY"},
    {InstructionType::SKIP_VX_NOT_EQUAL_VY, "SKIP_VX_NOT_EQUAL_VY"},
    {InstructionType::SET_VX_VY, "SET_VX_VY"},
    {InstructionType::BINARY_OR, "BINARY_OR"},
    {InstructionType::BINARY_AND, "BINARY_AND"},
    {InstructionType::LOGICAL_XOR, "LOGICAL_XOR"},
    {InstructionType::ADD_VX_VY, "ADD_VX_VY"},
    {InstructionType::SUBTRACT_VX_VY, "SUBTRACT_VX_VY"},
    {InstructionType::SUBTRACT_VY_VX, "SUBTRACT_VY_VX"},
    {InstructionType::SHIFT_RIGHT, "SHIFT_RIGHT"},
    {InstructionType::SHIFT_LEFT, "SHIFT_LEFT"},
    {InstructionType::RANDOM, "RANDOM"},
    {InstructionType::SKIP_IF_KEY_PRESSED, "SKIP_IF_KEY_PRESSED"},
    {InstructionType::SKIP_IF_KEY_NOT_PRESSED, "SKIP_IF_KEY_NOT_PRESSED"},
    {InstructionType::SET_VX_TIMER, "SET_VX_TIMER"},
    {InstructionType::SET_DELAY_TIMER_VX, "SET_DELAY_TIMER_VX"},
    {InstructionType::SET_SOUND_TIMER_VX, "SET_SOUND_TIMER_VX"},
    {InstructionType::ADD_TO_IDX, "ADD_TO_IDX"},
    {InstructionType::GET_KEY, "GET_KEY"},
    {InstructionType::FONT_CHAR, "FONT_CHAR"},
    {InstructionType::BIN_CODED_DEC_CONV, "BIN_CODED_DEC_CONV"},
    {InstructionType::STORE_MEM, "STORE_MEM"},
    {InstructionType::LOAD_MEM, "LOAD_MEM"}
};

std::string getInstructionTypeDescription(InstructionType instructionType);

struct InstructionData
{
    InstructionType instructionType;
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

#ifndef UNIT_TEST
private:
#else
public:
#endif

    static const uint16_t programStart = 0x200u;
    static const uint16_t fontOffset = 0x050u;
    static const uint8_t screenWidth = 64u;
    static const uint8_t screenHeight = 32u;
    static const uint8_t numOfRegisters = 16u;
    static const int memSize = 4096;

    // memory and registers
    uint8_t memory[memSize];
    uint16_t programCounter;
    uint16_t registerIdx;

    std::array<uint8_t, numOfRegisters> registers;

    uint8_t delayTimer;
    uint8_t soundTimer;

    std::stack<uint16_t> stack;

    void skipDependingOnKeyState(uint8_t,bool);
public:
    
    Chip();
    
    // operation funtion : fetch instruction, decode it and execute it
    uint16_t fetch();   
    InstructionData decode(uint16_t rawInstruction);
    void execute(GfxInterface* gfx, const InstructionData& data);

    void handleDelayTimer();
    void handleSoundTimer();
    
    // instructions
    void f_00E0_clearScreen(GfxInterface* gfx);
    void f_1NNN_jump(uint16_t nnn);
    void f_BNNN_jumpWithOffset(uint16_t nnn);
    void f_FX1E_addToIndex(uint8_t x);
    void f_6XNN_setRegisterVx(uint8_t x, uint8_t nn);
    void f_7XNN_addValToVx(uint8_t x, uint8_t nn);
    void f_ANNN_setIdxRegister(uint16_t nnn);
    
    void f_DXYN_display(GfxInterface* gfx, uint8_t x, uint8_t y, uint8_t n);
    
    void f_2NNN_callSubroutine(uint16_t nnn);
    void f_00EE_returnFromSubroutine();

    void f_3XNN_skipVxEqualNN(uint8_t x, uint8_t nn);
    void f_4XNN_skipVxNotEqualNN(uint8_t x, uint8_t nn);
    void f_5XY0_skipVxEqualVy(uint8_t x, uint8_t y);
    void f_9XY0_skipVxNotEqualVy(uint8_t x, uint8_t y);

    void f_8XY0_setVxVy(uint8_t x, uint8_t y);
    void f_8XY1_binaryOr(uint8_t x, uint8_t y);
    void f_8XY2_binaryAnd(uint8_t x, uint8_t y);
    void f_8XY3_logicalXor(uint8_t x, uint8_t y);
    void f_8XY4_addVxAndVy(uint8_t x, uint8_t y);
    void f_8XY5_substractVxAndVy(uint8_t x, uint8_t y);
    void f_8XY7_substractVyAndVx(uint8_t x, uint8_t y);
    void f_8XY6_shiftRight(uint8_t x, uint8_t y);
    void f_8XYE_shiftLeft(uint8_t x, uint8_t y);

    void f_CXNN_random(uint8_t x, uint8_t nn);
    
    void f_EX9E_skipIfKeyIsPressed(uint8_t x);
    void f_EXA1_skipIfKeyIsNotPressed(uint8_t x);
    void f_FX0A_getKey(uint8_t x);

    void f_FX29_getCharacter(uint8_t x);
    
    void f_FX33_binDecConvert(uint8_t x);

    void f_FX55_storeInMem(uint8_t x);
    void f_FX65_loadFromMem(uint8_t x);

    void f_FX07_setVxDelayTimer(uint8_t x);
    void f_FX15_setDelayTimerVx(uint8_t x);
    void f_FX18_setSoundTimerVx(uint8_t x);
        
    // logging
    void logRegisters();
    void logStack();
    void logMemory(int offset, int numOfBytes);
    
    // load program
    bool loadProgram(const std::string& fileName);

};

}