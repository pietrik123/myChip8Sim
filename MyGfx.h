#pragma once

#include "SDL2/SDL.h"

class MyGfx
{
    SDL_Window* window;
    SDL_Surface* scrSurf;
    SDL_Renderer* renderer;
    
    int width, height, scale;
    
    bool* pixelsFilled;
    
    bool init();
public:
    MyGfx(int w, int h, int s);
    ~MyGfx();
    
    void updateDisplay();
    
    void clearDisplay();
    void fillPixel(int x, int y);
    void clearPixel(int x, int y);
    bool isPixelFilled(int x, int y);
};