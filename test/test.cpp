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

TEST_CASE("Test_call_subroutine_and_return", "call subroutine and return")
{
    MyChip8::Chip myChip {};

    uint16_t prevCounter = 0x0200;
    uint16_t jumpTo = 0x0300;

    myChip.stack.push(0x1234);
    myChip.programCounter = prevCounter;

    myChip.f_2NNN_callSubroutine(jumpTo);
    REQUIRE( myChip.programCounter == jumpTo);

    myChip.f_00EE_returnFromSubroutine();
    REQUIRE( myChip.programCounter == prevCounter);
}

TEST_CASE("Test_binary_op_instructions", "test binary op instructions")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0xf0;
    myChip.registers[0x2] = 0x0f;

    myChip.registers[0x3] = 0x03;
    myChip.registers[0x4] = 0x01;

    SECTION("test OR")
    {
        myChip.f_8XY1_binaryOr(1u, 2u);
        REQUIRE( myChip.registers[0x1] == 0xff);
        REQUIRE( myChip.registers[0x2] == 0x0f);
    }

    SECTION("test AND")
    {
        myChip.f_8XY2_binaryAnd(1u, 2u);
        REQUIRE( myChip.registers[0x1] == 0x00);
        REQUIRE( myChip.registers[0x2] == 0x0f);
    }

    SECTION("test XOR")
    {
        myChip.f_8XY3_logicalXor(3u, 4u);
        REQUIRE( myChip.registers[0x3] == 0x02);
        REQUIRE( myChip.registers[0x4] == 0x01);
    }

}

TEST_CASE("Test_add_vx_and_nn","test add vx and nn")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x5] = 0x1;

    myChip.f_7XNN_addValToVx(0x5,0xfe);

    REQUIRE( myChip.registers[0x5] == 0xff); 
}

TEST_CASE("Test_add_vx_vy", "test add vx and vy")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0x05;
    myChip.registers[0x2] = 0x03;

    myChip.registers[0x3] = 0xfe;
    myChip.registers[0x4] = 0x02;

    SECTION("test ADD no overflow")
    {
        myChip.f_8XY4_addVxAndVy(1u, 2u);
        REQUIRE( myChip.registers[0x1] == 0x08);
        REQUIRE( myChip.registers[0x2] == 0x03);
        REQUIRE( myChip.registers[0xf] == 0x00);
    }

    SECTION("test ADD overflow")
    {
        myChip.f_8XY4_addVxAndVy(3u, 4u);
        REQUIRE( myChip.registers[0x3] == 0x00);
        REQUIRE( myChip.registers[0x4] == 0x02);
        REQUIRE( myChip.registers[0xf] == 0x01);
    }

}

TEST_CASE("Test_subtract", "test subtract")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0x05;
    myChip.registers[0x2] = 0x03;

    myChip.registers[0x3] = 0x01;
    myChip.registers[0x4] = 0x02;

    SECTION("test subtract x - y overflow")
    {
        myChip.f_8XY5_substractVxAndVy(1u, 2u);
        REQUIRE( myChip.registers[0x1] == 0x02);
        REQUIRE( myChip.registers[0x2] == 0x03);
        REQUIRE( myChip.registers[0xf] == 0x01);
    }

    SECTION("test subtract x - y no overflow")
    {
        myChip.f_8XY5_substractVxAndVy(3u, 4u);
        REQUIRE( myChip.registers[0x3] == 0xff);
        REQUIRE( myChip.registers[0x4] == 0x02);
        REQUIRE( myChip.registers[0xf] == 0x00);
    }

    SECTION("test subtract y - x overflow")
    {
        myChip.f_8XY7_substractVyAndVx(2u, 1u);
        REQUIRE( myChip.registers[0x2] == 0x02);
        REQUIRE( myChip.registers[0x1] == 0x05);
        REQUIRE( myChip.registers[0xf] == 0x01);
    }

    SECTION("test subtract y - x no overflow")
    {
        myChip.f_8XY7_substractVyAndVx(4u, 3u);
        REQUIRE( myChip.registers[0x4] == 0xff);
        REQUIRE( myChip.registers[0x3] == 0x01);
        REQUIRE( myChip.registers[0xf] == 0x00);
    }

}

TEST_CASE("Test_set_vx_vy", "test set vx to vy")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0x05;
    myChip.registers[0x2] = 0x03;

    myChip.f_8XY0_setVxVy(1u, 2u);
    REQUIRE( myChip.registers[0x1] == 0x03);
    REQUIRE( myChip.registers[0x2] == 0x03);
}

TEST_CASE("Test_set_idx", "test set index-register")
{
    MyChip8::Chip myChip {};

    myChip.registerIdx = 0x500;

    myChip.f_ANNN_setIdxRegister(0x123);
    REQUIRE( myChip.registerIdx == 0x123);
}

TEST_CASE("Test_jump_with_offset", "test jump with offset")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x0] = 0x01;
    myChip.programCounter = 0x100;

    myChip.f_BNNN_jumpWithOffset(0x200);
    REQUIRE( myChip.programCounter == 0x201);
}

TEST_CASE("Test_font_char", "test font character")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x2] = 0x01;
    myChip.registerIdx = 0x100;

    myChip.f_FX29_getCharacter(2u);
    REQUIRE( myChip.registerIdx == 0x050 + 0x05);
}

TEST_CASE("Test_skip", "test skip")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0x05;
    myChip.registers[0x2] = 0x05;
    myChip.registers[0x3] = 0x04;
    myChip.programCounter = 0x300;

    SECTION("Test skip if vx equals nn")
    {
        myChip.f_3XNN_skipVxEqualNN(0x01, 0x05);
        REQUIRE( myChip.programCounter == 0x302);

        myChip.f_3XNN_skipVxEqualNN(0x01, 0x06);
        REQUIRE( myChip.programCounter == 0x302);
    }

    SECTION("Test skip if vx does not equal nn")
    {
        myChip.f_4XNN_skipVxNotEqualNN(0x01, 0x11);
        REQUIRE( myChip.programCounter == 0x302);

        myChip.f_4XNN_skipVxNotEqualNN(0x01, 0x05);
        REQUIRE( myChip.programCounter == 0x302);
    }

    SECTION("Test skip if vx equals vy")
    {
        myChip.f_5XY0_skipVxEqualVy(0x01, 0x02);
        REQUIRE( myChip.programCounter == 0x302);

        myChip.f_5XY0_skipVxEqualVy(0x01, 0x03);
        REQUIRE( myChip.programCounter == 0x302);
    }

    SECTION("Test skip if vx does not equal vy")
    {
        myChip.f_9XY0_skipVxNotEqualVy(0x01, 0x03);
        REQUIRE( myChip.programCounter == 0x302);

        myChip.f_9XY0_skipVxNotEqualVy(0x01, 0x02);
        REQUIRE( myChip.programCounter == 0x302);
    }
}

TEST_CASE("Test_shift", "test shift")
{
    MyChip8::Chip myChip {};

    myChip.registers[0x1] = 0x00; 
    myChip.registers[0x2] = 0x00; 

    myChip.registers[0x3] = 0x03; // 0b0000'0011
    myChip.registers[0x4] = 0xc0; // 0b1100'0000

    myChip.f_8XY6_shiftRight(1u, 3u);
    REQUIRE( myChip.registers[0x1] == 0x01); // 0b0000'0001

    myChip.f_8XYE_shiftLeft(2u, 4u);
    REQUIRE( myChip.registers[0x2] == 0x80); // 0b1000'0000
}