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
            f_FX0A_getKey(data.x);
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