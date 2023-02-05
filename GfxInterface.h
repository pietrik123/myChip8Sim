#pragma once

#pragma once

class GfxInterface
{
public:
    virtual void updateDisplay() = 0;
    virtual void clearDisplay() = 0;
    virtual void fillPixel(int x, int y) = 0;
    virtual void clearPixel(int x, int y) = 0;
    virtual bool isPixelFilled(int x, int y) = 0;
};