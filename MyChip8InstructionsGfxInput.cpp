#include "Chip.h"

#include <iostream>

namespace MyChip8
{

void Chip::f_00E0_clearScreen(GfxInterface* gfx)
{
    gfx->clearDisplay();
}

void Chip::f_DXYN_display(GfxInterface* gfx, uint8_t x, uint8_t y, uint8_t n)
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

void Chip::f_FX0A_getKey(uint8_t x)
{
    auto optKeyPressed = getPressedKey();
    if (!optKeyPressed)
    {
        programCounter -= 2u;
    }
    else
    {
        registers[getRegisterName(x)] = optKeyPressed.value();
    }
}

}