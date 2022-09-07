#include "SDL.h"

#include <iostream>

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    
    auto initSdl = [](SDL_Window* w, SDL_Renderer* r)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cout << "could not init video\n";
            return false;
        }
    
        w = SDL_CreateWindow("CHIP8 sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                64, 32, SDL_WINDOW_SHOWN);
        if (!w)
        {
            std::cout << "Could not create windows!\n";
            return false;
        }
        r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
        if (!r)
        {
            std::cout << "Could not create renderer!\n";
            return false;
        }
        
        
        return true;
    };
    
    int* ptr = nullptr;
    auto f2 = [](int* p)
    {
        p = new int;
        *p = 10;
    };
    
    f2(ptr);
    
    if (!ptr)
    {
        std::cout << "broken ptr\n";
        return -1;
    }
    
    delete ptr;
    
    if (!initSdl(window, renderer))
    {
        std::cout << "init failed\n";
        return -1;
    }
        
    std::cout << "init ok\n";
    
    if (window && renderer) {
        SDL_bool done = SDL_FALSE;

        while (!done) {
            SDL_Event event;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, 32, 20, 30, 24);
            SDL_RenderDrawLine(renderer, 30, 24, 34, 24);
            SDL_RenderDrawLine(renderer, 34, 24, 32, 20);
            
            const SDL_Rect r {0,0,20,20};
            SDL_SetRenderDrawColor(renderer, 0x0,0x66,0x0,0xff);
            SDL_RenderFillRect(renderer, &r);
            SDL_SetRenderDrawColor(renderer, 0x0,0x0,0x0,0xff);
            
            
            
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                }
            }
        }
    }
    else
    {
        std::cout << "Broken w or r!\n";
    }
    

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    
    SDL_Quit();
    return 0;
}