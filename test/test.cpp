#include "Chip.h"

#include "catch_amalgamated.hpp"
#include <optional>

// stubs

bool isKeyPressed(uint8_t key)
{
    return false;
}

std::optional<uint8_t> getPressedKey()
{
    return std::nullopt;
}

// -----------------

TEST_CASE("Test_opcode", "test opcode")
{
    MyChip8::Chip myChip {};

    REQUIRE( MyChip8::InstructionType::CLEAR_SCREEN ==              myChip.decode(0x00e0).instructionType);

    REQUIRE( MyChip8::InstructionType::JUMP ==                      myChip.decode(0x1abc).instructionType);
    REQUIRE( MyChip8::InstructionType::CALL_SUBROUTINE ==           myChip.decode(0x2abc).instructionType);
    REQUIRE( MyChip8::InstructionType::RETURN_FROM_SUBROUTINE ==    myChip.decode(0x00ee).instructionType);

    REQUIRE( MyChip8::InstructionType::SKIP_VX_EQUAL_NN ==          myChip.decode(0x3abc).instructionType);
    REQUIRE( MyChip8::InstructionType::SKIP_VX_NOT_EQUAL_NN ==      myChip.decode(0x4abc).instructionType);
    REQUIRE( MyChip8::InstructionType::SKIP_VX_EQUAL_VY ==          myChip.decode(0x5abc).instructionType);
    REQUIRE( MyChip8::InstructionType::SKIP_VX_NOT_EQUAL_VY ==      myChip.decode(0x9abc).instructionType);

    REQUIRE( MyChip8::InstructionType::SET_VX ==                    myChip.decode(0x6abc).instructionType);
    REQUIRE( MyChip8::InstructionType::ADD_TO_VX ==                 myChip.decode(0x7abc).instructionType);

    REQUIRE( MyChip8::InstructionType::SET_VX_VY ==                 myChip.decode(0x8ab0).instructionType);
    REQUIRE( MyChip8::InstructionType::BINARY_OR ==                 myChip.decode(0x8ab1).instructionType);
    REQUIRE( MyChip8::InstructionType::BINARY_AND ==                myChip.decode(0x8ab2).instructionType);
    REQUIRE( MyChip8::InstructionType::LOGICAL_XOR ==               myChip.decode(0x8ab3).instructionType);
    REQUIRE( MyChip8::InstructionType::ADD_VX_VY ==                 myChip.decode(0x8ab4).instructionType);
    REQUIRE( MyChip8::InstructionType::SUBTRACT_VX_VY ==            myChip.decode(0x8ab5).instructionType);
    REQUIRE( MyChip8::InstructionType::SUBTRACT_VY_VX ==            myChip.decode(0x8ab7).instructionType);
    REQUIRE( MyChip8::InstructionType::SHIFT_RIGHT ==               myChip.decode(0x8ab6).instructionType);
    REQUIRE( MyChip8::InstructionType::SHIFT_LEFT ==                myChip.decode(0x8abe).instructionType);

    REQUIRE( MyChip8::InstructionType::SET_IDX ==                   myChip.decode(0xaabc).instructionType);
    REQUIRE( MyChip8::InstructionType::JUMP_WITH_OFFSET ==          myChip.decode(0xbabc).instructionType);

    REQUIRE( MyChip8::InstructionType::RANDOM ==                    myChip.decode(0xcabc).instructionType);

    REQUIRE( MyChip8::InstructionType::DISPLAY ==                   myChip.decode(0xdabc).instructionType);

    REQUIRE( MyChip8::InstructionType::SKIP_IF_KEY_PRESSED ==       myChip.decode(0xea9e).instructionType);
    REQUIRE( MyChip8::InstructionType::SKIP_IF_KEY_NOT_PRESSED ==   myChip.decode(0xeaa1).instructionType);

    REQUIRE( MyChip8::InstructionType::SET_VX_TIMER ==              myChip.decode(0xfa07).instructionType);
    REQUIRE( MyChip8::InstructionType::SET_DELAY_TIMER_VX ==        myChip.decode(0xfa15).instructionType);
    REQUIRE( MyChip8::InstructionType::SET_SOUND_TIMER_VX ==        myChip.decode(0xfa18).instructionType);

    REQUIRE( MyChip8::InstructionType::ADD_TO_IDX ==                myChip.decode(0xfa1e).instructionType);
    REQUIRE( MyChip8::InstructionType::GET_KEY ==                   myChip.decode(0xfa0a).instructionType);
    REQUIRE( MyChip8::InstructionType::FONT_CHAR ==                 myChip.decode(0xfa29).instructionType);
    REQUIRE( MyChip8::InstructionType::BIN_CODED_DEC_CONV ==        myChip.decode(0xfa33).instructionType);
    REQUIRE( MyChip8::InstructionType::STORE_MEM ==                 myChip.decode(0xfa55).instructionType);
    REQUIRE( MyChip8::InstructionType::LOAD_MEM ==                  myChip.decode(0xfa65).instructionType);
}

TEST_CASE("Test_instructions", "test instructions")
{
    MyChip8::Chip myChip {};

    REQUIRE( myChip.registers["v0"] == 0 );
}