#include "MyGfx.h"

#include <stdexcept>
#include <iostream>

MyGfx::MyGfx(int w, int h, int s) : pixelsFilled(new bool [w*h])
{
    width = w;
    height = h;
    scale = s;

    for (int i=0; i<width*height; i++)
    {
        pixelsFilled[i] = true;
    }
    
    if (!init())
    {
        throw std::runtime_error("Could not initialize GFX!\n");
    }
}

MyGfx::~MyGfx()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool MyGfx::init()
{
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "Could not init video!\n";
        return false;
    }
    window = SDL_CreateWindow("CHIP8 sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width*scale, height*scale, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Could not create a window!\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Failed to create renderer!\n";
        return false;
    }
    
    int res = SDL_RenderSetScale(renderer, static_cast<float>(scale), static_cast<float>(scale));
    if (res != 0)
    {
        std::cout << "Failed to set scale! " << res << "\n";
        return false;
    }
    
    if (0 != SDL_SetRenderDrawColor(renderer, 0x0,0x0,0x0,0xff))
    {
        std::cout << "Failed to set draw color!\n";
        return false;
    }
    
    return true;
}

void MyGfx::clearDisplay()
{
    SDL_SetRenderDrawColor(renderer, 0x0, 0x66, 0x0, 0xff);
    SDL_RenderClear(renderer);
    for (int i=0; i<width*height; i++)
    {
        pixelsFilled[i] = false;
    }
}

void MyGfx::fillPixel(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xff);
    SDL_RenderDrawPoint(renderer, x, y);
    pixelsFilled[x*height + y] = true;
}

void MyGfx::clearPixel(int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 0x0, 0x66, 0x0, 0xff);
    SDL_RenderDrawPoint(renderer, x, y);
    pixelsFilled[x*height + y] = false;
}

bool MyGfx::isPixelFilled(int x, int y)
{
    return pixelsFilled[x*height + y];
}

void MyGfx::updateDisplay()
{
    SDL_RenderPresent(renderer);
}

